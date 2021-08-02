#pragma once

#include "game_object.h"

class RenderableGameObject : public GameObject {
public:
	RenderableGameObject();
	bool Initialize(ID3D11Device* device, const std::string& filePath, ConstantBuffer<CB_VS_VertexShader>& cbvs);
	void Draw(ID3D11DeviceContext* deviceContext, DirectX::FXMMATRIX viewProjectionMatrix);

	virtual void UpdateMatrix() override;

protected:
	Model m_model;
	DirectX::XMFLOAT4X4 m_world_matrix;
};