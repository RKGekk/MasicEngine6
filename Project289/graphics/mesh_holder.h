#pragma once

#include <vector>

#include <DirectXMath.h>

#include "../graphics/vertex.h"
#include "../graphics/material_texture.h"
#include "../graphics/material_shader.h"

struct MeshHolder {
    std::vector<Vertex> vertices;
    std::vector<DWORD> indices;
    std::vector<MaterialTexture> textures;
    MaterialShader material;
    DirectX::XMFLOAT4X4 transform;
};
