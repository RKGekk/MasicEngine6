#pragma once

#include <vector>
#include <d3d11.h>
#include <wrl.h>

#include "bindable.h"
#include "../tools/memory_utility.h"
#include "../tools/com_exception.h"

class IndexBufferBindable : public Bindable {
public:
	IndexBufferBindable(ID3D11Device* device, const std::vector<DWORD>& indices);

	void Bind(ID3D11DeviceContext* deviceContext) override;
	UINT GetCount() const;

protected:
	UINT count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
};