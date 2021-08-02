#include "geometry_generator.h"

void GeometryGenerator::CreateBox(float width, float height, float depth, MeshHolder& meshData) {
	Vertex v[24];

	float w2 = 0.5f * width;
	float h2 = 0.5f * height;
	float d2 = 0.5f * depth;

	// Fill in the front face vertex data.
	v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	v[8] = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9] = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	meshData.vertices.assign(&v[0], &v[24]);

	// Create the indices.
	unsigned int i[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	meshData.indices.assign(&i[0], &i[36]);
}

void GeometryGenerator::CreateSphere(float radius, unsigned int sliceCount, unsigned int stackCount, MeshHolder& meshData) {
	meshData.vertices.clear();
	meshData.indices.clear();

	// Compute the vertices stating at the top pole and moving down the stacks.

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	meshData.vertices.push_back(topVertex);

	float phiStep = DirectX::XM_PI / stackCount;
	float thetaStep = 2.0f * DirectX::XM_PI / sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (unsigned int i = 1; i <= stackCount - 1; ++i) {
		float phi = i * phiStep;

		// Vertices of ring.
		for (unsigned int j = 0; j <= sliceCount; ++j) {
			float theta = j * thetaStep;

			Vertex v;

			// spherical to cartesian
			v.pos.x = radius * DirectX::XMScalarSin(phi) * DirectX::XMScalarCos(theta);
			v.pos.y = radius * DirectX::XMScalarCos(phi);
			v.pos.z = radius * DirectX::XMScalarSin(phi) * DirectX::XMScalarSin(theta);

			// Partial derivative of P with respect to theta
			v.tg.x = -radius * DirectX::XMScalarSin(phi) * DirectX::XMScalarSin(theta);
			v.tg.y = 0.0f;
			v.tg.z = +radius * DirectX::XMScalarSin(phi) * DirectX::XMScalarCos(theta);

			DirectX::XMVECTOR T = DirectX::XMLoadFloat3(&v.tg);
			DirectX::XMStoreFloat3(&v.tg, DirectX::XMVector3Normalize(T));

			DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&v.pos);
			DirectX::XMStoreFloat3(&v.normal, DirectX::XMVector3Normalize(p));

			v.uv.x = theta / DirectX::XM_2PI;
			v.uv.y = phi / DirectX::XM_PI;

			meshData.vertices.push_back(v);
		}
	}

	meshData.vertices.push_back(bottomVertex);

	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	for (unsigned int i = 1; i <= sliceCount; ++i) {
		meshData.indices.push_back(0);
		meshData.indices.push_back(i + 1);
		meshData.indices.push_back(i);
	}

	// Compute indices for inner stacks (not connected to poles).

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	unsigned int baseIndex = 1;
	unsigned int ringVertexCount = sliceCount + 1;
	for (unsigned int i = 0; i < stackCount - 2; ++i) {
		for (unsigned int j = 0; j < sliceCount; ++j) {
			meshData.indices.push_back(baseIndex + i * ringVertexCount + j);
			meshData.indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			meshData.indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			meshData.indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			meshData.indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			meshData.indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.

	// South pole vertex was added last.
	unsigned int southPoleIndex = (unsigned int)meshData.vertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (unsigned int i = 0; i < sliceCount; ++i) {
		meshData.indices.push_back(southPoleIndex);
		meshData.indices.push_back(baseIndex + i);
		meshData.indices.push_back(baseIndex + i + 1);
	}
}

void GeometryGenerator::Subdivide(MeshHolder& meshData) {
	// Save a copy of the input geometry.
	MeshHolder inputCopy = meshData;

	meshData.vertices.resize(0);
	meshData.indices.resize(0);

	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	unsigned int numTris = inputCopy.indices.size() / 3;
	for (unsigned int i = 0; i < numTris; ++i) {
		Vertex v0 = inputCopy.vertices[inputCopy.indices[i * 3 + 0]];
		Vertex v1 = inputCopy.vertices[inputCopy.indices[i * 3 + 1]];
		Vertex v2 = inputCopy.vertices[inputCopy.indices[i * 3 + 2]];

		// Generate the midpoints.
		Vertex m0, m1, m2;

		// For subdivision, we just care about the position component.  We derive the other
		// vertex components in CreateGeosphere.
		m0.pos = DirectX::XMFLOAT3(
			0.5f * (v0.pos.x + v1.pos.x),
			0.5f * (v0.pos.y + v1.pos.y),
			0.5f * (v0.pos.z + v1.pos.z)
		);

		m1.pos = DirectX::XMFLOAT3(
			0.5f * (v1.pos.x + v2.pos.x),
			0.5f * (v1.pos.y + v2.pos.y),
			0.5f * (v1.pos.z + v2.pos.z)
		);

		m2.pos = DirectX::XMFLOAT3(
			0.5f * (v0.pos.x + v2.pos.x),
			0.5f * (v0.pos.y + v2.pos.y),
			0.5f * (v0.pos.z + v2.pos.z)
		);

		// Add new geometry.
		meshData.vertices.push_back(v0); // 0
		meshData.vertices.push_back(v1); // 1
		meshData.vertices.push_back(v2); // 2
		meshData.vertices.push_back(m0); // 3
		meshData.vertices.push_back(m1); // 4
		meshData.vertices.push_back(m2); // 5

		meshData.indices.push_back(i * 6 + 0);
		meshData.indices.push_back(i * 6 + 3);
		meshData.indices.push_back(i * 6 + 5);

		meshData.indices.push_back(i * 6 + 3);
		meshData.indices.push_back(i * 6 + 4);
		meshData.indices.push_back(i * 6 + 5);

		meshData.indices.push_back(i * 6 + 5);
		meshData.indices.push_back(i * 6 + 4);
		meshData.indices.push_back(i * 6 + 2);

		meshData.indices.push_back(i * 6 + 3);
		meshData.indices.push_back(i * 6 + 1);
		meshData.indices.push_back(i * 6 + 4);
	}
}

void GeometryGenerator::CreateGeosphere(float radius, unsigned int numSubdivisions, MeshHolder& meshData) {
	// Put a cap on the number of subdivisions.
	numSubdivisions = (std::min)(numSubdivisions, 5u);

	// Approximate a sphere by tessellating an icosahedron.
	const float X = 0.525731f;
	const float Z = 0.850651f;

	DirectX::XMFLOAT3 pos[12] = {
		DirectX::XMFLOAT3(-X,		 0.0f,	 Z),	DirectX::XMFLOAT3( X,		 0.0f,	 Z),
		DirectX::XMFLOAT3(-X,		 0.0f,	-Z),	DirectX::XMFLOAT3( X,		 0.0f,	-Z),
		DirectX::XMFLOAT3( 0.0f,	 Z,		 X),	DirectX::XMFLOAT3( 0.0f,	 Z,		-X),
		DirectX::XMFLOAT3( 0.0f,	-Z,		 X),	DirectX::XMFLOAT3( 0.0f,	-Z,		-X),
		DirectX::XMFLOAT3( Z,		 X,		 0.0f),	DirectX::XMFLOAT3(-Z,		 X,		 0.0f),
		DirectX::XMFLOAT3( Z,		-X,		 0.0f),	DirectX::XMFLOAT3(-Z,		-X,		 0.0f)
	};

	int k[60] = {
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	meshData.vertices.resize(12);
	meshData.indices.resize(60);

	for (unsigned int i = 0; i < 12; ++i) {
		meshData.vertices[i].pos = pos[i];
	}

	for (unsigned int i = 0; i < 60; ++i) {
		meshData.indices[i] = k[i];
	}

	for (unsigned int i = 0; i < numSubdivisions; ++i) {
		Subdivide(meshData);
	}

	// Project vertices onto sphere and scale.
	for (unsigned int i = 0; i < meshData.vertices.size(); ++i) {
		// Project onto unit sphere.
		DirectX::XMVECTOR n = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&meshData.vertices[i].pos));

		// Project onto sphere.
		DirectX::XMVECTOR p = DirectX::XMVectorScale(n, radius);

		DirectX::XMStoreFloat3(&meshData.vertices[i].pos, p);
		DirectX::XMStoreFloat3(&meshData.vertices[i].normal, n);

		// Derive texture coordinates from spherical coordinates. Angle from XY
		float x = meshData.vertices[i].pos.x;
		float y = meshData.vertices[i].pos.z;
		DirectX::XMVECTOR xXM = DirectX::XMVectorReplicate(x);
		DirectX::XMVECTOR yXM = DirectX::XMVectorReplicate(y);
		float theta = DirectX::XMVectorGetX(DirectX::XMVectorATan2(yXM, xXM)); // in [-pi/2, +pi/2]
		// Quadrant I or IV
		if (x >= 0.0f) { // in [0, 2*pi); If x = 0, then atanf(y/x) = +pi/2 if y > 0; atanf(y/x) = -pi/2 if y < 0 
			if (theta < 0.0f) {
				theta += 2.0f * DirectX::XM_PI;
			}
		} 
		// Quadrant II or III
		else {
			theta += DirectX::XM_PI; // in [0, 2*pi).
		}

		float phi = DirectX::XMScalarACos(meshData.vertices[i].pos.y / radius);

		meshData.vertices[i].uv.x = theta / DirectX::XM_2PI;
		meshData.vertices[i].uv.y = phi / DirectX::XM_PI;

		// Partial derivative of P with respect to theta
		meshData.vertices[i].tg.x = -radius * DirectX::XMScalarSin(phi) * DirectX::XMScalarSin(theta);
		meshData.vertices[i].tg.y = 0.0f;
		meshData.vertices[i].tg.z = +radius * DirectX::XMScalarSin(phi) * DirectX::XMScalarCos(theta);

		DirectX::XMVECTOR T = DirectX::XMLoadFloat3(&meshData.vertices[i].tg);
		DirectX::XMStoreFloat3(&meshData.vertices[i].tg, DirectX::XMVector3Normalize(T));
	}
}

