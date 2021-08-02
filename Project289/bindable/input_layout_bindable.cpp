#include "input_layout_bindable.h"

InputLayoutBindable::InputLayoutBindable(ID3D11Device* device, ID3DBlob* pVertexShaderBytecode) {

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied(5);
	ied[0].SemanticName = "POSITION";
	ied[0].SemanticIndex = 0;
	ied[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	ied[0].InputSlot = 0;
	ied[0].AlignedByteOffset = 0;
	ied[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	ied[0].InstanceDataStepRate = 0;

	ied[1].SemanticName = "NORMAL";
	ied[1].SemanticIndex = 0;
	ied[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	ied[1].InputSlot = 0;
	ied[1].AlignedByteOffset = 16;
	ied[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	ied[1].InstanceDataStepRate = 0;

	ied[2].SemanticName = "TEXCOORD";
	ied[2].SemanticIndex = 0;
	ied[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	ied[2].InputSlot = 0;
	ied[2].AlignedByteOffset = 28;
	ied[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	ied[2].InstanceDataStepRate = 0;

	ied[3].SemanticName = "COLOR";
	ied[3].SemanticIndex = 0;
	ied[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	ied[3].InputSlot = 0;
	ied[3].AlignedByteOffset = 36;
	ied[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	ied[3].InstanceDataStepRate = 0;

	ied[4].SemanticName = "TANGENT";
	ied[4].SemanticIndex = 0;
	ied[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	ied[4].InputSlot = 0;
	ied[4].AlignedByteOffset = 52;
	ied[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	ied[4].InstanceDataStepRate = 0;

	HRESULT hr = device->CreateInputLayout(ied.data(), (UINT)ied.size(), pVertexShaderBytecode->GetBufferPointer(), pVertexShaderBytecode->GetBufferSize(), &pInputLayout);
	COM_ERROR_IF_FAILED(hr, "Failed to create input layout");
}

InputLayoutBindable::InputLayoutBindable(ID3D11Device* device, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode) {
	HRESULT hr = device->CreateInputLayout(layout.data(), (UINT)layout.size(), pVertexShaderBytecode->GetBufferPointer(), pVertexShaderBytecode->GetBufferSize(), &pInputLayout);
	COM_ERROR_IF_FAILED(hr, "Failed to create input layout");
}

void InputLayoutBindable::Bind(ID3D11DeviceContext* deviceContext) noexcept {
	deviceContext->IASetInputLayout(pInputLayout.Get());
}