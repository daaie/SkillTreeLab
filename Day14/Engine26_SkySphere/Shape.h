#pragma once
#include "DXUtil.h"
#include "ShaderUtil.h"

namespace Shape
{
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
		XMFLOAT3 biNormal;
		XMFLOAT3 tangent;

		Vertex() { }
		Vertex(XMFLOAT3 pos) : position(pos) { }
		Vertex(XMFLOAT3 pos, XMFLOAT2 uv) : position(pos), texCoord(uv) { }
		Vertex(XMFLOAT3 pos, XMFLOAT2 uv, XMFLOAT3 normal) : position(pos), texCoord(uv), normal(normal) { }
	};

	class Mesh
	{
	public:

		LPCSTR fbxName;
		std::vector<Vertex> vertices;
		std::vector<DWORD> indices;
		D3D11_INPUT_ELEMENT_DESC inputLayout[5] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// Buffer.
		ID3D11Buffer*		vertexBuffer;			// 정점 버퍼.
		ID3D11Buffer*		indexBuffer;			// 인덱스 버퍼.
		ID3D11InputLayout*	vertexInputLayout;		// 정점 입력 레이아웃.

		// Shader.
		ShaderUtil::VertexShader vertexShader;		// 정점 쉐이더.
		ShaderUtil::PixelShader pixelShader;		// 픽셀 쉐이더.

		// 위치/회전/스케일.
		XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		XMFLOAT3 rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
		XMFLOAT3 scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	public:

		bool InitMesh();

		bool CreateVertexBuffer(ID3D11Device* pDevice);
		void BindVertexBuffer(ID3D11DeviceContext* pDeviceContext);
		
		bool CreateIndexBuffer(ID3D11Device* pDevice);
		void BindIndexBuffer(ID3D11DeviceContext* pDeviceContext);

		bool CreateInputLayout(ID3D11Device* pDevice);
		void BindInputLayout(ID3D11DeviceContext* pDeviceContext);

		int GetVerticesCount() const { return vertices.size(); }
		int GetIndicesCount() const { return indices.size(); }

		void DrawMesh(ID3D11DeviceContext* pDeviceContext);

		void Release();

		void SetVertices(std::vector<Vertex> inVertices)
		{
			for (int ix = 0; ix < inVertices.size(); ++ix)
				vertices.push_back(inVertices[ix]);
		}

		void SetIndices(std::vector<DWORD> inIndices)
		{
			for (int ix = 0; ix < inIndices.size(); ++ix)
				indices.push_back(inIndices[ix]);
		}
	};

	void CreatePlane(Mesh* pOutMesh);

	/*ShapeVertex* CreateCube();
	ShapeVertex* CreateSphere();*/
}