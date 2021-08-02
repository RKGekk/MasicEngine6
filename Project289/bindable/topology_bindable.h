#pragma once

#include <d3d11.h>
#include <wrl.h>

#include "bindable.h"
#include "../tools/memory_utility.h"

class TopologyBindable : public Bindable {
public:
	TopologyBindable(ID3D11Device* device, D3D11_PRIMITIVE_TOPOLOGY type);
	void Bind(ID3D11DeviceContext* deviceContext) override;
protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};