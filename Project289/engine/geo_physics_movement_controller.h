#pragma once

#include <memory>
#include <algorithm>

#include <DirectXMath.h>

#include "i_pointer_handler.h"
#include "i_keyboard_handler.h"
#include "../actors/actor.h"
#include "../nodes/scene_node.h"

class GeoPhysicsMovementController : public IPointerHandler, public IKeyboardHandler {
protected:
	DirectX::XMFLOAT4X4 m_matFromWorld;
	DirectX::XMFLOAT4X4 m_matToWorld;
	DirectX::XMFLOAT4X4 m_matPosition;

	float m_maxSpeed;
	float m_currentSpeed;
	float m_force;
	float m_jump_force;

	bool m_bKey[256];

	std::shared_ptr<SceneNode> m_object;

public:
	GeoPhysicsMovementController(std::shared_ptr<SceneNode> object);
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