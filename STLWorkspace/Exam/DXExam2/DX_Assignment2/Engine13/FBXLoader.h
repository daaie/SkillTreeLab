#pragma once
#include "DXUtil.h"
#include "Shape.h"

namespace FBXLoader
{
	// FBX Manager ����.
	extern FbxManager* fbxManager;

	// FBX �ε� �޼ҵ�.
	HRESULT LoadFBX(const char* fileName, std::vector<Shape::Vertex>* pOutVertex, std::vector<DWORD>* pOutIndices);

	// FBX �ε� �޼ҵ�.
	HRESULT LoadFBX(const char* fileName, Shape::Mesh* mesh);

	// ���� ��� �о���� �޼ҵ�.
	XMFLOAT3 ReadNormal(FbxMesh* pMesh, int vertexIndex, int vertexCounter);

	XMFLOAT2 ReadUV(FbxMesh* pMesh, int vertexIndex, int vertexCounter);

	XMFLOAT3 ReadTangent(FbxMesh* pMesh, int vertexIndex, int vertexCounter);

	XMFLOAT3 ReadBinormal(FbxMesh* pMesh, int vertexIndex, int vertexCounter);
}