#pragma once

#include <Windows.h>
#include <utility>
#include <memory>
#include <string>

#include "../tools/string_utility.h"
#include "../tools/error_logger.h"
#include "../engine_options.h"

using namespace std::literals;

class Engine;

class RenderWindowConfig {
public:
	RenderWindowConfig() = default;
	RenderWindowConfig(const EngineOptions& opt) {
		width = opt.m_screenWidth;
		height = opt.m_screenHeight;
		is_windowed_mode = !opt.m_fullScreen;
		options = opt;
	}

	RenderWindowConfig& set_hInstance(HINSTANCE hInstance);
	RenderWindowConfig& set_window_title(std::string title);
	RenderWindowConfig& set_window_class(std::string class_name);
	RenderWindowConfig& set_width(int width);
	RenderWindowConfig& set_height(int height);
	RenderWindowConfig& set_window_mode(bool mode);

	HINSTANCE hInstance;

	std::string window_title;
	std::wstring window_title_w;

	std::string window_class;
	std::wstring window_class_w;

	int width;
	int height;

	bool is_windowed_mode;

	EngineOptions options;
};

class RenderWindow {
public:
	RenderWindow() = default;
	RenderWindow(const RenderWindow&) = delete;
	RenderWindow& operator=(const RenderWindow& right) = delete;

	bool Initialize(Engine& windowContainer, const RenderWindowConfig& cfg);
	bool ProcessMessages();
	~RenderWindow();

	HWND GetHWND() const;
	HINSTANCE GetInstance() const;
	int GetWidth() const;
	int GetHeight() const;

	const RenderWindowConfig& GetConfig() const;

private:
	void RegisterWindowClass();

	HWND m_handle = NULL;
	RenderWindowConfig m_window_config;
};