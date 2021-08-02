#pragma once

#include <memory>
#include <algorithm>

#include <DirectXMath.h>

#include "i_pointer_handler.h"
#include "i_keyboard_handler.h"
#include "../nodes/scene_node.h"

class MovementController : public IPointerHandler, public IKeyboardHandler {
protected:
	DirectX::XMFLOAT4X4 m_matFromWorld;
	DirectX::XMFLOAT4X4 m_matToWorld;
	DirectX::XMFLOAT4X4 m_matPosition;

	int m_lastMousePos_x;
	int m_lastMousePos_y;
	bool m_bKey[256];

	float m_fTargetYaw;
	float m_fTargetPitch;
	float m_fYaw;
	float m_fPitch;
	float m_fPitchOnDown;
	float m_fYawOnDown;
	float m_maxSpeed;
	float m_currentSpeed;

	bool m_mouseLButtonDown;
	bool m_bRotateWhenLButtonDown;

	std::shared_ptr<SceneNode> m_object;

public:
	MovementController(std::shared_ptr<SceneNode> object, float initialYaw, float initialPitch, bool rotateWhenLButtonDown);
	void SetObject(std::shared_ptr<SceneNode> newObject);

	void OnUpdate(float elapsed_seconds);

public:
	bool VOnPointerMove(int x, int y, const int radius) override;
	bool VOnPointerButtonDown(int x, int y, const int radius, const std::string& buttonName) override;
	bool VOnPointerButtonUp(int x, int y, const int radius, const std::string& buttonName) override;

	bool VOnKeyDown(const BYTE c);
	bool VOnKeyUp(const BYTE c);

	const DirectX::XMFLOAT4X4& GetToWorld4x4();
	DirectX::XMMATRIX GetToWorld();

	const DirectX::XMFLOAT4X4& GetFromWorld4x4();
	DirectX::XMMATRIX GetFromWorld();
};