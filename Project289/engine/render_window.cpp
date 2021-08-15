#include "render_window.h"
#include "engine.h"

bool RenderWindow::Initialize(Engine& windowContainer, const RenderWindowConfig& cfg) {
	m_window_config = cfg;

	static bool raw_input_initialized = false;
	if (!raw_input_initialized) {
		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x01;
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = 0;
		if (!RegisterRawInputDevices(&rid, 1, sizeof(rid))) {
			ErrorLogger::Log(GetLastError(), "Failed to register raw input device.");
			exit(-1);
		}
		raw_input_initialized = true;
	}

	this->RegisterWindowClass();

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int posX = 0;
	int posY = 0;

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (!cfg.is_windowed_mode) {

		m_window_config.height = screenHeight;
		m_window_config.width = screenWidth;

		// If full screen set the screen to maximum size of the users desktop and 32bit.
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;

		// Create the window with the screen settings and get the handle to it.
		m_handle = CreateWindowEx(
			WS_EX_APPWINDOW,
			s2w(m_window_config.window_class).c_str(),
			s2w(m_window_config.window_title).c_str(),
			//WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, // окно без рамки
			WS_OVERLAPPEDWINDOW,
			posX,
			posY,
			screenWidth,
			screenHeight,
			NULL,
			NULL,
			m_window_config.hInstance,
			&windowContainer
		);
	}
	else {
		// If windowed then set it to 800x600 resolution.
		screenWidth = cfg.width;
		screenHeight = cfg.height;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

		DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
		RECT wr;
		wr.left = 0;
		wr.top = 0;
		wr.right = wr.left + cfg.width;
		wr.bottom = wr.top + cfg.height;
		AdjustWindowRect(&wr, style, FALSE);

		m_handle = CreateWindowEx(
			0,
			s2w(m_window_config.window_class).c_str(),
			s2w(m_window_config.window_title).c_str(),
			style,
			posX,
			posY,
			wr.right - wr.left,
			wr.bottom - wr.top,
			NULL,
			NULL,
			m_window_config.hInstance,
			&windowContainer
		);
	}

	if (m_handle == NULL) {
		ErrorLogger::Log(GetLastError(), L"CreateWindowEx Failed for window: "s + s2w(m_window_config.window_title));
		return false;
	}

	ShowWindow(m_handle, SW_SHOW);
	SetForegroundWindow(m_handle);
	SetFocus(m_handle);

	return true;
}

bool RenderWindow::ProcessMessages() {
	MSG msg;

	ZeroMemory(&msg, sizeof(MSG));

	while (PeekMessage(&msg, m_handle, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_NULL) {
		if (!IsWindow(m_handle)) {
			m_handle = NULL;
			UnregisterClass(s2w(m_window_config.window_class).c_str(), m_window_config.hInstance);
			return false;
		}
	}

	return true;
}

RenderWindow::~RenderWindow() {
	if (m_handle != NULL) {
		UnregisterClass(s2w(m_window_config.window_class).c_str(), m_window_config.hInstance);
		DestroyWindow(m_handle);
	}
}

HWND RenderWindow::GetHWND() const {
	return m_handle;
}

HINSTANCE RenderWindow::GetInstance() const {
	return m_window_config.hInstance;
}

int RenderWindow::GetWidth() const {
	return m_window_config.width;
}

int RenderWindow::GetHeight() const {
	return m_window_config.height;
}

const RenderWindowConfig& RenderWindow::GetConfig() const {
	return m_window_config;
}

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_CLOSE: {
			DestroyWindow(hwnd);
			return 0;
		}
		default: {
			Engine* const pWindow = reinterpret_cast<Engine*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_NCCREATE: {
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Engine* pWindow = reinterpret_cast<Engine*>(pCreate->lpCreateParams);
			if (!pWindow) {
				ErrorLogger::Log("Error: pointer to window container is null during WM_NCCREATE.");
				exit(-1);
			}
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));

			return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
		}
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

void RenderWindow::RegisterWindowClass() {
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_window_config.hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_window_config.window_class_w.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);
}

RenderWindowConfig::RenderWindowConfig(const EngineOptions& opt) {
	width = opt.m_screenWidth;
	height = opt.m_screenHeight;
	is_windowed_mode = !opt.m_fullScreen;
	options = opt;
}

RenderWindowConfig& RenderWindowConfig::set_hInstance(HINSTANCE hInstance) {
	this->hInstance = hInstance;
	return *this;
}

RenderWindowConfig& RenderWindowConfig::set_window_title(std::string title) {
	this->window_title_w = s2w(title);
	this->window_title = std::move(title);
	return *this;
}

RenderWindowConfig& RenderWindowConfig::set_window_class(std::string class_name) {
	this->window_class_w = s2w(class_name);
	this->window_class = std::move(class_name);
	return *this;
}

RenderWindowConfig& RenderWindowConfig::set_width(int width) {
	this->width = width;
	return *this;
}

RenderWindowConfig& RenderWindowConfig::set_height(int height) {
	this->height = height;
	return *this;
}

RenderWindowConfig& RenderWindowConfig::set_window_mode(bool mode) {
	this->is_windowed_mode = mode;
	return *this;
}