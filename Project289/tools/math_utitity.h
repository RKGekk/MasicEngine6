#pragma once

#include <DirectXMath.h>
#include <limits>

const float EPSILON = std::numeric_limits<float>::epsilon();

DirectX::XMVECTOR GetTranslation(DirectX::XMMATRIX mat);
DirectX::XMVECTOR BarycentricToVec3(DirectX::FXMVECTOR v0, DirectX::FXMVECTOR v1, DirectX::FXMVECTOR v2, float u, float v);
DirectX::XMVECTOR BarycentricToVec3(const DirectX::XMFLOAT3& v0, const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2, float u, float v);
DirectX::XMVECTOR BarycentricToVec3(const DirectX::XMFLOAT4& v0, const DirectX::XMFLOAT4& v1, const DirectX::XMFLOAT4& v2, float u, float v);

const DirectX::XMFLOAT3 DEFAULT_FORWARD_VECTOR = { 0.0f, 0.0f, 1.0f };
const DirectX::XMFLOAT3 DEFAULT_BACKWARD_VECTOR = { 0.0f, 0.0f, -1.0f };
const DirectX::XMFLOAT3 DEFAULT_UP_VECTOR = { 0.0f, 1.0f, 0.0f };
const DirectX::XMFLOAT3 DEFAULT_DOWN_VECTOR = { 0.0f, -1.0f, 0.0f };
const DirectX::XMFLOAT3 DEFAULT_LEFT_VECTOR = { -1.0f, 0.0f, 0.0f };
const DirectX::XMFLOAT3 DEFAULT_RIGHT_VECTOR = { 1.0f, 0.0f, 0.0f };