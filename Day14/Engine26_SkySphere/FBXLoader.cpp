#include "FBXLoader.h"

namespace FBXLoader
{
	FbxManager* fbxManager;

	HRESULT LoadFBX(const char * fileName, std::vector<Shape::Vertex>* pOutVertex, std::vector<DWORD>* pOutIndices)
	{
		// fbxManager�� �������� ������ ����.
		if (fbxManager == NULL)
		{
			// �ʱ� SDK Manager ����.
			fbxManager = FbxManager::Create();

			FbxIOSettings* pIOSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
			fbxManager->SetIOSettings(pIOSettings);
		}

		FbxGeometryConverter geometryConverter(fbxManager);
		FbxImporter* pImporter = FbxImporter::Create(fbxManager, "");
		FbxScene* pFbxScene = FbxScene::Create(fbxManager, "");

		// ������ �ʱ�ȭ.
		if (!pImporter->Initialize(fileName, -1, fbxManager->GetIOSettings()))
		{
			return E_FAIL;
		}

		// 3D �� ����Ʈ.
		if (!pImporter->Import(pFbxScene))
		{
			return E_FAIL;
		}

		// ������ ����.
		pImporter->Destroy();

		// FbxNode �б�.
		FbxNode* pFbxRootNode = pFbxScene->GetRootNode();
		if (pFbxRootNode == NULL) return E_FAIL;

		// �𵨸� ������ �⺻ ������ �ﰢ������ ����.
		geometryConverter.Triangulate(pFbxScene, true);

		int childCount = pFbxRootNode->GetChildCount();
		for (int ix = 0; ix < childCount; ++ix)
		{
			// �ڽ� ���� ��� �о����.
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(ix);
			if (pFbxChildNode->GetNodeAttribute() == NULL)
				continue;

			// ��� �Ӽ� Ÿ�� Ȯ��.
			FbxNodeAttribute::EType attrType
				= pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			// ��� Ÿ���� �޽��� �ƴ� ��쿡�� ����.
			if (attrType != FbxNodeAttribute::EType::eMesh)
				continue;

			// �޽� ���� �б�.
			FbxMesh* pMesh = pFbxChildNode->GetMesh();


			// ��Ʈ�� ����Ʈ �� �迭 ���� ������.
			FbxVector4* pVertices = pMesh->GetControlPoints();
			// ������ �� Ȯ��.
			int polygonCount = pMesh->GetPolygonCount();
			int vertexCounter = 0;
			for (int jx = 0; jx < polygonCount; ++jx)
			{
				// �������� �����ϴ� ���� ���� ���ϱ�.
				int vertexCount = pMesh->GetPolygonSize(jx);
				for (int kx = 0; kx < vertexCount; ++kx)
				{
					// �ﰢ�� ��ȣ, ���� ��ȣ�� ���� ���� �б�.
					int vertexIndex = pMesh->GetPolygonVertex(jx, kx);

					// ���� ��ġ ���� �о����.
					Shape::Vertex vertex;
					vertex.position.x = (float)pVertices[vertexIndex].mData[0];
					vertex.position.y = (float)pVertices[vertexIndex].mData[1];
					vertex.position.z = (float)pVertices[vertexIndex].mData[2];

					// ��� �� �о����.
					vertex.normal = ReadNormal(pMesh, vertexIndex, vertexCounter);

					// UV �� �о����.
					vertex.texCoord = ReadUV(pMesh, vertexIndex, vertexCounter);

					bool notHasTangent = pMesh->GetElementTangentCount() < 1;
					if (notHasTangent)
					{
						MessageBox(NULL, L"ź��Ʈ ����", L"����", MB_OK);
						OutputDebugString(L"ź��Ʈ ����");
						OutputDebugString(L"\n");
						return E_FAIL;
					}

					// ź��Ʈ �о����.
					vertex.tangent = notHasTangent ? XMFLOAT3(0.0f, 0.0f, 0.0f) : ReadTangent(pMesh, vertexIndex, vertexCounter);

					bool notHasBinormal = pMesh->GetElementBinormalCount() < 1;
					if (notHasBinormal)
					{
						MessageBox(NULL, L"���̳�� ����", L"����", MB_OK);
						OutputDebugString(L"���̳�� ����");
						OutputDebugString(L"\n");
						return E_FAIL;
					}

					// ���̳�� �о����.
					vertex.biNormal = notHasBinormal ? XMFLOAT3(0.0f, 0.0f, 0.0f) : ReadBinormal(pMesh, vertexIndex, vertexCounter);

					// ���� �迭�� �߰�.
					pOutVertex->push_back(vertex);
					pOutIndices->push_back(vertexCounter);
					++vertexCounter;
				}
			}
		}

		return S_OK;
	}

