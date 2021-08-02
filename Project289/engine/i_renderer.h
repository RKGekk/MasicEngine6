#pragma once

#include <memory>

#include <Windows.h>
#include <DirectXMath.h>

#include "i_render_state.h"
#include "../nodes/light_node.h"
#include "render_window.h"

class IRenderer {
public:
	virtual bool Initialize(const RenderWindow& rw) = 0;

	virtual void VSetBackgroundColor(BYTE bgA, BYTE bgR, BYTE bgG, BYTE bgB) = 0;
	virtual void VSetBackgroundColor4f(float bgA, float bgR, float bgG, float bgB) = 0;
	virtual HRESULT VOnRestore() = 0;
	virtual void VShutdown() = 0;
	virtual bool VPreRender() = 0;
	virtual bool VPresent() = 0;
	virtual bool VPostRender() = 0;
	virtual void VCalcLighting(Lights* lights, int maximumLights) = 0;
	virtual void VSetWorldTransform(DirectX::FXMMATRIX m) = 0;
	virtual void VSetWorldTransform4x4(const DirectX::XMFLOAT4X4& m) = 0;
	virtual void VSetViewTransform(DirectX::FXMMATRIX m) = 0;
	virtual void VSetViewTransform4x4(const DirectX::XMFLOAT4X4& m) = 0;
	virtual void VSetProjectionTransform(DirectX::FXMMATRIX m) = 0;
	virtual void VSetProjectionTransform4x4(const DirectX::XMFLOAT4X4& m) = 0;
	virtual std::shared_ptr<IRenderState> VPrepareAlphaPass() = 0;
	virtual std::shared_ptr<IRenderState> VPrepareSkyBoxPass() = 0;
};