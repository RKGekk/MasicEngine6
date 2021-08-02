#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <vector>

#include "Bindable.h"
#include "../tools/memory_utility.h"
#include "../tools/com_exception.h"

class InputLayoutBindable : public Bindable {
public:
	InputLayoutBindable(ID3D11Device* device, ID3DBlob* pVertexShaderBytecode);
	InputLayoutBindable(ID3D11Device* device, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode);
	void Bind(ID3D11DeviceContext* deviceContext) noexcept override;

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};