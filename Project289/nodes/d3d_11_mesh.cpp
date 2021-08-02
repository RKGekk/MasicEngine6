#include "d3d_11_mesh.h"
#include "../bindable/vertex_buffer_bindable.h"
#include "../bindable/index_buffer_bindable.h"
#include "../bindable/vertex_shader_bindable.h"
#include "../bindable/pixel_shader_bindable.h"
#include "../bindable/shader_resource_bindable.h"
#include "../bindable/input_layout_bindable.h"
#include "../bindable/topology_bindable.h"
#include "../bindable/sampler_state_bindable.h"
#include "../bindable/depth_stencil_state_bindable.h"
#include "../bindable/rasterizer_state_bindable.h"
#include "../bindable/blend_state_bindable.h"
#include "../bindable/vertex_constant_buffer_bindable.h"
#include "../bindable/pixel_constant_buffer_bindable.h"
#include "../engine/engine.h"
#include "../engine/d3d_renderer11.h"
#include "../actors/mesh_render_component.h"
#include "../graphics/material_texture.h"
#include "../nodes/light_manager.h"

D3D11Mesh::D3D11Mesh(int mesh_id, BaseRenderComponent* renderComponent, const DirectX::XMFLOAT4X4* pMatrix) : D3D11Mesh(mesh_id, renderComponent, DirectX::XMLoadFloat4x4(pMatrix), DirectX::XMMatrixIdentity(), true) {}

D3D11Mesh::D3D11Mesh(int mesh_id, BaseRenderComponent* renderComponent, DirectX::FXMMATRIX to, DirectX::CXMMATRIX from, bool calculate_from) : D3D11Drawable(nullptr, to, from, calculate_from) {
	m_mesh_id = mesh_id;

	MeshRenderComponent* mrc = static_cast<MeshRenderComponent*>(renderComponent);
	MeshHolder& mesh = mrc->GetMesh(mesh_id);

	D3DRenderer11* renderer = static_cast<D3DRenderer11*>(g_pApp->GetRenderer());
	ID3D11Device* device = renderer->GetDevice();

	std::unique_ptr<VertexBufferBindable> vb = std::make_unique<VertexBufferBindable>(device, mesh.vertices);
	AddBind(BindableType::vertex_buffer_bindable, std::move(vb));

	std::unique_ptr<IndexBufferBindable> ibuf = std::make_unique<IndexBufferBindable>(device, mesh.indices);
	AddBind(BindableType::index_buffer, std::move(ibuf));

	bool can_draw = m_vs.Initialize(
		device,
		ShaderGonfig{}
			.set_shader_name(s2w(mrc->GetVertexShaderResource()))
			.set_description(mrc->GetLayout())
	);

	auto pvs = std::make_unique<VertexShaderBindable>(device, m_vs.GetBuffer(), m_vs.GetShader());
	ID3DBlob* pvsbc = pvs->GetBytecode();
	AddBind(BindableType::vertex_shader, std::move(pvs));

	can_draw &= m_ps.Initialize(
		device,
		ShaderGonfig{}
			.set_shader_name(s2w(mrc->GetPixelShaderResource()))
			.set_description({})
	);

	auto pps = std::make_unique<PixelShaderBindable>(device, m_ps.GetBuffer(), m_ps.GetShader());
	ID3DBlob* ppsbc = pps->GetBytecode();
	AddBind(BindableType::pixel_shader, std::move(pps));

	std::unique_ptr<ShaderResourceBindable> srv0 = std::make_unique<ShaderResourceBindable>(device, mesh.textures[0].GetTextureResourceView());
	AddBind(BindableType::shader_resource, std::move(srv0));

	std::unique_ptr<InputLayoutBindable> inputLayout = std::make_unique<InputLayoutBindable>(device, mrc->GetLayout(), pvsbc);
	AddBind(BindableType::input_layout, std::move(inputLayout));

	std::unique_ptr<TopologyBindable> topology = std::make_unique<TopologyBindable>(device, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(BindableType::topology, std::move(topology));

	std::unique_ptr<SamplerStateBindable> samplerState = std::make_unique<SamplerStateBindable>(device);
	AddBind(BindableType::sampler_state, std::move(samplerState));

	std::unique_ptr<DepthStencilStateBindable> depthStencilState = std::make_unique<DepthStencilStateBindable>(device);
	AddBind(BindableType::depth_stencil_state, std::move(depthStencilState));

	std::unique_ptr<RasterizerStateBindable> rasterizerState = std::make_unique<RasterizerStateBindable>(device);
	AddBind(BindableType::rasterizer_state, std::move(rasterizerState));

	if (mrc->GetAlphaBlend()) {
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
		std::unique_ptr<BlendStateBindable> blendState = std::make_unique<BlendStateBindable>(device, alphaToCoverageDesc);
		AddBind(BindableType::blend_state, std::move(blendState));
	}
	else {
		std::unique_ptr<BlendStateBindable> blendState = std::make_unique<BlendStateBindable>(device);
		AddBind(BindableType::blend_state, std::move(blendState));
	}

	CB_VS_VertexShader mt;
	DirectX::XMStoreFloat4x4(&mt.lwvpMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mt.invWorldMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mt.worldMatrix, DirectX::XMMatrixIdentity());

	std::unique_ptr<VertexConstantBufferBindable<CB_VS_VertexShader>> cbvs = std::make_unique<VertexConstantBufferBindable<CB_VS_VertexShader>>(device, mt);
	AddBind(BindableType::vertex_constant_buffer, std::move(cbvs));

	CB_PS_PixelShader_Light lt;
	DirectionalLight dl;
	dl.Ambient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	dl.Diffuse = DirectX::XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	dl.Specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	dl.Direction = DirectX::XMFLOAT3(0.0f, -1.0f, 1.0f);
	dl.Pad = 1.0f;
	lt.gDirLights[0] = dl;
	lt.gEyePosW = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	std::unique_ptr<PixelConstantBufferBindable<CB_PS_PixelShader_Light>> cbps1 = std::make_unique<PixelConstantBufferBindable<CB_PS_PixelShader_Light>>(device, lt);
	AddBind(BindableType::pixel_constant_buffer, std::move(cbps1));
}

D3D11Mesh::~D3D11Mesh() {}

HRESULT D3D11Mesh::VOnUpdate(Scene* pScene, float const elapsedMs) {
	return S_OK;
}

HRESULT D3D11Mesh::VPreRender(Scene* pScene) {
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

	static_cast<VertexConstantBufferBindable<CB_VS_VertexShader>*>(m_binds[BindableType::vertex_constant_buffer].get())->Update(deviceContext, mt);

	CB_PS_PixelShader_Light lt;
	LightManager* lightManager = pScene->GetLightManager();
	lightManager->CopyLighting(&lt, this);

	static_cast<PixelConstantBufferBindable<CB_PS_PixelShader_Light>*>(m_binds[BindableType::pixel_constant_buffer].get())->Update(deviceContext, lt);

	return S_OK;
}