void GeometryGenerator::CreateCylinder(float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount, MeshHolder& meshData) {
	meshData.vertices.clear();
	meshData.indices.clear();

	// Build Stacks.
	float stackHeight = height / stackCount;

	// Amount to increment radius as we move up each stack level from bottom to top.
	float radiusStep = (topRadius - bottomRadius) / stackCount;

	unsigned int ringCount = stackCount + 1;

	// Compute vertices for each stack ring starting at the bottom and moving up.
	for (unsigned int i = 0; i < ringCount; ++i) {
		float y = -0.5f * height + i * stackHeight;
		float r = bottomRadius + i * radiusStep;

		// vertices of ring
		float dTheta = 2.0f * DirectX::XM_PI / sliceCount;
		for (unsigned int j = 0; j <= sliceCount; ++j) {
			Vertex vertex;

			float c = DirectX::XMScalarCos(j * dTheta);
			float s = DirectX::XMScalarSin(j * dTheta);

			vertex.pos = DirectX::XMFLOAT3(r * c, y, r * s);

			vertex.uv.x = (float)j / sliceCount;
			vertex.uv.y = 1.0f - (float)i / stackCount;

			// Cylinder can be parameterized as follows, where we introduce v
			// parameter that goes in the same direction as the v tex-coord
			// so that the bitangent goes in the same direction as the v tex-coord.
			//   Let r0 be the bottom radius and let r1 be the top radius.
			//   y(v) = h - hv for v in [0,1].
			//   r(v) = r1 + (r0-r1)v
			//
			//   x(t, v) = r(v)*cos(t)
			//   y(t, v) = h - hv
			//   z(t, v) = r(v)*sin(t)
			// 
			//  dx/dt = -r(v)*sin(t)
			//  dy/dt = 0
			//  dz/dt = +r(v)*cos(t)
			//
			//  dx/dv = (r0-r1)*cos(t)
			//  dy/dv = -h
			//  dz/dv = (r0-r1)*sin(t)

			// This is unit length.
			vertex.tg = DirectX::XMFLOAT3(-s, 0.0f, c);

			float dr = bottomRadius - topRadius;
			DirectX::XMFLOAT3 bitangent(dr * c, -height, dr * s);

			DirectX::XMVECTOR T = DirectX::XMLoadFloat3(&vertex.tg);
			DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&bitangent);
			DirectX::XMVECTOR N = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(T, B));
			DirectX::XMStoreFloat3(&vertex.normal, N);

			meshData.vertices.push_back(vertex);
		}
	}

	// Add one because we duplicate the first and last vertex per ring
	// since the texture coordinates are different.
	unsigned int ringVertexCount = sliceCount + 1;

	// Compute indices for each stack.
	for (unsigned int i = 0; i < stackCount; ++i) {
		for (unsigned int j = 0; j < sliceCount; ++j) {
			meshData.indices.push_back(i * ringVertexCount + j);
			meshData.indices.push_back((i + 1) * ringVertexCount + j);
			meshData.indices.push_back((i + 1) * ringVertexCount + j + 1);

			meshData.indices.push_back(i * ringVertexCount + j);
			meshData.indices.push_back((i + 1) * ringVertexCount + j + 1);
			meshData.indices.push_back(i * ringVertexCount + j + 1);
		}
	}

	BuildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
	BuildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
}

