#include "mesh.h"

Mesh::Mesh(ID3D11Device* device, const std::vector<Vertex>& vertices, const std::vector<DWORD>& indices, const std::vector<MaterialTexture>& textures, DirectX::FXMMATRIX parentTransformMatrix) {
	DirectX::XMStoreFloat4x4(&m_transform_matrix, parentTransformMatrix);
	m_textures = textures;

	HRESULT hr = m_vertex_buffer.Initialize(device, vertices);
	COM_ERROR_IF_FAILED(hr, "Failed to create vertex buffer data.");

	hr = m_index_buffer.Initialize(device, indices);
	COM_ERROR_IF_FAILED(hr, "Failed to create indices buffer data.");
}

Mesh::Mesh(const Mesh& rhv) {
	m_vertex_buffer = rhv.m_vertex_buffer;
	m_index_buffer = rhv.m_index_buffer;
	m_textures = rhv.m_textures;
	m_transform_matrix = rhv.m_transform_matrix;
}

void Mesh::Draw(ID3D11DeviceContext* deviceContext) {
	for (int i = 0; i < m_textures.size(); ++i) {
		if (m_textures[i].GetType() == aiTextureType::aiTextureType_DIFFUSE) {
			deviceContext->PSSetShaderResources(0, 1, m_textures[i].GetTextureResourceViewAddress());
			break;
		}
	}

	deviceContext->IASetIndexBuffer(m_index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	UINT stride = m_vertex_buffer.Stride();
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, m_vertex_buffer.GetAddressOf(), &stride, &offset);
	deviceContext->DrawIndexed(m_index_buffer.IndexCount(), 0, 0);
}

const DirectX::XMMATRIX Mesh::GetTransformMatrix() {
	return DirectX::XMLoadFloat4x4(&m_transform_matrix);
}

const DirectX::XMFLOAT4X4& Mesh::GetTransformMatrix4() {
	return m_transform_matrix;
}