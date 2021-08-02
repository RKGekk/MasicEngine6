#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "scene_node.h"
#include "../graphics/material_texture.h"
#include "../graphics/mesh_holder.h"
#include "../graphics/shader.h"

class SmokeComponent;

class D3D11GeometryPipeline : public SceneNode {
private:
	std::shared_ptr<VertexShader> m_vs;
	std::shared_ptr<GeometryShader> m_gs;
	std::shared_ptr<PixelShader> m_ps;

public:
	D3D11GeometryPipeline(SmokeComponent& data, const DirectX::XMFLOAT4X4* pMatrix);
	D3D11GeometryPipeline(SmokeComponent& data, DirectX::FXMMATRIX to, DirectX::CXMMATRIX from, bool calulate_from = false);
	virtual ~D3D11GeometryPipeline();

	virtual HRESULT VRender(Scene* pScene) override;
	virtual HRESULT VOnUpdate(Scene* pScene, float const elapsedMs) override;
	virtual HRESULT VPreRender(Scene* pScene) override;
	virtual HRESULT VPostRender(Scene* pScene) override;

protected:
	UINT m_vertex_stride;
	UINT m_vertex_count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ID3DBlob* m_pVSBytecodeBlob;
	ID3D11VertexShader* m_pVertexShader;
	ID3DBlob* m_pGSBytecodeBlob;
	ID3D11GeometryShader* m_pGeometryShader;
	ID3DBlob* m_pPSBytecodeBlob;
	ID3D11PixelShader* m_pPixelShader;
	MaterialTexture m_texture;
	ID3D11ShaderResourceView* m_srv0;
	unsigned int m_srv_start_slot;
	ID3D11InputLayout* m_pVSInputLayout;
	D3D11_PRIMITIVE_TOPOLOGY m_primitive_topology_type;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampleState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	m_depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterState;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState;
	unsigned int m_vs_cb_slot1;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVSConstantBuffer1;
	unsigned int m_gs_cb_slot1;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pGSConstantBuffer1;
	unsigned int m_gs_cb_slot2;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pGSConstantBuffer2;
	unsigned int m_ps_cb_slot;
	float m_atlas_width;
	float m_atlas_height;
};