	HRESULT FBXLoader::LoadFBX(const char * fileName, Shape::Mesh * mesh)
	{
		HRESULT hr = FBXLoader::LoadFBX(fileName, &mesh->vertices, &mesh->indices);
		if (FAILED(hr))
		{
			MessageBox(NULL, L"�𵨸� �ε� ����", L"����", MB_OK);

			OutputDebugString(L"�𵨸� �ε� ����");
			OutputDebugString(L"\n");

			return E_FAIL;
		}

		return S_OK;
	}

	XMFLOAT3 FBXLoader::ReadNormal(FbxMesh * pMesh, int controlPointIndex, int vertexCounter)
	{
		XMFLOAT3 normal(0.0f, 0.0f, 0.0f);
		if (pMesh->GetElementNormalCount() < 1)
		{
			MessageBox(NULL, L"��� ���� ��ȿ���� �ʽ��ϴ�.", L"����", MB_OK);
			return normal;
		}

		FbxGeometryElementNormal* vertexNormal = pMesh->GetElementNormal(0);
		switch (vertexNormal->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			switch (vertexNormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				normal.x = (float)vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0];
				normal.y = (float)vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1];
				normal.z = (float)vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2];
			}
			break;
			case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
				normal.x = (float)vertexNormal->GetDirectArray().GetAt(index).mData[0];
				normal.y = (float)vertexNormal->GetDirectArray().GetAt(index).mData[1];
				normal.z = (float)vertexNormal->GetDirectArray().GetAt(index).mData[2];
			}
			break;

			default: return normal;
			}
			break;

		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (vertexNormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				normal.x = (float)vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[0];
				normal.y = (float)vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[1];
				normal.z = (float)vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[2];
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexNormal->GetIndexArray().GetAt(vertexCounter);
				normal.x = (float)vertexNormal->GetDirectArray().GetAt(index).mData[0];
				normal.y = (float)vertexNormal->GetDirectArray().GetAt(index).mData[1];
				normal.z = (float)vertexNormal->GetDirectArray().GetAt(index).mData[2];
			}
			break; default: return normal;
			}
		}
		break;

		default: return normal;
		}

		return normal;
	}

	XMFLOAT2 FBXLoader::ReadUV(FbxMesh * pMesh, int vertexIndex, int vertexCounter)
	{
		// UV�� �ִ��� Ȯ��.
		if (pMesh->GetElementUVCount() < 1)
		{
			MessageBox(NULL, L"UV ���� ��ȿ���� �ʽ��ϴ�", L"����", MB_OK);
			return NULL;
		}

		// ���Ͽ� ���� ����.
		XMFLOAT2 texCoord(0.0f, 0.0f);
		// UV ��ü �迭 ������.
		FbxGeometryElementUV* vertexUV = pMesh->GetElementUV(0);
		const bool isUsingIndex = vertexUV->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int indexCount = isUsingIndex ? vertexUV->GetIndexArray().GetCount() : 0;

		switch (vertexUV->GetMappingMode())
		{
			// UV �Ӽ��� ������ ������ ���.
		case FbxGeometryElement::eByControlPoint:
		{
			// ���� UV ���� �о�� �ε��� ������.
			int index = isUsingIndex ? vertexUV->GetIndexArray().GetAt(vertexIndex) : vertexIndex;

			// UV �� �о����. (DirectX�� FBX�� UV V��ǥ�� ���� �ݴ�).
			texCoord.x = (float)vertexUV->GetDirectArray().GetAt(index).mData[0];
			texCoord.y = 1.0f - (float)vertexUV->GetDirectArray().GetAt(index).mData[1];

			// UV �� ��ȯ.
			return texCoord;
		}

		// UV �Ӽ��� ���� ������ ���.
		case FbxGeometryElement::eByPolygonVertex:
		{
			// ���� UV ���� �о�� �ε��� ������. ���׿�����.
			int index = isUsingIndex == true ? vertexUV->GetIndexArray().GetAt(vertexCounter) : vertexCounter;

			/*if (isUsingIndex == true)
			index = vertexUV->GetIndexArray().GetAt(vertexCounter);
			else
			index = vertexCounter;*/

			// UV �� �о����. (DirectX�� FBX�� UV V��ǥ�� ���� �ݴ�).
			texCoord.x = (float)vertexUV->GetDirectArray().GetAt(index).mData[0];
			texCoord.y = 1.0f - (float)vertexUV->GetDirectArray().GetAt(index).mData[1];

			// UV �� ��ȯ.
			return texCoord;
		}

		default:
		{
			MessageBox(NULL, L"UV ���� ��ȿ���� �ʽ��ϴ�", L"����", MB_OK);
			return NULL;
		}
		}


		return NULL;
	}

	XMFLOAT3 FBXLoader::ReadTangent(FbxMesh * pMesh, int vertexIndex, int vertexCounter)
	{
		// UV�� �ִ��� Ȯ��.
		if (pMesh->GetElementTangentCount() < 1)
		{
			MessageBox(NULL, L"Tangent ���� ��ȿ���� �ʽ��ϴ�", L"����", MB_OK);
			return NULL;
		}

		// ���Ͽ� ���� ����.
		XMFLOAT3 tangent(0.0f, 0.0f, 0.0f);
		// UV ��ü �迭 ������.
		FbxGeometryElementTangent* vertexTangent = pMesh->GetElementTangent(0);
		const bool isUsingIndex = vertexTangent->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int indexCount = isUsingIndex ? vertexTangent->GetIndexArray().GetCount() : 0;

		switch (vertexTangent->GetMappingMode())
		{
			// UV �Ӽ��� ������ ������ ���.
		case FbxGeometryElement::eByControlPoint:
		{
			// ���� UV ���� �о�� �ε��� ������.
			int index = isUsingIndex ? vertexTangent->GetIndexArray().GetAt(vertexIndex) : vertexIndex;

			// UV �� �о����. (DirectX�� FBX�� UV V��ǥ�� ���� �ݴ�).
			tangent.x = (float)vertexTangent->GetDirectArray().GetAt(index).mData[0];
			tangent.y = (float)vertexTangent->GetDirectArray().GetAt(index).mData[1];
			tangent.z = (float)vertexTangent->GetDirectArray().GetAt(index).mData[2];

			// UV �� ��ȯ.
			return tangent;
		}

		// UV �Ӽ��� ���� ������ ���.
		case FbxGeometryElement::eByPolygonVertex:
		{
			// ���� UV ���� �о�� �ε��� ������. ���׿�����.
			int index = isUsingIndex == true ? vertexTangent->GetIndexArray().GetAt(vertexCounter) : vertexCounter;

			// UV �� �о����. (DirectX�� FBX�� UV V��ǥ�� ���� �ݴ�).
			tangent.x = (float)vertexTangent->GetDirectArray().GetAt(index).mData[0];
			tangent.y = (float)vertexTangent->GetDirectArray().GetAt(index).mData[1];
			tangent.z = (float)vertexTangent->GetDirectArray().GetAt(index).mData[2];

			// UV �� ��ȯ.
			return tangent;
		}

		default:
		{
			MessageBox(NULL, L"Tangent ���� ��ȿ���� �ʽ��ϴ�", L"����", MB_OK);
			return NULL;
		}
		}

		return NULL;
	}

	XMFLOAT3 FBXLoader::ReadBinormal(FbxMesh * pMesh, int vertexIndex, int vertexCounter)
	{
		// UV�� �ִ��� Ȯ��.
		if (pMesh->GetElementBinormalCount() < 1)
		{
			MessageBox(NULL, L"Binormal ���� ��ȿ���� �ʽ��ϴ�", L"����", MB_OK);
			return NULL;
		}

		// ���Ͽ� ���� ����.
		XMFLOAT3 binormal(0.0f, 0.0f, 0.0f);
		// UV ��ü �迭 ������.
		FbxGeometryElementBinormal* vertexBinormal = pMesh->GetElementBinormal(0);
		const bool isUsingIndex = vertexBinormal->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int indexCount = isUsingIndex ? vertexBinormal->GetIndexArray().GetCount() : 0;

		switch (vertexBinormal->GetMappingMode())
		{
			// UV �Ӽ��� ������ ������ ���.
		case FbxGeometryElement::eByControlPoint:
		{
			// ���� UV ���� �о�� �ε��� ������.
			int index = isUsingIndex ? vertexBinormal->GetIndexArray().GetAt(vertexIndex) : vertexIndex;

			// UV �� �о����. (DirectX�� FBX�� UV V��ǥ�� ���� �ݴ�).
			binormal.x = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[0];
			binormal.y = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[1];
			binormal.z = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[2];

			// UV �� ��ȯ.
			return binormal;
		}

		// UV �Ӽ��� ���� ������ ���.
		case FbxGeometryElement::eByPolygonVertex:
		{
			// ���� UV ���� �о�� �ε��� ������. ���׿�����.
			int index = isUsingIndex == true ? vertexBinormal->GetIndexArray().GetAt(vertexCounter) : vertexCounter;

			// UV �� �о����. (DirectX�� FBX�� UV V��ǥ�� ���� �ݴ�).
			binormal.x = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[0];
			binormal.y = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[1];
			binormal.z = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[2];

			// UV �� ��ȯ.
			return binormal;
		}

		default:
		{
			MessageBox(NULL, L"Binormal ���� ��ȿ���� �ʽ��ϴ�", L"����", MB_OK);
			return NULL;
		}
		}

		return NULL;
	}
}