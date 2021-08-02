#pragma once

#include <windows.h>
#include <wrl.h>
#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <array>

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>

#include "adapter_reader.h"
#include "shader.h"
#include "vertex.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "constant_buffer.h"
#include "constant_buffer_types.h"
#include "model.h"
#include "game_object.h"
#include "camera.h"
#include "../tools/game_timer.h"
#include "../tools/error_logger.h"
#include "../tools/com_exception.h"
#include "renderable_game_object.h"
#include "light.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_impl_dx11.h"

class Graphics {
public:
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();

	Camera m_camera;
	RenderableGameObject m_game_object;
	Light m_light;

private:
	bool InitializeDirectX(HWND hwnd);
	bool InitializeShaders();
	bool InitializeScene();

	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_device_context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swap_chain;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_render_target_view;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depth_stencil_buffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depth_stencil_view;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_raster_state;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blend_state;

	VertexShader m_vertex_shader;
	PixelShader m_pixel_shader;

	ConstantBuffer<CB_VS_VertexShader> m_cbvs;
	ConstantBuffer<CB_PS_PixelShader_Light> m_cbps_light;

	std::unique_ptr<DirectX::SpriteBatch> m_sprite_batch;
	std::unique_ptr<DirectX::SpriteFont> m_sprite_font;

	int m_width;
	int m_height;

	GameTimer m_fps_timer;
};