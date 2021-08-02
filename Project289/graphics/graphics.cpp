#include "graphics.h"

bool Graphics::Initialize(HWND hwnd, int width, int height) {
	m_fps_timer.Start();
	m_width = width;
	m_height = height;
	if (!InitializeDirectX(hwnd)) { return false; };
	if(!InitializeShaders()) { return false; };
	if (!InitializeScene()) { return false; };

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(m_device.Get(), m_device_context.Get());
	ImGui::StyleColorsDark();

	return true;
}

void Graphics::RenderFrame() {
	m_fps_timer.Tick();

	std::array<float, 4> color = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_device_context->ClearRenderTargetView(m_render_target_view.Get(), color.data());
	m_device_context->ClearDepthStencilView(m_depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_device_context->IASetInputLayout(m_vertex_shader.GetInputLayout());
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->RSSetState(m_raster_state.Get());
	m_device_context->OMSetBlendState(m_blend_state.Get(), nullptr, 0xFFFFFFFF);
	m_device_context->OMSetDepthStencilState(m_depth_stencil_state.Get(), 0);
	m_device_context->PSSetSamplers(0, 1, m_sampler_state.GetAddressOf());
	m_device_context->VSSetShader(m_vertex_shader.GetShader(), NULL, 0);
	m_device_context->PSSetShader(m_pixel_shader.GetShader(), NULL, 0);

	//m_cbps_light.data.dColor = m_light.m_light_color;
	//m_cbps_light.data.dLightStrength = m_light.m_light_strenght;
	//m_cbps_light.data.dPos = m_light.GetPosition3();
	//m_cbps_light.data.dAtt_a = m_light.m_light_attenuation_a;
	//m_cbps_light.data.dAtt_b = m_light.m_light_attenuation_b;
	//m_cbps_light.data.dAtt_c = m_light.m_light_attenuation_c;
	if (!m_cbps_light.ApplyChanges(m_device_context.Get())) { return; }
	m_device_context->PSSetConstantBuffers(0u, 1u, m_cbps_light.GetAddressOf());

	//m_game_object.Draw(m_device_context.Get(), DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(0.001f, 0.001f, 0.001f), DirectX::XMMatrixMultiply(m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix())));
	m_game_object.Draw(m_device_context.Get(), DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f), DirectX::XMMatrixMultiply(m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix())));
	//m_game_object.Draw(m_device_context.Get(), DirectX::XMMatrixMultiply(m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix()));

	m_light.Draw(m_device_context.Get(), DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f), DirectX::XMMatrixMultiply(m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix())));

	static int fpsCounter = 0;
	static std::wstring fpsString = L"FPS: 0";
	fpsCounter += 1;
	if (m_fps_timer.TotalTime() > 1.0f) {
		fpsString = L"FPS: " + std::to_wstring(fpsCounter);
		fpsCounter = 0;
		m_fps_timer.Reset();
	}
	m_sprite_batch->Begin();
	DirectX::XMVECTOR text_pos = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR text_color = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMVECTOR text_origin = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR text_scale = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	m_sprite_font->DrawString(m_sprite_batch.get(), fpsString.c_str(), text_pos, DirectX::Colors::White, 0.0f, text_origin, text_scale);
	m_sprite_batch->End();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Light Controls");

	//ImGui::DragFloat3("Ambient Light Color", &m_cbps_light.data.color.x, 0.01f, 0.0f, 1.0f);
	//ImGui::DragFloat("Ambient Light Strenght", &m_cbps_light.data.ambientLightStrength, 0.01f, 0.0f, 10.0f);
	//ImGui::DragFloat3("Translation x/y/z", translationOffset.data(), 0.1f, -5.0f, 5.0f);
	//ImGui::DragFloat("Alpha", &alpha, 0.005f, 0.0f, 1.0f);
	ImGui::DragFloat3("Diffuse Light Color", &m_light.m_light_color.x, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Diffuse Light Strenght", &m_light.m_light_strenght, 0.1f, 0.0f, 10.0f);
	ImGui::DragFloat("Diffuse Light Attenuation A", &m_light.m_light_attenuation_a, 0.01f, 0.01f, 10.0f);
	ImGui::DragFloat("Diffuse Light Attenuation B", &m_light.m_light_attenuation_b, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("Diffuse Light Attenuation C", &m_light.m_light_attenuation_c, 0.01f, 0.0f, 10.0f);

	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	m_swap_chain->Present(1, NULL);
}

bool Graphics::InitializeDirectX(HWND hwnd) {
	try {
		std::vector<AdapterData>& adapters = AdapterReader::GetAdapterData();
		if (adapters.size() == 0u) {
			ErrorLogger::Log("No IDXGI Adapters found.");
			return false;
		}
		Microsoft::WRL::ComPtr<IDXGIAdapter> adapter = adapters[0].m_pAdapter;

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		swapChainDesc.BufferDesc.Width = m_width;
		swapChainDesc.BufferDesc.Height = m_height;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			adapter.Get(),
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL,
			NULL,
			NULL,
			0,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			m_swap_chain.GetAddressOf(),
			m_device.GetAddressOf(),
			NULL,
			m_device_context.GetAddressOf()
		);
		COM_ERROR_IF_FAILED(hr, "Failed to create device and swapchain.");

		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBufferPtr;
		hr = m_swap_chain->GetBuffer(0, IID_PPV_ARGS(backBufferPtr.GetAddressOf()));
		COM_ERROR_IF_FAILED(hr, "Failed to get back buffer.");

		hr = m_device->CreateRenderTargetView(backBufferPtr.Get(), NULL, m_render_target_view.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create render target view.");

		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
		depthBufferDesc.Width = m_width;
		depthBufferDesc.Height = m_height;
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

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		hr = m_device->CreateDepthStencilView(m_depth_stencil_buffer.Get(), &depthStencilViewDesc, m_depth_stencil_view.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil view.");

		m_device_context->OMSetRenderTargets(1, m_render_target_view.GetAddressOf(), m_depth_stencil_view.Get());

		D3D11_VIEWPORT viewport;
		viewport.Width = (float)(m_width);
		viewport.Height = (float)(m_height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		m_device_context->RSSetViewports(1, &viewport);

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

		
		/*
		CD3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		D3D11_RENDER_TARGET_BLEND_DESC rtbd;
		ZeroMemory(&rtbd, sizeof(rtbd));
		rtbd.BlendEnable = true;
		rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
		blendDesc.RenderTarget[0] = rtbd;*/
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

		m_sprite_batch = std::make_unique<DirectX::SpriteBatch>(m_device_context.Get());
		m_sprite_font = std::make_unique<DirectX::SpriteFont>(m_device.Get(), L"data/fonts/comic_sans_ms_16.sprite");
	}
	catch (COMException e) {
		ErrorLogger::Log(e);
		return false;
	}

	return true;
}

bool Graphics::InitializeShaders() {
	std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	bool can_draw = m_vertex_shader.Initialize(
		m_device.Get(),
		ShaderGonfig{}
			.set_shader_name(L"vertex_shader.cso")
			.set_description(layout)
	);

	can_draw &= m_pixel_shader.Initialize(
		m_device.Get(),
		ShaderGonfig{}
			.set_shader_name(L"pixel_shader.cso")
			.set_description({})
	);

	if (!can_draw) {
		return false;
	}

	return true;
}

bool Graphics::InitializeScene() {
	try {
		HRESULT hr = CoInitialize(NULL);
		COM_ERROR_IF_FAILED(hr, "Failed initialize WIC.");
			
		//hr = DirectX::CreateWICTextureFromFile(m_device.Get(), L"Data/Textures/storage-container2-albedo.png", nullptr, m_texture.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create WIC texture from file.");

		hr = m_cbvs.Initialize(m_device.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		//m_cbps_light.data.color = {1.0f, 1.0f, 1.0f};
		//m_cbps_light.data.ambientLightStrength = 1.0f;
		hr = m_cbps_light.Initialize(m_device.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		if (!m_game_object.Initialize(m_device.Get(), "data/objects/nanosuit/nanosuit.obj", m_cbvs)) { return false; };
		//if (!m_game_object.Initialize(m_device.Get(), "Data/Samples/dodge_challenger.fbx", m_cbvs)) { return false; };

		if (!m_light.Initialize(m_device.Get(), m_cbvs)) { return false; };
		m_light.SetPosition(0.0f, 15.0f, -3.0f);

		m_camera.SetPosition(0.0f, 0.0f, -2.0f);
		m_camera.SetProjectionValues(60.0f, float(m_width) / float(m_height), 0.1f, 1000.0f);
	}
	catch (COMException e) {
		ErrorLogger::Log(e);
		return false;
	}

	return true;
}
