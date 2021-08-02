#include "d3d_11_basic_pipeline.h"

#include "../actors/mesh_render_component.h"
#include "../engine/engine.h"
#include "../engine/d3d_renderer11.h"
#include "../nodes/light_manager.h"
#include "../tools/memory_utility.h"
#include "../tools/com_exception.h"

D3D11BasicPipeline::D3D11BasicPipeline(MeshHolder& mesh_data, std::shared_ptr<PixelShader> ps, std::shared_ptr<VertexShader> vs, const DirectX::XMFLOAT4X4* pMatrix) : D3D11BasicPipeline(mesh_data, ps, vs, DirectX::XMLoadFloat4x4(pMatrix), DirectX::XMMatrixIdentity(), true) {}

D3D11BasicPipeline::D3D11BasicPipeline(MeshHolder& mesh_data, std::shared_ptr<PixelShader> ps, std::shared_ptr<VertexShader> vs, DirectX::FXMMATRIX to, DirectX::CXMMATRIX from, bool calulate_from) : SceneNode(nullptr, RenderPass::RenderPass_Actor, to, from, calulate_from) {
	D3DRenderer11* renderer = static_cast<D3DRenderer11*>(g_pApp->GetRenderer());
	ID3D11Device* device = renderer->GetDevice();

	m_vertex_stride = sizeof(Vertex);
	D3D11_BUFFER_DESC vbd = {};
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.CPUAccessFlags = 0u;
	vbd.MiscFlags = 0u;
	vbd.ByteWidth = UINT(m_vertex_stride * mesh_data.vertices.size());
	vbd.StructureByteStride = m_vertex_stride;

	D3D11_SUBRESOURCE_DATA vsd = {};
	vsd.pSysMem = mesh_data.vertices.data();

	HRESULT hr = device->CreateBuffer(&vbd, &vsd, &m_pVertexBuffer);
	COM_ERROR_IF_FAILED(hr, "Failed to create vertex buffer");

	m_index_count = (UINT)mesh_data.indices.size();
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = UINT(m_index_count * sizeof(DWORD));
	ibd.StructureByteStride = sizeof(DWORD);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = mesh_data.indices.data();

	hr = device->CreateBuffer(&ibd, &isd, &m_pIndexBuffer);
	COM_ERROR_IF_FAILED(hr, "Failed to create index buffer");

	m_vs = vs;
	m_pVertexShader = m_vs->GetShader();
	m_pVSBytecodeBlob = m_vs->GetBuffer();

	m_ps = ps;
	m_pPixelShader = m_ps->GetShader();
	m_pPSBytecodeBlob = m_ps->GetBuffer();
	
	m_srv_start_slot = 0;
	m_srv0 = mesh_data.textures[0].GetTextureResourceView();

	m_pInputLayout = m_vs->GetInputLayout();

	m_primitive_topology_type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	COM_ERROR_IF_FAILED(hr, "Failed to create sampler state");

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = device->CreateDepthStencilState(&depthStencilDesc, m_depthStencilState.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state");

	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	hr = device->CreateRasterizerState(&rasterDesc, m_rasterState.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state");

	D3D11_BLEND_DESC alphaToCoverageDesc = CD3D11_BLEND_DESC(CD3D11_DEFAULT());
	alphaToCoverageDesc.AlphaToCoverageEnable = false;
	alphaToCoverageDesc.IndependentBlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].BlendEnable = true;
	alphaToCoverageDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	alphaToCoverageDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	alphaToCoverageDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	alphaToCoverageDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	alphaToCoverageDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	alphaToCoverageDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = device->CreateBlendState(&alphaToCoverageDesc, m_blendState.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create blend state.");

	CB_VS_VertexShader mt;
	DirectX::XMStoreFloat4x4(&mt.lwvpMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mt.invWorldMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mt.worldMatrix, DirectX::XMMatrixIdentity());

	m_vs_cb_slot = 0;
	D3D11_BUFFER_DESC vscbd;
	vscbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vscbd.Usage = D3D11_USAGE_DYNAMIC;
	vscbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vscbd.MiscFlags = 0u;
	vscbd.ByteWidth = sizeof(CB_VS_VertexShader);
	vscbd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA vscsd = {};
	vscsd.pSysMem = &mt;

	hr = device->CreateBuffer(&vscbd, &vscsd, &m_pVSConstantBuffer);
	COM_ERROR_IF_FAILED(hr, "Failed to create constant buffer");

	CB_PS_PixelShader_Light lt;
	DirectionalLight dl;
	dl.Ambient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	dl.Diffuse = DirectX::XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	dl.Specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	dl.Direction = DirectX::XMFLOAT3(0.0f, -1.0f, 1.0f);
	dl.Pad = 1.0f;
	lt.gDirLights[0] = dl;
	lt.gEyePosW = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_ps_cb_slot = 0;
	D3D11_BUFFER_DESC pscbd;
	pscbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pscbd.Usage = D3D11_USAGE_DYNAMIC;
	pscbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pscbd.MiscFlags = 0u;
	pscbd.ByteWidth = sizeof(CB_PS_PixelShader_Light);
	pscbd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA pscsd = {};
	pscsd.pSysMem = &lt;

	hr = device->CreateBuffer(&pscbd, &pscsd, &m_pPSConstantBuffer);
	COM_ERROR_IF_FAILED(hr, "Failed to create constant buffer");
	
}

D3D11BasicPipeline::~D3D11BasicPipeline() {}

HRESULT D3D11BasicPipeline::VRender(Scene* pScene) {
	D3DRenderer11* renderer = static_cast<D3DRenderer11*>(g_pApp->GetRenderer());
	ID3D11DeviceContext* deviceContext = renderer->GetDeviceContext();

	const UINT offset = 0u;
	deviceContext->IASetVertexBuffers(0u, 1u, m_pVertexBuffer.GetAddressOf(), &m_vertex_stride, &offset);
	deviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
	deviceContext->VSSetShader(m_pVertexShader, nullptr, 0u);
	deviceContext->PSSetShader(m_pPixelShader, nullptr, 0u);
	deviceContext->PSSetShaderResources(m_srv_start_slot, 1, &m_srv0);
	deviceContext->IASetInputLayout(m_pInputLayout);
	deviceContext->IASetPrimitiveTopology(m_primitive_topology_type);
	deviceContext->PSSetSamplers(0, 1, m_sampleState.GetAddressOf());
	deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);
	deviceContext->RSSetState(m_rasterState.Get());
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	deviceContext->OMSetBlendState(m_blendState.Get(), blendFactor, 0xffffffff);
	deviceContext->VSSetConstantBuffers(m_vs_cb_slot, 1u, m_pVSConstantBuffer.GetAddressOf());
	deviceContext->PSSetConstantBuffers(m_ps_cb_slot, 1u, m_pPSConstantBuffer.GetAddressOf());

	deviceContext->DrawIndexed(m_index_count, 0u, 0u);

	return S_OK;
}

HRESULT D3D11BasicPipeline::VOnUpdate(Scene* pScene, float const elapsedMs) {
	return S_OK;
}

HRESULT D3D11BasicPipeline::VPreRender(Scene* pScene) {
	StrongActorPtr pActor = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(VFindMyActor()));

	D3DRenderer11* renderer = static_cast<D3DRenderer11*>(g_pApp->GetRenderer());
	ID3D11DeviceContext* deviceContext = renderer->GetDeviceContext();
	SceneNode::VPreRender(pScene);

	const std::shared_ptr<CameraNode> camera = pScene->GetCamera();

	std::shared_ptr<MeshRenderComponent> pMeshComponent = MakeStrongPtr(pActor->GetComponent<MeshRenderComponent>(MeshRenderComponent::g_Name));
	CB_VS_VertexShader mt;
	mt.lwvpMatrix = camera->GetWorldViewProjection4x4T(pScene);
	mt.invWorldMatrix = pScene->GetTopInvMatrix4x4fT();
	mt.worldMatrix = pScene->GetTopMatrix4x4fT();
	mt.material.Ambient.w = pMeshComponent->GetColor().w;

	D3D11_MAPPED_SUBRESOURCE vsmsr;
	HRESULT hr = deviceContext->Map(m_pVSConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &vsmsr);
	COM_ERROR_IF_FAILED(hr, "Failed to update constant buffer");
	memcpy(vsmsr.pData, &mt, sizeof(CB_VS_VertexShader));
	deviceContext->Unmap(m_pVSConstantBuffer.Get(), 0u);

	CB_PS_PixelShader_Light lt;
	LightManager* lightManager = pScene->GetLightManager();
	lightManager->CopyLighting(&lt, this);

	D3D11_MAPPED_SUBRESOURCE psmsr;
	hr = deviceContext->Map(m_pPSConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &psmsr);
	COM_ERROR_IF_FAILED(hr, "Failed to update constant buffer");
	memcpy(psmsr.pData, &lt, sizeof(CB_PS_PixelShader_Light));
	deviceContext->Unmap(m_pPSConstantBuffer.Get(), 0u);

	return S_OK;
}

HRESULT D3D11BasicPipeline::VPostRender(Scene* pScene) {
	return SceneNode::VPostRender(pScene);
}
