#include "pixel_shader_bindable.h"

PixelShaderBindable::PixelShaderBindable(ID3D11Device* device, ID3DBlob* pBytecodeBlob, ID3D11PixelShader* pPixelShader) {
	m_pBytecodeBlob = pBytecodeBlob;
	m_pPixelShader = pPixelShader;
}

void PixelShaderBindable::Bind(ID3D11DeviceContext* deviceContext) {
	deviceContext->PSSetShader(m_pPixelShader, nullptr, 0u);
}

ID3DBlob* PixelShaderBindable::GetBytecode() const {
	return m_pBytecodeBlob;
}