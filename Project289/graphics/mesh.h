#pragma once

#include <vector>
#include <DirectXMath.h>

#include "vertex.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "constant_buffer.h"
#include "constant_buffer_types.h"

#include "material_texture.h"

class Mesh {
public:
	Mesh(ID3D11Device* device, const std::vector<Vertex>& vertices, const std::vector<DWORD>& indices, const std::vector<MaterialTexture>& textures, DirectX::FXMMATRIX parentTransformMatrix);
	Mesh(const Mesh& rhv);
	void Draw(ID3D11DeviceContext* deviceContext);
	const DirectX::XMMATRIX GetTransformMatrix();
	const DirectX::XMFLOAT4X4& GetTransformMatrix4();

private:
	VertexBuffer<Vertex> m_vertex_buffer;
	IndexBuffer m_index_buffer;
	std::vector<MaterialTexture> m_textures;

	DirectX::XMFLOAT4X4 m_transform_matrix;
};