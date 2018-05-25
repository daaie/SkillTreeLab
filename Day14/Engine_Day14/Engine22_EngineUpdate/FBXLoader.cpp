#include "FBXLoader.h"

namespace FBXLoader
{
	FbxManager* fbxManager;

	HRESULT LoadFBX(const char * fileName, std::vector<Shape::Vertex>* pOutVertex, std::vector<DWORD>* pOutIndices)
	{
		// fbxManager가 생성되지 않으면 생성.
		if (fbxManager == NULL)
		{
			// 초기 SDK Manager 설정.
			fbxManager = FbxManager::Create();

			FbxIOSettings* pIOSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
			fbxManager->SetIOSettings(pIOSettings);
		}

		FbxGeometryConverter geometryConverter(fbxManager);
		FbxImporter* pImporter = FbxImporter::Create(fbxManager, "");
		FbxScene* pFbxScene = FbxScene::Create(fbxManager, "");

		// 임포터 초기화.
		if (!pImporter->Initialize(fileName, -1, fbxManager->GetIOSettings()))
		{
			return E_FAIL;
		}

		// 3D 씬 임포트.
		if (!pImporter->Import(pFbxScene))
		{
			return E_FAIL;
		}

		// 임포터 해제.
		pImporter->Destroy();

		// FbxNode 읽기.
		FbxNode* pFbxRootNode = pFbxScene->GetRootNode();
		if (pFbxRootNode == NULL) return E_FAIL;

		// 모델링 폴리곤 기본 구조를 삼각면으로 설정.
		geometryConverter.Triangulate(pFbxScene, true);

		int childCount = pFbxRootNode->GetChildCount();
		for (int ix = 0; ix < childCount; ++ix)
		{
			// 자식 계층 노드 읽어오기.
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(ix);
			if (pFbxChildNode->GetNodeAttribute() == NULL)
				continue;

			// 노드 속성 타입 확인.
			FbxNodeAttribute::EType attrType
				= pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			// 노드 타입이 메쉬가 아닌 경우에는 무시.
			if (attrType != FbxNodeAttribute::EType::eMesh)
				continue;

			// 메쉬 정보 읽기.
			FbxMesh* pMesh = pFbxChildNode->GetMesh();


			// 컨트롤 포인트 수 배열 정보 얻어오기.
			FbxVector4* pVertices = pMesh->GetControlPoints();
			// 폴리곤 수 확인.
			int polygonCount = pMesh->GetPolygonCount();
			int vertexCounter = 0;
			for (int jx = 0; jx < polygonCount; ++jx)
			{
				// 폴리곤을 구성하는 정점 개수 구하기.
				int vertexCount = pMesh->GetPolygonSize(jx);
				for (int kx = 0; kx < vertexCount; ++kx)
				{
					// 삼각면 번호, 정점 번호를 통해 정점 읽기.
					int vertexIndex = pMesh->GetPolygonVertex(jx, kx);

					// 정점 위치 정보 읽어오기.
					Shape::Vertex vertex;
					vertex.position.x = (float)pVertices[vertexIndex].mData[0];
					vertex.position.y = (float)pVertices[vertexIndex].mData[1];
					vertex.position.z = (float)pVertices[vertexIndex].mData[2];

					// 노멀 값 읽어오기.
					vertex.normal = ReadNormal(pMesh, vertexIndex, vertexCounter);

					// UV 값 읽어오기.
					vertex.texCoord = ReadUV(pMesh, vertexIndex, vertexCounter);

					bool notHasTangent = pMesh->GetElementTangentCount() < 1;
					if (notHasTangent)
					{
						MessageBox(NULL, L"탄젠트 없음", L"오류", MB_OK);
						OutputDebugString(L"탄젠트 없음");
						OutputDebugString(L"\n");
						return E_FAIL;
					}

					// 탄젠트 읽어오기.
					vertex.tangent = notHasTangent ? XMFLOAT3(0.0f, 0.0f, 0.0f) : ReadTangent(pMesh, vertexIndex, vertexCounter);

					bool notHasBinormal = pMesh->GetElementBinormalCount() < 1;
					if (notHasBinormal)
					{
						MessageBox(NULL, L"바이노멀 없음", L"오류", MB_OK);
						OutputDebugString(L"바이노멀 없음");
						OutputDebugString(L"\n");
						return E_FAIL;
					}

					// 바이노멀 읽어오기.
					vertex.biNormal = notHasBinormal ? XMFLOAT3(0.0f, 0.0f, 0.0f) : ReadBinormal(pMesh, vertexIndex, vertexCounter);

					// 정점 배열에 추가.
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
			MessageBox(NULL, L"모델링 로드 실패", L"오류", MB_OK);

			OutputDebugString(L"모델링 로드 실패");
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
			MessageBox(NULL, L"노멀 값이 유효하지 않습니다.", L"오류", MB_OK);
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
		// UV가 있는지 확인.
		if (pMesh->GetElementUVCount() < 1)
		{
			MessageBox(NULL, L"UV 값이 유효하지 않습니다", L"오류", MB_OK);
			return NULL;
		}

		// 리턴용 변수 선언.
		XMFLOAT2 texCoord(0.0f, 0.0f);
		// UV 전체 배열 얻어오기.
		FbxGeometryElementUV* vertexUV = pMesh->GetElementUV(0);
		const bool isUsingIndex = vertexUV->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int indexCount = isUsingIndex ? vertexUV->GetIndexArray().GetCount() : 0;

		switch (vertexUV->GetMappingMode())
		{
			// UV 속성이 제어점 유형인 경우.
		case FbxGeometryElement::eByControlPoint:
		{
			// 현재 UV 값을 읽어올 인덱스 얻어오기.
			int index = isUsingIndex ? vertexUV->GetIndexArray().GetAt(vertexIndex) : vertexIndex;

			// UV 값 읽어오기. (DirectX와 FBX의 UV V좌표는 서로 반대).
			texCoord.x = (float)vertexUV->GetDirectArray().GetAt(index).mData[0];
			texCoord.y = 1.0f - (float)vertexUV->GetDirectArray().GetAt(index).mData[1];

			// UV 값 반환.
			return texCoord;
		}

		// UV 속성이 정점 유형인 경우.
		case FbxGeometryElement::eByPolygonVertex:
		{
			// 현재 UV 값을 읽어올 인덱스 얻어오기. 삼항연산자.
			int index = isUsingIndex == true ? vertexUV->GetIndexArray().GetAt(vertexCounter) : vertexCounter;

			/*if (isUsingIndex == true)
			index = vertexUV->GetIndexArray().GetAt(vertexCounter);
			else
			index = vertexCounter;*/

			// UV 값 읽어오기. (DirectX와 FBX의 UV V좌표는 서로 반대).
			texCoord.x = (float)vertexUV->GetDirectArray().GetAt(index).mData[0];
			texCoord.y = 1.0f - (float)vertexUV->GetDirectArray().GetAt(index).mData[1];

			// UV 값 반환.
			return texCoord;
		}

		default:
		{
			MessageBox(NULL, L"UV 값이 유효하지 않습니다", L"오류", MB_OK);
			return NULL;
		}
		}


		return NULL;
	}

	XMFLOAT3 FBXLoader::ReadTangent(FbxMesh * pMesh, int vertexIndex, int vertexCounter)
	{
		// UV가 있는지 확인.
		if (pMesh->GetElementTangentCount() < 1)
		{
			MessageBox(NULL, L"Tangent 값이 유효하지 않습니다", L"오류", MB_OK);
			return NULL;
		}

		// 리턴용 변수 선언.
		XMFLOAT3 tangent(0.0f, 0.0f, 0.0f);
		// UV 전체 배열 얻어오기.
		FbxGeometryElementTangent* vertexTangent = pMesh->GetElementTangent(0);
		const bool isUsingIndex = vertexTangent->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int indexCount = isUsingIndex ? vertexTangent->GetIndexArray().GetCount() : 0;

		switch (vertexTangent->GetMappingMode())
		{
			// UV 속성이 제어점 유형인 경우.
		case FbxGeometryElement::eByControlPoint:
		{
			// 현재 UV 값을 읽어올 인덱스 얻어오기.
			int index = isUsingIndex ? vertexTangent->GetIndexArray().GetAt(vertexIndex) : vertexIndex;

			// UV 값 읽어오기. (DirectX와 FBX의 UV V좌표는 서로 반대).
			tangent.x = (float)vertexTangent->GetDirectArray().GetAt(index).mData[0];
			tangent.y = (float)vertexTangent->GetDirectArray().GetAt(index).mData[1];
			tangent.z = (float)vertexTangent->GetDirectArray().GetAt(index).mData[2];

			// UV 값 반환.
			return tangent;
		}

		// UV 속성이 정점 유형인 경우.
		case FbxGeometryElement::eByPolygonVertex:
		{
			// 현재 UV 값을 읽어올 인덱스 얻어오기. 삼항연산자.
			int index = isUsingIndex == true ? vertexTangent->GetIndexArray().GetAt(vertexCounter) : vertexCounter;

			// UV 값 읽어오기. (DirectX와 FBX의 UV V좌표는 서로 반대).
			tangent.x = (float)vertexTangent->GetDirectArray().GetAt(index).mData[0];
			tangent.y = (float)vertexTangent->GetDirectArray().GetAt(index).mData[1];
			tangent.z = (float)vertexTangent->GetDirectArray().GetAt(index).mData[2];

			// UV 값 반환.
			return tangent;
		}

		default:
		{
			MessageBox(NULL, L"Tangent 값이 유효하지 않습니다", L"오류", MB_OK);
			return NULL;
		}
		}

		return NULL;
	}

	XMFLOAT3 FBXLoader::ReadBinormal(FbxMesh * pMesh, int vertexIndex, int vertexCounter)
	{
		// UV가 있는지 확인.
		if (pMesh->GetElementBinormalCount() < 1)
		{
			MessageBox(NULL, L"Binormal 값이 유효하지 않습니다", L"오류", MB_OK);
			return NULL;
		}

		// 리턴용 변수 선언.
		XMFLOAT3 binormal(0.0f, 0.0f, 0.0f);
		// UV 전체 배열 얻어오기.
		FbxGeometryElementBinormal* vertexBinormal = pMesh->GetElementBinormal(0);
		const bool isUsingIndex = vertexBinormal->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int indexCount = isUsingIndex ? vertexBinormal->GetIndexArray().GetCount() : 0;

		switch (vertexBinormal->GetMappingMode())
		{
			// UV 속성이 제어점 유형인 경우.
		case FbxGeometryElement::eByControlPoint:
		{
			// 현재 UV 값을 읽어올 인덱스 얻어오기.
			int index = isUsingIndex ? vertexBinormal->GetIndexArray().GetAt(vertexIndex) : vertexIndex;

			// UV 값 읽어오기. (DirectX와 FBX의 UV V좌표는 서로 반대).
			binormal.x = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[0];
			binormal.y = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[1];
			binormal.z = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[2];

			// UV 값 반환.
			return binormal;
		}

		// UV 속성이 정점 유형인 경우.
		case FbxGeometryElement::eByPolygonVertex:
		{
			// 현재 UV 값을 읽어올 인덱스 얻어오기. 삼항연산자.
			int index = isUsingIndex == true ? vertexBinormal->GetIndexArray().GetAt(vertexCounter) : vertexCounter;

			// UV 값 읽어오기. (DirectX와 FBX의 UV V좌표는 서로 반대).
			binormal.x = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[0];
			binormal.y = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[1];
			binormal.z = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[2];

			// UV 값 반환.
			return binormal;
		}

		default:
		{
			MessageBox(NULL, L"Binormal 값이 유효하지 않습니다", L"오류", MB_OK);
			return NULL;
		}
		}

		return NULL;
	}
}