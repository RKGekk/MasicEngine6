#include "topology_bindable.h"

TopologyBindable::TopologyBindable(ID3D11Device* device, D3D11_PRIMITIVE_TOPOLOGY type) : type(type) {}

void TopologyBindable::Bind(ID3D11DeviceContext* deviceContext) {
	deviceContext->IASetPrimitiveTopology(type);
}