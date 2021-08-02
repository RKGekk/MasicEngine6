#include "d3d_11_geometry_pipeline.h"

#include <algorithm>

#include "../actors/mesh_render_component.h"
#include "../actors/smoke_component.h"
#include "../engine/engine.h"
#include "../engine/d3d_renderer11.h"
#include "../nodes/light_manager.h"
#include "../tools/memory_utility.h"
#include "../tools/com_exception.h"

D3D11GeometryPipeline::D3D11GeometryPipeline(SmokeComponent& data, const DirectX::XMFLOAT4X4* pMatrix) : D3D11GeometryPipeline(data, DirectX::XMLoadFloat4x4(pMatrix), DirectX::XMMatrixIdentity(), true) {}

D3D11GeometryPipeline::D3D11GeometryPipeline(SmokeComponent& data, DirectX::FXMMATRIX to, DirectX::CXMMATRIX from, bool calulate_from) : SceneNode(&data, RenderPass::RenderPass_Actor, to, from, calulate_from) {
	D3DRenderer11* renderer = static_cast<D3DRenderer11*>(g_pApp->GetRenderer());
	ID3D11Device* device = renderer->GetDevice();

	m_vertex_stride = sizeof(SmokePointSprite);
	std::vector<SmokePointSprite> mesh = data.GetMesh();
	m_vertex_count = mesh.size();
	D3D11_BUFFER_DESC vbd = {};
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0u;
	vbd.ByteWidth = UINT(m_vertex_stride * m_vertex_count);
	vbd.StructureByteStride = m_vertex_stride;

	D3D11_SUBRESOURCE_DATA vsd = {};
	vsd.pSysMem = mesh.data();

	HRESULT hr = device->CreateBuffer(&vbd, &vsd, &m_pVertexBuffer);
	COM_ERROR_IF_FAILED(hr, "Failed to create vertex buffer");

	
	const std::string& vertex_shader_resource_name = data.GetVertexShaderResourece();
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& vertex_shader_input_layout = data.GetVertexShaderInputLayout();
	m_vs = std::make_shared<VertexShader>();
	m_vs->Initialize(
		device,
		ShaderGonfig{}
			.set_shader_name(s2w(vertex_shader_resource_name))
			.set_description(vertex_shader_input_layout)
	);
	m_pVertexShader = m_vs->GetShader();
	m_pVSBytecodeBlob = m_vs->GetBuffer();

	const std::string& geometry_shader_resource_name = data.GetGeometryShaderResourece();
	m_gs = std::make_shared<GeometryShader>();
	m_gs->Initialize(
		device,
		ShaderGonfig{}
			.set_shader_name(s2w(geometry_shader_resource_name))
			.set_description({})
	);
	m_pGeometryShader = m_gs->GetShader();
	m_pGSBytecodeBlob = m_gs->GetBuffer();

	const std::string& pixel_shader_resource_name = data.GetPixelShaderResourece();
	m_ps = std::make_shared<PixelShader>();
	m_ps->Initialize(
		device,
		ShaderGonfig{}
			.set_shader_name(s2w(pixel_shader_resource_name))
			.set_description({})
	);
	m_pPixelShader = m_ps->GetShader();
	m_pPSBytecodeBlob = m_ps->GetBuffer();

	m_texture = MaterialTexture(device, data.GetTexture());
	m_srv_start_slot = 0;
	m_srv0 = m_texture.GetTextureResourceView();

	m_pVSInputLayout = m_vs->GetInputLayout();

	m_primitive_topology_type = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

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

	CB_VS_VertexShader_PerObject vs_mt;
	DirectX::XMStoreFloat4x4(&vs_mt.worldMatrix, DirectX::XMMatrixIdentity());

	m_vs_cb_slot1 = 0;
	D3D11_BUFFER_DESC vscbd1;
	vscbd1.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vscbd1.Usage = D3D11_USAGE_DYNAMIC;
	vscbd1.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vscbd1.MiscFlags = 0u;
	vscbd1.ByteWidth = sizeof(CB_VS_VertexShader_PerObject);
	vscbd1.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA vscsd1 = {};
	vscsd1.pSysMem = &vs_mt;

	hr = device->CreateBuffer(&vscbd1, &vscsd1, &m_pVSConstantBuffer1);
	COM_ERROR_IF_FAILED(hr, "Failed to create constant buffer");

	CB_GS_GeometryShader_PerPrame mt1;
	mt1.gEyePosW = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_atlas_width = 1.0f / ((float)data.GetAtlasWidth());
	m_atlas_height = 1.0f / ((float)data.GetAtlasHeight());
	mt1.gAtlasDim = { m_atlas_width, m_atlas_height, 0.0f, 0.0f };

	m_gs_cb_slot1 = 0;
	D3D11_BUFFER_DESC gscbd1;
	gscbd1.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	gscbd1.Usage = D3D11_USAGE_DYNAMIC;
	gscbd1.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	gscbd1.MiscFlags = 0u;
	gscbd1.ByteWidth = sizeof(CB_GS_GeometryShader_PerPrame);
	gscbd1.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA gscsd1 = {};
	gscsd1.pSysMem = &mt1;

	hr = device->CreateBuffer(&gscbd1, &gscsd1, &m_pGSConstantBuffer1);
	COM_ERROR_IF_FAILED(hr, "Failed to create constant buffer");

	CB_GS_GeometryShader_PerObject mt2;
	DirectX::XMStoreFloat4x4(&mt2.gViewProj, DirectX::XMMatrixIdentity());

	m_gs_cb_slot2 = 1;
	D3D11_BUFFER_DESC gscbd2;
	gscbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	gscbd2.Usage = D3D11_USAGE_DYNAMIC;
	gscbd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	gscbd2.MiscFlags = 0u;
	gscbd2.ByteWidth = sizeof(CB_GS_GeometryShader_PerObject);
	gscbd2.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA gscsd2 = {};
	gscsd2.pSysMem = &mt2;

	hr = device->CreateBuffer(&gscbd2, &gscsd2, &m_pGSConstantBuffer2);
	COM_ERROR_IF_FAILED(hr, "Failed to create constant buffer");

	m_ps_cb_slot = 0;
}

