#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <string>
#include <fstream>

#include "bindable.h"
#include "../tools/memory_utility.h"
#include "../tools/com_exception.h"

class PixelShaderBindable : public Bindable {
public:
	PixelShaderBindable(ID3D11Device* device, ID3DBlob* pBytecodeBlob, ID3D11PixelShader* pPixelShader);
	void Bind(ID3D11DeviceContext* deviceContext) override;
	ID3DBlob* GetBytecode() const;

protected:
	ID3DBlob* m_pBytecodeBlob;
	ID3D11PixelShader* m_pPixelShader;
};