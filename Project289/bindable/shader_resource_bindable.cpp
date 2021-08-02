#include "shader_resource_bindable.h"

ShaderResourceBindable::ShaderResourceBindable(ID3D11Device* device, ID3D11ShaderResourceView* srv) : srv(srv), m_startSlot(0) {}

ShaderResourceBindable::ShaderResourceBindable(ID3D11Device* device, ID3D11ShaderResourceView* srv, unsigned int startSlot) : srv(srv), m_startSlot(startSlot) {}

void ShaderResourceBindable::Bind(ID3D11DeviceContext* deviceContext) {
	deviceContext->PSSetShaderResources(m_startSlot, 1, &srv);
}