void GeometryGenerator::BuildCylinderTopCap(float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount, MeshHolder& meshData) {
	unsigned int baseIndex = (unsigned int)meshData.vertices.size();

	float y = 0.5f * height;
	float dTheta = 2.0f * DirectX::XM_PI / sliceCount;

	// Duplicate cap ring vertices because the texture coordinates and normals differ.
	for (unsigned int i = 0; i <= sliceCount; ++i) {
		float x = topRadius * DirectX::XMScalarCos(i * dTheta);
		float z = topRadius * DirectX::XMScalarSin(i * dTheta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		meshData.vertices.push_back(Vertex(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
	}

	// Cap center vertex.
	meshData.vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));

	// Index of center vertex.
	unsigned int centerIndex = (unsigned int)meshData.vertices.size() - 1;

	for (unsigned int i = 0; i < sliceCount; ++i) {
		meshData.indices.push_back(centerIndex);
		meshData.indices.push_back(baseIndex + i + 1);
		meshData.indices.push_back(baseIndex + i);
	}
}

void GeometryGenerator::BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount, MeshHolder& meshData) {
	// Build bottom cap.
	unsigned int baseIndex = (unsigned int)meshData.vertices.size();
	float y = -0.5f * height;

	// vertices of ring
	float dTheta = 2.0f * DirectX::XM_PI / sliceCount;
	for (unsigned int i = 0; i <= sliceCount; ++i) {
		float x = bottomRadius * DirectX::XMScalarCos(i * dTheta);
		float z = bottomRadius * DirectX::XMScalarSin(i * dTheta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		meshData.vertices.push_back(Vertex(x, y, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
	}

	// Cap center vertex.
	meshData.vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));

	// Cache the index of center vertex.
	unsigned int centerIndex = (unsigned int)meshData.vertices.size() - 1;

	for (unsigned int i = 0; i < sliceCount; ++i) {
		meshData.indices.push_back(centerIndex);
		meshData.indices.push_back(baseIndex + i);
		meshData.indices.push_back(baseIndex + i + 1);
	}
}

void GeometryGenerator::CreateGrid(float width, float depth, unsigned int m, unsigned int n, MeshHolder& meshData) {
	unsigned int vertexCount = m * n;
	unsigned int faceCount = (m - 1) * (n - 1) * 2;

	// Create the vertices.
	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;

	float dx = width / (n - 1);
	float dz = depth / (m - 1);

	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	meshData.vertices.resize(vertexCount);
	for (unsigned int i = 0; i < m; ++i) {
		float z = halfDepth - i * dz;
		for (unsigned int j = 0; j < n; ++j) {
			float x = -halfWidth + j * dx;

			meshData.vertices[i * n + j].pos = DirectX::XMFLOAT3(x, 0.0f, z);
			meshData.vertices[i * n + j].normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
			meshData.vertices[i * n + j].tg = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);

			// Stretch texture over grid.
			meshData.vertices[i * n + j].uv.x = j * du;
			meshData.vertices[i * n + j].uv.y = i * dv;
		}
	}

	// Create the indices.
	meshData.indices.resize(faceCount * 3); // 3 indices per face

	// Iterate over each quad and compute indices.
	unsigned int k = 0;
	for (unsigned int i = 0; i < m - 1; ++i) {
		for (unsigned int j = 0; j < n - 1; ++j) {
			meshData.indices[k] = i * n + j;
			meshData.indices[k + 1] = i * n + j + 1;
			meshData.indices[k + 2] = (i + 1) * n + j;

			meshData.indices[k + 3] = (i + 1) * n + j;
			meshData.indices[k + 4] = i * n + j + 1;
			meshData.indices[k + 5] = (i + 1) * n + j + 1;

			k += 6; // next quad
		}
	}
}

void GeometryGenerator::CreateFullscreenQuad(MeshHolder& meshData) {
	meshData.vertices.resize(4);
	meshData.indices.resize(6);

	// Position coordinates specified in NDC space.
	meshData.vertices[0] = Vertex(
		-1.0f, -1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f
	);

	meshData.vertices[1] = Vertex(
		-1.0f, +1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f
	);

	meshData.vertices[2] = Vertex(
		+1.0f, +1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f
	);

	meshData.vertices[3] = Vertex(
		+1.0f, -1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f
	);

	meshData.indices[0] = 0;
	meshData.indices[1] = 1;
	meshData.indices[2] = 2;

	meshData.indices[3] = 0;
	meshData.indices[4] = 2;
	meshData.indices[5] = 3;
}