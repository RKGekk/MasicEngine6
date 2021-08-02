#include "vertex_shader_bindable.h"

VertexShaderBindable::VertexShaderBindable(ID3D11Device* device, ID3DBlob* pBytecodeBlob, ID3D11VertexShader* pVertexShader) {
	m_pBytecodeBlob = pBytecodeBlob;
	m_pVertexShader = pVertexShader;
}

void VertexShaderBindable::Bind(ID3D11DeviceContext* deviceContext) {
	deviceContext->VSSetShader(m_pVertexShader, nullptr, 0u);
}

ID3DBlob* VertexShaderBindable::GetBytecode() const {
	return m_pBytecodeBlob;
}