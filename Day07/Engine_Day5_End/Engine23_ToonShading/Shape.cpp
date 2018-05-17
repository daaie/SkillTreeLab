#include "Shape.h"

void Shape::CreatePlane(Mesh* pOutMesh)
{
	std::vector<Vertex> plane;
	plane.push_back(Vertex(XMFLOAT3(-10.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(-1.0f, 1.0f, -1.0f)));
	plane.push_back(Vertex(XMFLOAT3(-10.0f, -1.0f, 10.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 1.0f, 1.0f)));
	plane.push_back(Vertex(XMFLOAT3(10.0f, -1.0f, 10.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)));
	plane.push_back(Vertex(XMFLOAT3(10.0f, -1.0f, -10.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(1.0f, 1.0f, -1.0f)));

	std::vector<DWORD> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	pOutMesh->SetVertices(plane);
	pOutMesh->SetIndices(indices);
}

bool Shape::Mesh::InitMesh()
{
	return true;
}

bool Shape::Mesh::CreateVertexBuffer(ID3D11Device* pDevice)
{
	// 정점 개수 저장.
	int nVertices = vertices.size();

	// 버텍스 버퍼 서술자(Descriptor).
	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.ByteWidth = sizeof(Vertex) * nVertices;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;

	// 서브 리소스 생성.
	D3D11_SUBRESOURCE_DATA vbData;
	ZeroMemory(&vbData, sizeof(D3D11_SUBRESOURCE_DATA));
	vbData.pSysMem = &vertices[0];

	// 정점 버퍼 생성.
	if (FAILED(pDevice->CreateBuffer(&vbDesc, &vbData, &vertexBuffer)))
	{
		MessageBox(NULL, L"버텍스 버퍼 생성 실패", L"오류", MB_OK);
		OutputDebugString(L"버텍스 버퍼 생성 실패");
		OutputDebugString(L"\n");
		return false;
	}	

	return true;
}

void Shape::Mesh::BindVertexBuffer(ID3D11DeviceContext * pDeviceContext)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// 정점 버퍼 바인딩.
	pDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
}

bool Shape::Mesh::CreateIndexBuffer(ID3D11Device * pDevice)
{
	// 인덱스 개수 저장.
	int nIndices = indices.size();

	// 인덱스 버퍼 서술자(Descriptor).
	D3D11_BUFFER_DESC ibDesc;
	ZeroMemory(&ibDesc, sizeof(D3D11_BUFFER_DESC));
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.ByteWidth = sizeof(DWORD) * nIndices;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;

	// 서브 리소스 생성.
	D3D11_SUBRESOURCE_DATA ibData;
	ZeroMemory(&ibData, sizeof(D3D11_SUBRESOURCE_DATA));
	ibData.pSysMem = &indices[0];

	// 인덱스 버퍼 생성.
	if (FAILED(pDevice->CreateBuffer(&ibDesc, &ibData, &indexBuffer)))
	{
		MessageBox(NULL, L"인덱스 버퍼 생성 실패", L"오류", MB_OK);
		OutputDebugString(L"인덱스 버퍼 생성 실패");
		OutputDebugString(L"\n");
		return false;
	}

	return true;
}

void Shape::Mesh::BindIndexBuffer(ID3D11DeviceContext * pDeviceContext)
{
	// 인덱스 버퍼 바인딩.
	pDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

bool Shape::Mesh::CreateInputLayout(ID3D11Device * pDevice)
{
	int nSize = ARRAYSIZE(inputLayout);

	// 입력 레이아웃 생성.
	HRESULT hr = pDevice->CreateInputLayout(inputLayout, nSize,
		vertexShader.pShaderBuffer->GetBufferPointer(),
		vertexShader.pShaderBuffer->GetBufferSize(), &vertexInputLayout);

	if (FAILED(hr))
	{
		OutputDebugString(L"입력 레이아웃 생성 실패");
		OutputDebugString(L"\n");

		MessageBox(NULL, L"입력 레이아웃 생성 실패", L"오류", MB_OK);
		return false;
	}

	return true;
}

void Shape::Mesh::BindInputLayout(ID3D11DeviceContext * pDeviceContext)
{
	// 입력 레이아웃 바인딩.
	pDeviceContext->IASetInputLayout(vertexInputLayout);
}

void Shape::Mesh::DrawMesh(ID3D11DeviceContext * pDeviceContext)
{
	pDeviceContext->DrawIndexed(indices.size(), 0, 0);
}

void Shape::Mesh::Release()
{
	Memory::SafeRelease(vertexBuffer);
	Memory::SafeRelease(indexBuffer);
	Memory::SafeRelease(vertexInputLayout);

	vertexShader.Release();
	pixelShader.Release();
}
