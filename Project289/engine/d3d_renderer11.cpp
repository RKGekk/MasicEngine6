#include "d3d_renderer11.h"

D3DRenderer11::D3DRenderer11() {}

bool D3DRenderer11::Initialize(const RenderWindow& rw) {
	if (!DirectX::XMVerifyCPUSupport()) { return false;	}

	UINT createDeviceFlags = 0;
//#if defined(DEBUG) || defined(_DEBUG)  
//	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif

	m_vsync_enabled = !rw.GetConfig().options.m_runFullSpeed;

	std::vector<AdapterData>& adapters = AdapterReader::GetAdapterData();

	if (adapters.size() == 0u) {
		ErrorLogger::Log("No IDXGI Adapters found.");
		return false;
	}

	const AdapterData& adapter = adapters[0];
	m_videoCardMemory = (int)(adapter.m_description.DedicatedVideoMemory / 1024 / 1024);
	unsigned long long stringLength;
	int error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapter.m_description.Description, 128);
	if (error != 0) { return false;	}

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = rw.GetWidth();
	swapChainDesc.BufferDesc.Height = rw.GetHeight();
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = rw.GetHWND();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	if (rw.GetConfig().is_windowed_mode) {
		swapChainDesc.Windowed = true;
	}
	else {
		swapChainDesc.Windowed = false;
	}
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		createDeviceFlags,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		m_swap_chain.GetAddressOf(),
		m_device.GetAddressOf(),
		NULL,
		m_device_context.GetAddressOf()
	);
	COM_ERROR_IF_FAILED(hr, "Failed to create the swap chain, Direct3D device, and Direct3D device context.");

	if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	hr = m_swap_chain->GetBuffer(0, IID_PPV_ARGS(m_back_buffer.GetAddressOf()));
	COM_ERROR_IF_FAILED(hr, "Failed to get back buffer.");

	hr = m_device->CreateRenderTargetView(m_back_buffer.Get(), NULL, m_render_target_view.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create render target view.");

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = rw.GetWidth();
	depthBufferDesc.Height = rw.GetHeight();
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	hr = m_device->CreateTexture2D(&depthBufferDesc, NULL, m_depth_stencil_buffer.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil buffer.");

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = m_device->CreateDepthStencilView(m_depth_stencil_buffer.Get(), &depthStencilViewDesc, m_depth_stencil_view.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil view.");

	//m_render_targer_stack.push({ m_render_target_view, m_depth_stencil_view });
	//m_device_context->OMSetRenderTargets(1, m_render_target_view.GetAddressOf(), m_depth_stencil_view.Get());

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	hr = m_device->CreateDepthStencilState(&depthStencilDesc, m_depth_stencil_state.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state.");

	m_device_context->OMSetDepthStencilState(m_depth_stencil_state.Get(), 1);

	m_viewport.Width = (float)(rw.GetWidth());
	m_viewport.Height = (float)(rw.GetHeight());
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	m_device_context->RSSetViewports(1, &m_viewport);

	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	hr = m_device->CreateRasterizerState(&rasterDesc, m_raster_state.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

	m_device_context->RSSetState(m_raster_state.Get());

	D3D11_BLEND_DESC blendDesc = CD3D11_BLEND_DESC(CD3D11_DEFAULT());
	hr = m_device->CreateBlendState(&blendDesc, m_blend_state.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create blend state.");

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = m_device->CreateSamplerState(&samplerDesc, m_sampler_state.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");

	hr = CoInitialize(NULL);
	COM_ERROR_IF_FAILED(hr, "Failed to CoInitialize.");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(rw.GetHWND());
	ImGui_ImplDX11_Init(m_device.Get(), m_device_context.Get());
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();
	io.WantCaptureMouse = true;
	io.WantCaptureKeyboard = true;

	m_sprite_batch = std::make_unique<DirectX::SpriteBatch>(m_device_context.Get());
	m_sprite_font = std::make_unique<DirectX::SpriteFont>(m_device.Get(), L"data/fonts/comic_sans_ms_16.sprite");

	return true;
}

void D3DRenderer11::VSetBackgroundColor(BYTE R, BYTE G, BYTE B, BYTE A) {
	m_backgroundColor[0] = float(R) / 255.0f;
	m_backgroundColor[1] = float(G) / 255.0f;
	m_backgroundColor[2] = float(B) / 255.0f;
	m_backgroundColor[3] = float(A) / 255.0f;
}

void D3DRenderer11::VSetBackgroundColor4f(float R, float G, float B, float A) {
	m_backgroundColor[0] = R;
	m_backgroundColor[1] = G;
	m_backgroundColor[2] = B;
	m_backgroundColor[3] = A;
}

bool D3DRenderer11::VPreRender() {
	VPushRenderTarget(m_render_target_view, m_depth_stencil_view);

	m_device_context->ClearRenderTargetView(m_render_target_view.Get(), m_backgroundColor.data());
	m_device_context->ClearDepthStencilView(m_depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	return true;
}

bool D3DRenderer11::VPresent() {
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	m_swap_chain->Present(1, NULL);
	return true;
}

bool D3DRenderer11::VPostRender() {
	VPopRenderTarget();
	return true;
}

void D3DRenderer11::VPushRenderTarget(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view, Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view) {
	m_render_targer_stack.push({ render_target_view, depth_stencil_view });
	m_device_context->OMSetRenderTargets(1, render_target_view.GetAddressOf(), depth_stencil_view.Get());
}

void D3DRenderer11::VPushRenderTarget(UINT NumViews, Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view, Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view) {
	m_render_targer_stack.push({ render_target_view, depth_stencil_view });
	m_device_context->OMSetRenderTargets(NumViews, render_target_view.GetAddressOf(), depth_stencil_view.Get());
}

void D3DRenderer11::VPopRenderTarget() {
	m_render_targer_stack.pop();
	if (m_render_targer_stack.size()) {
		std::pair<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>, Microsoft::WRL::ComPtr<ID3D11DepthStencilView>>& prev_render_target = m_render_targer_stack.top();
		m_device_context->OMSetRenderTargets(1, prev_render_target.first.GetAddressOf(), prev_render_target.second.Get());
		m_device_context->RSSetViewports(1, &m_viewport);
	}
}

HRESULT D3DRenderer11::VOnRestore() {
	return S_OK;
}

void D3DRenderer11::VShutdown() {}

void D3DRenderer11::VCalcLighting(Lights* lights, int maximumLights) {}

ID3D11Device* D3DRenderer11::GetDevice() {
	return m_device.Get();
}

ID3D11DeviceContext* D3DRenderer11::GetDeviceContext() {
	return m_device_context.Get();
}

IDXGISwapChain* D3DRenderer11::GetSwapChain() {
	return m_swap_chain.Get();
}

void D3DRenderer11::VSetWorldTransform(DirectX::FXMMATRIX m) {}

void D3DRenderer11::VSetWorldTransform4x4(const DirectX::XMFLOAT4X4& m) {}

void D3DRenderer11::VSetViewTransform(DirectX::FXMMATRIX m) {}

void D3DRenderer11::VSetViewTransform4x4(const DirectX::XMFLOAT4X4& m) {}

void D3DRenderer11::VSetProjectionTransform(DirectX::FXMMATRIX m) {}

void D3DRenderer11::VSetProjectionTransform4x4(const DirectX::XMFLOAT4X4& m) {}

std::shared_ptr<IRenderState> D3DRenderer11::VPrepareAlphaPass() {
	return std::shared_ptr<IRenderState>();
}

std::shared_ptr<IRenderState> D3DRenderer11::VPrepareSkyBoxPass() {
	return std::shared_ptr<IRenderState>();
}