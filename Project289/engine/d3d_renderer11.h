#pragma once

#include <memory>
#include <array>
#include <stack>
#include <utility>

#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

#include "i_renderer.h"
#include "../tools/com_exception.h"
#include "../graphics/adapter_reader.h"

#include "../graphics/imgui/imgui.h"
#include "../graphics/imgui/imgui_impl_win32.h"
#include "../graphics/imgui/imgui_impl_dx11.h"

class D3DRenderer11 : public IRenderer {
public:
	D3DRenderer11();

	virtual bool Initialize(const RenderWindow& rw) override;

	virtual void VSetBackgroundColor(BYTE R, BYTE G, BYTE B, BYTE A) override;
	virtual void VSetBackgroundColor4f(float R, float G, float B, float A) override;
	virtual bool VPreRender() override;
	virtual bool VPresent() override;
	virtual bool VPostRender();
	virtual void VPushRenderTarget(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view, Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view);
	virtual void VPushRenderTarget(UINT NumViews, Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view, Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view);
	virtual void VPopRenderTarget();
	virtual HRESULT VOnRestore();
	virtual void VShutdown();
	virtual void VCalcLighting(Lights* lights, int maximumLights);

	virtual void VSetWorldTransform(DirectX::FXMMATRIX m) override;
	virtual void VSetWorldTransform4x4(const DirectX::XMFLOAT4X4& m) override;
	virtual void VSetViewTransform(DirectX::FXMMATRIX m) override;
	virtual void VSetViewTransform4x4(const DirectX::XMFLOAT4X4& m) override;
	virtual void VSetProjectionTransform(DirectX::FXMMATRIX m) override;
	virtual void VSetProjectionTransform4x4(const DirectX::XMFLOAT4X4& m) override;
	virtual std::shared_ptr<IRenderState> VPrepareAlphaPass() override;
	virtual std::shared_ptr<IRenderState> VPrepareSkyBoxPass() override;

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	IDXGISwapChain* GetSwapChain();

protected:
	std::array<float, 4> m_backgroundColor;
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_device_context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swap_chain;
	D3D11_VIEWPORT m_viewport;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_back_buffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_render_target_view;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depth_stencil_buffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depth_stencil_view;

	std::stack<std::pair<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>, Microsoft::WRL::ComPtr<ID3D11DepthStencilView>>> m_render_targer_stack;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_raster_state;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blend_state;

	std::unique_ptr<DirectX::SpriteBatch> m_sprite_batch;
	std::unique_ptr<DirectX::SpriteFont> m_sprite_font;
};