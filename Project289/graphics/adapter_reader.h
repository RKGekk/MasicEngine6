#pragma once

#include <d3d11.h>
#include <Windows.h>
#include <wrl.h>
#include <vector>

#include "../tools/error_logger.h"

class OutputData {
public:
	OutputData(Microsoft::WRL::ComPtr<IDXGIOutput> pOutput);
	Microsoft::WRL::ComPtr<IDXGIOutput> m_pOutput;
	DXGI_OUTPUT_DESC m_description;
	std::vector<DXGI_MODE_DESC> m_modes;
};

class AdapterData {
public:
	AdapterData(Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter);
	Microsoft::WRL::ComPtr<IDXGIAdapter> m_pAdapter;
	DXGI_ADAPTER_DESC m_description;
	std::vector<OutputData> m_outputs;
};

class AdapterReader {
public:
	static std::vector<AdapterData>& GetAdapterData();
private:
	static std::vector<AdapterData> m_adapters;
};