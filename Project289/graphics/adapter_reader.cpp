#include "adapter_reader.h"

std::vector<AdapterData> AdapterReader::m_adapters;

std::vector<AdapterData>& AdapterReader::GetAdapterData() {
	if (m_adapters.size() > 0) { return m_adapters; }

	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(pFactory.GetAddressOf()));
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to create DXGIFactory for enumerating adapters.");
		exit(-1);
	}

	Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter;
	UINT index = 0;
	while (pFactory->EnumAdapters(index++, pAdapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
		m_adapters.push_back({ pAdapter });
	}

	return m_adapters;
}

AdapterData::AdapterData(Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter) {
	m_pAdapter = pAdapter;
	HRESULT hr = m_pAdapter->GetDesc(&m_description);
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to get description for IDXGIAdapter.");
	}
	UINT index = 0;
	Microsoft::WRL::ComPtr<IDXGIOutput> pOutput;
	while (m_pAdapter->EnumOutputs(index++, pOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
		m_outputs.push_back({ pOutput });
	}
}

OutputData::OutputData(Microsoft::WRL::ComPtr<IDXGIOutput> pOutput) {
	m_pOutput = pOutput;
	HRESULT hr = pOutput->GetDesc(&m_description);
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to get description for IDXGIOutput.");
	}
	unsigned int numModes = 0;
	hr = m_pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to get number of modes for IDXGIOutput.");
	}
	m_modes.resize(numModes);
	hr = m_pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, m_modes.data());
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to get modes for IDXGIOutput.");
	}
}
