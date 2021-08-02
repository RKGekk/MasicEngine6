#pragma once

#include <DirectXMath.h>

struct Vertex {
	Vertex() : pos(0.0f, 0.0f, 0.0f), color(0.0f, 0.0f, 0.0f, 1.0f), uv(0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), tg() {}
	Vertex(float x, float y, float z, float r, float g, float b, float u, float v, float nx, float ny, float nz, float tx, float ty, float tz) : pos(x, y, z), color(r, g, b, 1.0f), uv(u, v), normal(nx, ny, nz), tg(tx, ty, tz) {}
	Vertex(float x, float y, float z, float nx, float ny, float nz, float tx, float ty, float tz, float u, float v) : pos(x, y, z), uv(u, v), normal(nx, ny, nz), tg(tx, ty, tz), color() {}
	Vertex(const DirectX::XMFLOAT3& pos) : pos(pos), normal(), uv(), color(), tg() {}
	Vertex(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& n) : pos(pos), normal(n), uv(), color(), tg() {}
	Vertex(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& n, const DirectX::XMFLOAT2& t) : pos(pos), normal(n), uv(t), color(), tg() {}
	Vertex(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& n, const DirectX::XMFLOAT2& t, const DirectX::XMFLOAT3& tg) : pos(pos), normal(n), uv(t), color(), tg(tg) {}
	Vertex(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& n, const DirectX::XMFLOAT2& t, const DirectX::XMFLOAT3& tg, const DirectX::XMFLOAT4& color) : pos(pos), normal(n), uv(t), color(color), tg(tg) {}

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 tg;
};

struct Basic32 {
	Basic32() : Pos(0.0f, 0.0f, 0.0f), Normal(0.0f, 0.0f, 0.0f), Tex(0.0f, 0.0f) {}
	Basic32(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT3& n, const DirectX::XMFLOAT2& uv) : Pos(p), Normal(n), Tex(uv) {}
	Basic32(float px, float py, float pz, float nx, float ny, float nz, float u, float v) : Pos(px, py, pz), Normal(nx, ny, nz), Tex(u, v) {}

	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 Tex;
};

struct PosColor {
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};

struct PosNormal {
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
};

struct Post {
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 Tex;
};

struct Position {
	DirectX::XMFLOAT3 Pos;
};

struct SmokePointSprite {
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 Size;
	float TTL;
};