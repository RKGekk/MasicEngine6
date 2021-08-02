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

class D3D11BasicPipeline : public SceneNode {
private:
	std::shared_ptr<VertexShader> m_vs;
	std::shared_ptr<PixelShader> m_ps;

public:
	D3D11BasicPipeline(MeshHolder& mesh_data, std::shared_ptr<PixelShader> ps, std::shared_ptr<VertexShader> vs, const DirectX::XMFLOAT4X4* pMatrix);
	D3D11BasicPipeline(MeshHolder& mesh_data, std::shared_ptr<PixelShader> ps, std::shared_ptr<VertexShader> vs, DirectX::FXMMATRIX to, DirectX::CXMMATRIX from, bool calulate_from = false);
	virtual ~D3D11BasicPipeline();

	virtual HRESULT VRender(Scene* pScene) override;
	virtual HRESULT VOnUpdate(Scene* pScene, float const elapsedMs) override;
	virtual HRESULT VPreRender(Scene* pScene) override;
	virtual HRESULT VPostRender(Scene* pScene) override;

protected:
	UINT m_vertex_stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
	UINT m_index_count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
	ID3DBlob* m_pVSBytecodeBlob;
	ID3D11VertexShader* m_pVertexShader;
	ID3DBlob* m_pPSBytecodeBlob;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11ShaderResourceView* m_srv0;
	unsigned int m_srv_start_slot;
	ID3D11InputLayout* m_pInputLayout;
	D3D11_PRIMITIVE_TOPOLOGY m_primitive_topology_type;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampleState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	m_depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterState;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState;
	unsigned int m_vs_cb_slot;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVSConstantBuffer;
	unsigned int m_ps_cb_slot;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pPSConstantBuffer;
};