D3D11GeometryPipeline::~D3D11GeometryPipeline() {}

HRESULT D3D11GeometryPipeline::VRender(Scene* pScene) {
	D3DRenderer11* renderer = static_cast<D3DRenderer11*>(g_pApp->GetRenderer());
	ID3D11DeviceContext* deviceContext = renderer->GetDeviceContext();

	const UINT offset = 0u;
	deviceContext->IASetVertexBuffers(0u, 1u, m_pVertexBuffer.GetAddressOf(), &m_vertex_stride, &offset);
	deviceContext->VSSetShader(m_pVertexShader, nullptr, 0u);
	deviceContext->GSSetShader(m_pGeometryShader, nullptr, 0u);
	deviceContext->PSSetShader(m_pPixelShader, nullptr, 0u);
	deviceContext->PSSetShaderResources(m_srv_start_slot, 1, &m_srv0);
	deviceContext->IASetInputLayout(m_pVSInputLayout);
	deviceContext->IASetPrimitiveTopology(m_primitive_topology_type);
	deviceContext->PSSetSamplers(0, 1, m_sampleState.GetAddressOf());
	deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);
	deviceContext->RSSetState(m_rasterState.Get());
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	deviceContext->OMSetBlendState(m_blendState.Get(), blendFactor, 0xffffffff);
	deviceContext->VSSetConstantBuffers(m_vs_cb_slot1, 1u, m_pVSConstantBuffer1.GetAddressOf());
	deviceContext->GSSetConstantBuffers(m_gs_cb_slot1, 1u, m_pGSConstantBuffer1.GetAddressOf());
	deviceContext->GSSetConstantBuffers(m_gs_cb_slot2, 1u, m_pGSConstantBuffer2.GetAddressOf());
	deviceContext->PSSetConstantBuffers(m_ps_cb_slot, 1u, m_pGSConstantBuffer1.GetAddressOf());

	deviceContext->Draw(m_vertex_count, 0u);
	deviceContext->GSSetShader(nullptr, nullptr, 0u);

	return S_OK;
}

HRESULT D3D11GeometryPipeline::VOnUpdate(Scene* pScene, float const elapsedMs) {
	return S_OK;
}

