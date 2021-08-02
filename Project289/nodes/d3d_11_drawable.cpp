#include "d3d_11_drawable.h"
#include "../engine/engine.h"
#include "../engine/d3d_renderer11.h"

D3D11Drawable::D3D11Drawable(BaseRenderComponent* renderComponent, const DirectX::XMFLOAT4X4* pMatrix) : SceneNode(renderComponent, RenderPass::RenderPass_Actor, pMatrix) {}

D3D11Drawable::D3D11Drawable(BaseRenderComponent* renderComponent, DirectX::FXMMATRIX to, DirectX::CXMMATRIX from, bool calulate_from) : SceneNode(renderComponent, RenderPass::RenderPass_Actor, to, from, calulate_from) {}

D3D11Drawable::~D3D11Drawable() {}

void D3D11Drawable::AddBind(BindableType key, std::unique_ptr<Bindable> bind) {
	m_binds[key] = std::move(bind);
}

HRESULT D3D11Drawable::VRender(Scene* pScene) {
	D3DRenderer11* renderer = static_cast<D3DRenderer11*>(g_pApp->GetRenderer());
	ID3D11DeviceContext* deviceContext = renderer->GetDeviceContext();

	for (auto const& [key, val] : m_binds) {
		val->Bind(deviceContext);
	}
	size_t index_count = static_cast<const IndexBufferBindable*>(m_binds.at(BindableType::index_buffer).get())->GetCount();
	deviceContext->DrawIndexed(index_count, 0u, 0u);

	return S_OK;
}
