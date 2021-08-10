#include "d3d_11_stat_pipeline.h"

#include <algorithm>

#include "../actors/mesh_render_component.h"
#include "../actors/stats_render_component.h"
#include "../actors/pers_texture_anim_state_component.h"
#include "../actors/stats_render_component.h"
#include "../engine/engine.h"
#include "../engine/d3d_renderer11.h"
#include "../nodes/light_manager.h"
#include "../nodes/shadow_manager.h"
#include "../tools/memory_utility.h"
#include "../tools/com_exception.h"

D3D11StatPipeline::D3D11StatPipeline(int mesh_id, StatsRenderComponent& data, const DirectX::XMFLOAT4X4* pMatrix) : D3D11StatPipeline(mesh_id, data, DirectX::XMLoadFloat4x4(pMatrix), DirectX::XMMatrixIdentity(), true) {}

D3D11StatPipeline::D3D11StatPipeline(int mesh_id, StatsRenderComponent& data, DirectX::FXMMATRIX to, DirectX::CXMMATRIX from, bool calulate_from) : SceneNode(nullptr, RenderPass::RenderPass_Actor, to, from, calulate_from) {
	m_mesh_id = mesh_id;

	D3DRenderer11* renderer = static_cast<D3DRenderer11*>(g_pApp->GetRenderer());
	ID3D11Device* device = renderer->GetDevice();

	m_vertex_stride = sizeof(Vertex);
	MeshHolder& mesh = data.GetMesh(mesh_id);
	m_vertex_count = mesh.vertices.size();
	D3D11_BUFFER_DESC vbd = {};
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.CPUAccessFlags = 0u;
	vbd.MiscFlags = 0u;
	vbd.ByteWidth = UINT(m_vertex_stride * m_vertex_count);
	vbd.StructureByteStride = m_vertex_stride;

	D3D11_SUBRESOURCE_DATA vsd = {};
	vsd.pSysMem = mesh.vertices.data();

	HRESULT hr = device->CreateBuffer(&vbd, &vsd, &m_pVertexBuffer);
	COM_ERROR_IF_FAILED(hr, "Failed to create vertex buffer");

	m_index_count = (UINT)mesh.indices.size();
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = UINT(m_index_count * sizeof(DWORD));
	ibd.StructureByteStride = sizeof(DWORD);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = mesh.indices.data();

	hr = device->CreateBuffer(&ibd, &isd, &m_pIndexBuffer);
	COM_ERROR_IF_FAILED(hr, "Failed to create index buffer");

	const std::string& vertex_shader_resource_name = data.GetVertexShaderResource();
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& vertex_shader_input_layout = data.GetLayout();
	m_vs = std::make_shared<VertexShader>();
	m_vs->Initialize(
		device,
		ShaderGonfig{}
		.set_shader_name(s2w(vertex_shader_resource_name))
		.set_description(vertex_shader_input_layout)
	);
	m_pVertexShader = m_vs->GetShader();
	m_pVSBytecodeBlob = m_vs->GetBuffer();

	const std::string& pixel_shader_resource_name = data.GetPixelShaderResource();
	m_ps = std::make_shared<PixelShader>();
	m_ps->Initialize(
		device,
		ShaderGonfig{}
		.set_shader_name(s2w(pixel_shader_resource_name))
		.set_description({})
	);
	m_pPixelShader = m_ps->GetShader();
	m_pPSBytecodeBlob = m_ps->GetBuffer();

	m_diffuse_srv_start_slot = 0;
	m_diffuse_srv0 = mesh.textures[0].GetTextureResourceView();

	m_normal_srv_start_slot = 1;
	m_normal_srv1 = mesh.textures[1].GetTextureResourceView();

	m_shadow_srv_start_slot = 2;

	m_pVSInputLayout = m_vs->GetInputLayout();

	m_primitive_topology_type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	D3D11_SAMPLER_DESC samplerDesc0;
	samplerDesc0.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc0.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc0.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc0.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc0.MipLODBias = 0.0f;
	samplerDesc0.MaxAnisotropy = 4;
	samplerDesc0.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc0.BorderColor[0] = 0;
	samplerDesc0.BorderColor[1] = 0;
	samplerDesc0.BorderColor[2] = 0;
	samplerDesc0.BorderColor[3] = 0;
	samplerDesc0.MinLOD = 0;
	samplerDesc0.MaxLOD = D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&samplerDesc0, &m_sampleState0);
	COM_ERROR_IF_FAILED(hr, "Failed to create sampler state");

	D3D11_SAMPLER_DESC samplerDesc1;
	samplerDesc1.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc1.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc1.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc1.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc1.MipLODBias = 0.0f;
	samplerDesc1.MaxAnisotropy = 4;
	samplerDesc1.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc1.BorderColor[0] = 0;
	samplerDesc1.BorderColor[1] = 0;
	samplerDesc1.BorderColor[2] = 0;
	samplerDesc1.BorderColor[3] = 0;
	samplerDesc1.MinLOD = 0;
	samplerDesc1.MaxLOD = D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&samplerDesc1, &m_sampleState1);
	COM_ERROR_IF_FAILED(hr, "Failed to create sampler state");

	D3D11_SAMPLER_DESC samplerDesc2;
	samplerDesc2.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc2.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc2.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc2.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc2.MipLODBias = 0.0f;
	samplerDesc2.MaxAnisotropy = 1;
	samplerDesc2.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	samplerDesc2.BorderColor[0] = 0;
	samplerDesc2.BorderColor[1] = 0;
	samplerDesc2.BorderColor[2] = 0;
	samplerDesc2.BorderColor[3] = 0;
	samplerDesc2.MinLOD = 0;
	samplerDesc2.MaxLOD = D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&samplerDesc2, &m_sampleState2);
	COM_ERROR_IF_FAILED(hr, "Failed to create sampler state");

	//D3D11_SAMPLER_COMPARSION samplerDesc2;

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

	/*D3D11_BLEND_DESC alphaToCoverageDesc = CD3D11_BLEND_DESC(CD3D11_DEFAULT());
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
	COM_ERROR_IF_FAILED(hr, "Failed to create blend state.");*/
	D3D11_BLEND_DESC blendDesc = { 0 };
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = false;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = device->CreateBlendState(&blendDesc, m_blendState.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create blend state.");

	CB_VS_VertexShader mt;
	DirectX::XMStoreFloat4x4(&mt.lwvpMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mt.invWorldMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mt.worldMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mt.gTexTransform, DirectX::XMMatrixIdentity());

	m_vs_cb_slot0 = 0;
	m_ps_cb_slot0 = 0;
	D3D11_BUFFER_DESC vscbd;
	vscbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vscbd.Usage = D3D11_USAGE_DYNAMIC;
	vscbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vscbd.MiscFlags = 0u;
	vscbd.ByteWidth = sizeof(CB_VS_VertexShader);
	vscbd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA vscsd = {};
	vscsd.pSysMem = &mt;

	hr = device->CreateBuffer(&vscbd, &vscsd, &m_pVSConstantBuffer0);
	COM_ERROR_IF_FAILED(hr, "Failed to create constant buffer");

	CB_PS_PixelShader_Light_All lt;
	lt.gDirLightCount = 1;
	lt.gPointLightCount = 0;
	lt.gSpotLightCount = 0;
	lt.gFogStart = 100.0f;
	lt.gFogRange = 900.0f;
	lt.gFogColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectionalLight dl;
	dl.Ambient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	dl.Diffuse = DirectX::XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	dl.Specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	dl.Direction = DirectX::XMFLOAT3(0.0f, -1.0f, 1.0f);
	dl.Pad = 1.0f;
	lt.gDirLights[0] = dl;
	lt.gEyePosW = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_ps_cb_slot1 = 1;
	D3D11_BUFFER_DESC pscbd;
	pscbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pscbd.Usage = D3D11_USAGE_DYNAMIC;
	pscbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pscbd.MiscFlags = 0u;
	pscbd.ByteWidth = sizeof(CB_PS_PixelShader_Light_All);
	pscbd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA pscsd = {};
	pscsd.pSysMem = &lt;

	hr = device->CreateBuffer(&pscbd, &pscsd, &m_pPSConstantBuffer1);
	COM_ERROR_IF_FAILED(hr, "Failed to create constant buffer");
}