HRESULT D3D11GeometryPipeline::VPreRender(Scene* pScene) {
	StrongActorPtr pActor = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(VFindMyActor()));

	D3DRenderer11* renderer = static_cast<D3DRenderer11*>(g_pApp->GetRenderer());
	ID3D11DeviceContext* deviceContext = renderer->GetDeviceContext();
	SceneNode::VPreRender(pScene);

	const std::shared_ptr<CameraNode> camera = pScene->GetCamera();
	std::shared_ptr<SmokeComponent> pSmokeComponent = MakeStrongPtr(pActor->GetComponent<SmokeComponent>(SmokeComponent::g_Name));
	//DirectX::XMFLOAT4X4 world = pScene->GetTopMatrix4x4f();
	DirectX::XMMATRIX fromWorld = pScene->GetCamera()->VGet().FromWorld();

	std::vector<SmokePointSprite> sp = pSmokeComponent->GetMesh();

	std::sort(
		sp.begin(),
		sp.end(),
		[&](const SmokePointSprite& lhs, const SmokePointSprite& rhs) {
			DirectX::XMVECTOR lhs_world = DirectX::XMLoadFloat3(&lhs.Pos);
			lhs_world = DirectX::XMVector4Transform(lhs_world, fromWorld);
			float lhs_z = DirectX::XMVectorGetZ(lhs_world);

			DirectX::XMVECTOR rhs_world = DirectX::XMLoadFloat3(&rhs.Pos);
			rhs_world = DirectX::XMVector4Transform(rhs_world, fromWorld);
			float rhs_z = DirectX::XMVectorGetZ(rhs_world);

			return lhs_z > rhs_z;
		}
	);

	m_vertex_count = sp.size();
	D3D11_MAPPED_SUBRESOURCE vsVertexMsr;
	HRESULT hr = deviceContext->Map(m_pVertexBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &vsVertexMsr);
	COM_ERROR_IF_FAILED(hr, "Failed to update vertex buffer");
	memcpy(vsVertexMsr.pData, sp.data(), m_vertex_stride * m_vertex_count);
	deviceContext->Unmap(m_pVertexBuffer.Get(), 0u);

	CB_VS_VertexShader_PerObject vs_mt1;
	//DirectX::XMStoreFloat4x4(&vs_mt1.worldMatrix, DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(world._41, world._42, world._43)));
	DirectX::XMStoreFloat4x4(&vs_mt1.worldMatrix, DirectX::XMMatrixIdentity());
	//vs_mt1.worldMatrix = pScene->GetTopMatrix4x4fT();

	D3D11_MAPPED_SUBRESOURCE vsmsr1;
	hr = deviceContext->Map(m_pVSConstantBuffer1.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &vsmsr1);
	COM_ERROR_IF_FAILED(hr, "Failed to update constant buffer");
	memcpy(vsmsr1.pData, &vs_mt1, sizeof(CB_VS_VertexShader_PerObject));
	deviceContext->Unmap(m_pVSConstantBuffer1.Get(), 0u);

	CB_GS_GeometryShader_PerPrame mt1;
	mt1.gEyePosW = camera->GetPosition4();
	mt1.gAtlasDim = { m_atlas_width, m_atlas_height, 0.0f, 0.0f };

	D3D11_MAPPED_SUBRESOURCE gsmsr1;
	hr = deviceContext->Map(m_pGSConstantBuffer1.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &gsmsr1);
	COM_ERROR_IF_FAILED(hr, "Failed to update constant buffer");
	memcpy(gsmsr1.pData, &mt1, sizeof(CB_GS_GeometryShader_PerPrame));
	deviceContext->Unmap(m_pGSConstantBuffer1.Get(), 0u);

	CB_GS_GeometryShader_PerObject mt2;
	mt2.gViewProj = camera->GetViewProjection4x4T();

	D3D11_MAPPED_SUBRESOURCE gsmsr2;
	hr = deviceContext->Map(m_pGSConstantBuffer2.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &gsmsr2);
	COM_ERROR_IF_FAILED(hr, "Failed to update constant buffer");
	memcpy(gsmsr2.pData, &mt2, sizeof(CB_GS_GeometryShader_PerObject));
	deviceContext->Unmap(m_pGSConstantBuffer2.Get(), 0u);

	return S_OK;
}

HRESULT D3D11GeometryPipeline::VPostRender(Scene* pScene) {
	return SceneNode::VPostRender(pScene);
}
