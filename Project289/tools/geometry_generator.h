#pragma once

#include <DirectXMath.h>
#include <vector>
#include <algorithm>

#include "../graphics/mesh_holder.h"

class GeometryGenerator {
public:
	void CreateBox(float width, float height, float depth, MeshHolder& meshData);
	void CreateSphere(float radius, unsigned int sliceCount, unsigned int stackCount, MeshHolder& meshData);
	void CreateGeosphere(float radius, unsigned int numSubdivisions, MeshHolder& meshData);
	void CreateCylinder(float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount, MeshHolder& meshData);
	void CreateGrid(float width, float depth, unsigned int m, unsigned int n, MeshHolder& meshData);
	void CreateFullscreenQuad(MeshHolder& meshData);

private:
	void Subdivide(MeshHolder& meshData);
	void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount, MeshHolder& meshData);
	void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount, MeshHolder& meshData);
};