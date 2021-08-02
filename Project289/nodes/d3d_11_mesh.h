#pragma once

#include <d3d11.h>
#include <wrl.h>

#include "d3d_11_drawable.h"
#include "../graphics/material_texture.h"
#include "../graphics/shader.h"

class D3D11Mesh : public D3D11Drawable {
	VertexShader m_vs;
	PixelShader m_ps;
	int m_mesh_id;

public:
	D3D11Mesh(int mesh_id, BaseRenderComponent* renderComponent, const DirectX::XMFLOAT4X4* pMatrix);
	D3D11Mesh(int mesh_id, BaseRenderComponent* renderComponent, DirectX::FXMMATRIX to, DirectX::CXMMATRIX from, bool calulate_from = false);
	virtual ~D3D11Mesh();
	virtual HRESULT VOnUpdate(Scene* pScene, float const elapsedMs) override;
	virtual HRESULT VPreRender(Scene* pScene) override;
};