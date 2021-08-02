#pragma once

#include <d3d11.h>
#include <wrl.h>

#include "bindable.h"
#include "constant_buffer_bindable.h"
#include "../tools/memory_utility.h"
#include "../tools/com_exception.h"

template <typename T>
class PixelConstantBufferBindable : public ConstantBufferBindable<T> {
	using ConstantBufferBindable<T>::pConstantBuffer;

private:
	unsigned int m_slot;

public:

	PixelConstantBufferBindable(ID3D11Device* device, const T& consts) : ConstantBufferBindable<T>::ConstantBufferBindable(device, consts) {
		m_slot = 0;
	};

	PixelConstantBufferBindable(ID3D11Device* device, const T& consts, unsigned int slot) : ConstantBufferBindable<T>::ConstantBufferBindable(device, consts) {
		m_slot = slot;
	};

	void Bind(ID3D11DeviceContext* deviceContext) override {
		deviceContext->PSSetConstantBuffers(m_slot, 1u, pConstantBuffer.GetAddressOf());
	}
};