D3D11StatPipeline::~D3D11StatPipeline() {}

HRESULT D3D11StatPipeline::VRender(Scene* pScene) {
	D3DRenderer11* renderer = static_cast<D3DRenderer11*>(g_pApp->GetRenderer());
	ID3D11DeviceContext* deviceContext = renderer->GetDeviceContext();

	const UINT offset = 0u;
	deviceContext->IASetVertexBuffers(0u, 1u, m_pVertexBuffer.GetAddressOf(), &m_vertex_stride, &offset);
	deviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
	deviceContext->VSSetShader(m_pVertexShader, nullptr, 0u);
	deviceContext->PSSetShader(m_pPixelShader, nullptr, 0u);
	deviceContext->PSSetShaderResources(m_diffuse_srv_start_slot, 1, &m_diffuse_srv0);
	deviceContext->PSSetShaderResources(m_normal_srv_start_slot, 1, &m_normal_srv1);
	deviceContext->PSSetShaderResources(m_shadow_srv_start_slot, 1, &m_shadow_srv2);
	deviceContext->IASetInputLayout(m_pVSInputLayout);
	deviceContext->IASetPrimitiveTopology(m_primitive_topology_type);
	deviceContext->PSSetSamplers(0, 1, m_sampleState0.GetAddressOf());
	deviceContext->PSSetSamplers(1, 1, m_sampleState1.GetAddressOf());
	deviceContext->PSSetSamplers(2, 1, m_sampleState2.GetAddressOf());
	//deviceContext->PSSetSamplers(2, 1, m_sampleState1.GetAddressOf());
	deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);
	deviceContext->RSSetState(m_rasterState.Get());
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	deviceContext->OMSetBlendState(m_blendState.Get(), blendFactor, 0xffffffff);
	deviceContext->VSSetConstantBuffers(m_vs_cb_slot0, 1u, m_pVSConstantBuffer0.GetAddressOf());
	deviceContext->PSSetConstantBuffers(m_ps_cb_slot0, 1u, m_pVSConstantBuffer0.GetAddressOf());
	deviceContext->PSSetConstantBuffers(m_ps_cb_slot1, 1u, m_pPSConstantBuffer1.GetAddressOf());

	//deviceContext->Draw(m_vertex_count, 0u);
	deviceContext->DrawIndexed(m_index_count, 0u, 0u);

	return S_OK;
}

