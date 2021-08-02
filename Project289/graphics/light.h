#pragma once

#include <DirectXMath.h>

#include "renderable_game_object.h"
#include "constant_buffer.h"
#include "constant_buffer_types.h"

class Light : public RenderableGameObject {
public:
	bool Initialize(ID3D11Device* device, ConstantBuffer<CB_VS_VertexShader>& cb_vs);

	DirectX::XMFLOAT3 m_light_color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	float m_light_strenght = 1.0f;
	float m_light_attenuation_a = 1.0f;
	float m_light_attenuation_b = 0.1f;
	float m_light_attenuation_c = 0.1f;
};