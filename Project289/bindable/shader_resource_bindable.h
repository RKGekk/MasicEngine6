#pragma once

#include <d3d11.h>
#include <wrl.h>

#include "bindable.h"
#include "../tools/memory_utility.h"
#include "../tools/com_exception.h"

class ShaderResourceBindable : public Bindable {
public:
	ShaderResourceBindable(ID3D11Device* device, ID3D11ShaderResourceView* srv);
	ShaderResourceBindable(ID3D11Device* device, ID3D11ShaderResourceView* srv, unsigned int startSlot);
	void Bind(ID3D11DeviceContext* deviceContext) override;
protected:
	ID3D11ShaderResourceView* srv;
	unsigned int m_startSlot;
};