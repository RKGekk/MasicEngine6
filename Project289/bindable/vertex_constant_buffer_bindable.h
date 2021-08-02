#pragma once

#include "bindable.h"
#include "constant_buffer_bindable.h"
#include "../tools/memory_utility.h"

template <typename T>
class VertexConstantBufferBindable : public ConstantBufferBindable<T> {
	using ConstantBufferBindable<T>::pConstantBuffer;

private:
	unsigned int m_slot;

public:

	VertexConstantBufferBindable(ID3D11Device* device, const T& consts) : ConstantBufferBindable<T>::ConstantBufferBindable(device, consts) {
		m_slot = 0;
	}

	VertexConstantBufferBindable(ID3D11Device* device, const T& consts, unsigned int slot) : ConstantBufferBindable<T>::ConstantBufferBindable(device, consts) {
		m_slot = slot;
	}

	void Bind(ID3D11DeviceContext* deviceContext) override {
		deviceContext->VSSetConstantBuffers(m_slot, 1u, pConstantBuffer.GetAddressOf());
	}
};