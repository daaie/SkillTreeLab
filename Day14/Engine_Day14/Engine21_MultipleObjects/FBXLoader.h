#pragma once
#include "DXUtil.h"
#include "Shape.h"

namespace FBXLoader
{
	// FBX Manager 변수.
	extern FbxManager* fbxManager;

	// FBX 로드 메소드.
	HRESULT LoadFBX(const char* fileName, std::vector<Shape::Vertex>* pOutVertex, std::vector<DWORD>* pOutIndices);

	// FBX 로드 메소드.
	HRESULT LoadFBX(const char* fileName, Shape::Mesh* mesh);

	// 정점 노멀 읽어오는 메소드.
	XMFLOAT3 ReadNormal(FbxMesh* pMesh, int vertexIndex, int vertexCounter);

	XMFLOAT2 ReadUV(FbxMesh* pMesh, int vertexIndex, int vertexCounter);

	XMFLOAT3 ReadTangent(FbxMesh* pMesh, int vertexIndex, int vertexCounter);

	XMFLOAT3 ReadBinormal(FbxMesh* pMesh, int vertexIndex, int vertexCounter);
}