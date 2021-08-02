#include "light.h"

bool Light::Initialize(ID3D11Device* device, ConstantBuffer<CB_VS_VertexShader>& cb_vs) {
	if (!m_model.Initialize(device, "Data/light.fbx", cb_vs)) { return false; }

	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);
	UpdateMatrix();
	return true;
}
