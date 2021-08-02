#pragma once

#include <DirectXMath.h>

#include "game_object.h"

class Camera : public GameObject {
public:
	Camera();
	void SetProjectionValues(float fov, float aspect_ratio, float near_z, float far_z);

	const DirectX::XMMATRIX GetViewMatrix() const;
	const DirectX::XMMATRIX GetViewMatrixT() const;
	const DirectX::XMFLOAT4X4 GetViewMatrix4x4() const;
	const DirectX::XMFLOAT4X4 GetViewMatrix4x4T() const;

	const DirectX::XMMATRIX GetProjectionMatrix() const;
	const DirectX::XMMATRIX GetProjectionMatrixT() const;
	const DirectX::XMFLOAT4X4 GetProjectionMatrix4x4() const;
	const DirectX::XMFLOAT4X4 GetProjectionMatrix4x4T() const;

	virtual void UpdateMatrix() override;

private:
	DirectX::XMFLOAT4X4 m_viewMatrix;
	DirectX::XMFLOAT4X4 m_projectionMatrix;
};