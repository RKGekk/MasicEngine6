#include <Windows.h>
#include <utility>
#include <tuple>
#include <string>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib,"DXGI.lib")
#pragma comment(lib,"D3DCompiler.lib")
#pragma comment(lib,"assimp-vc142-mtd.lib")

#include "tools/error_logger.h"
#include "engine/render_window.h"
#include "engine/engine.h"

using namespace std::literals;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	Engine engine;
	bool can_run = engine.Initialize(
		RenderWindowConfig{EngineOptions("EngineOptions.xml"s)}
			.set_hInstance(hInstance)
			.set_window_title("Title")
			.set_window_class("MyTestWindowsClass")
	);
	if (can_run) {
		engine.Run();
	}
	
	return 0;
}