HRESULT D3D11StatPipeline::VOnUpdate(Scene* pScene, float const elapsedMs) {
	return S_OK;
}

HRESULT D3D11StatPipeline::VPreRender(Scene* pScene) {
	StrongActorPtr pActor = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(VFindMyActor()));

	D3DRenderer11* renderer = static_cast<D3DRenderer11*>(g_pApp->GetRenderer());
	ID3D11DeviceContext* deviceContext = renderer->GetDeviceContext();
	SceneNode::VPreRender(pScene);

	const std::shared_ptr<CameraNode> camera = pScene->GetCamera();
	std::shared_ptr<StatsRenderComponent> pMeshComponent = MakeStrongPtr(pActor->GetComponent<StatsRenderComponent>(StatsRenderComponent::g_Name));
	//pMeshComponent->UpdateFromScenePrerender(g_pApp->GetTimer().DeltaTime());
	MeshHolder& mesh = pMeshComponent->GetMesh(m_mesh_id);

	ShadowManager* shadowManager = pScene->GetShadowManager();
	m_shadow_srv2 = shadowManager->ShadowDepthMapSRV();

	CB_VS_VertexShader mt;
	mt.lwvpMatrix = camera->GetWorldViewProjection4x4T(pScene);
	mt.invWorldMatrix = pScene->GetTopInvMatrix4x4f();
	mt.worldMatrix = pScene->GetTopMatrix4x4fT();
	//mt.lwvpMatrix = camera->GetWorldViewProjection4x4T(pMeshComponent->GetTransform());
	//mt.invWorldMatrix = pMeshComponent->GetInvTransform4x4f();
	//mt.worldMatrix = pMeshComponent->GetTransform4x4fT();
	mt.shadowTransform = shadowManager->GetWorldShadowTransform4x4T(pScene);
	mt.material.Ambient = mesh.material.Ambient;
	mt.material.Ambient.w = pMeshComponent->GetColor().w;
	mt.material.Diffuse = mesh.material.Diffuse;
	mt.material.Specular = mesh.material.Specular;
	DirectX::XMStoreFloat4x4(&mt.gTexTransform, DirectX::XMMatrixIdentity());

	D3D11_MAPPED_SUBRESOURCE vsmsr;
	HRESULT hr = deviceContext->Map(m_pVSConstantBuffer0.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &vsmsr);
	COM_ERROR_IF_FAILED(hr, "Failed to update constant buffer");
	memcpy(vsmsr.pData, &mt, sizeof(CB_VS_VertexShader));
	deviceContext->Unmap(m_pVSConstantBuffer0.Get(), 0u);

	LightManager* lightManager = pScene->GetLightManager();
	CB_PS_PixelShader_Light_All lt;
	lightManager->CopyLighting(&lt, this);
	lt.gFogStart = pMeshComponent->GetFogStart();
	lt.gFogRange = pMeshComponent->GetFogRange();
	lt.gFogColor = pMeshComponent->GetFogColor();
	lt.gEyePosW = camera->GetPosition3();

	D3D11_MAPPED_SUBRESOURCE psmsr;
	hr = deviceContext->Map(m_pPSConstantBuffer1.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &psmsr);
	COM_ERROR_IF_FAILED(hr, "Failed to update constant buffer");
	memcpy(psmsr.pData, &lt, sizeof(CB_PS_PixelShader_Light_All));
	deviceContext->Unmap(m_pPSConstantBuffer1.Get(), 0u);

	return S_OK;
}

HRESULT D3D11StatPipeline::VPostRender(Scene* pScene) {
	return SceneNode::VPostRender(pScene);
}