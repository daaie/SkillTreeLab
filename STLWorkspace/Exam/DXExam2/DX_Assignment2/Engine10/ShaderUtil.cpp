#include "ShaderUtil.h"

bool ShaderUtil::CompileShader(LPCWSTR shader, LPCSTR entry, LPCSTR profile, ID3DBlob ** pOutShaderBuffer)
{
	HRESULT hr;
	hr = D3DX11CompileFromFile(shader, NULL, NULL, entry, profile, NULL, NULL, NULL, pOutShaderBuffer, NULL, NULL);
	if (FAILED(hr))
	{
		OutputDebugString(L"쉐이더 컴파일 오류");
		OutputDebugString(shader);
		OutputDebugString(L"\n");
		return false;
	}

	return true;
}

bool ShaderUtil::CreateVertexShader(ID3D11Device * pDevice, const void * pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage * pClassLinkage, ID3D11VertexShader ** ppVertexShader)
{
	// 정점 쉐이더 오브젝트 생성.
	HRESULT hr = pDevice->CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);
	if (FAILED(hr))
	{
		OutputDebugString(L"버텍스 쉐이더 생성 오류");
		OutputDebugString(L"\n");
		return false;
	}

	return true;
}

bool ShaderUtil::BindVertexShader(ID3D11DeviceContext * pDeviceContext, ID3D11VertexShader ** ppVertexShader)
{
	//// 정점 쉐이더 오브젝트 생성.
	//HRESULT hr = pDevice->CreateVertexShader(pShaderBytecode, BytecodeLength, NULL, ppVertexShader);
	//if (FAILED(hr))
	//{
	//	OutputDebugString(L"버텍스 쉐이더 바인딩 오류");
	//	OutputDebugString(L"\n");
	//	return false;
	//}

	// 정점 쉐이더 설정.(바인딩).
	pDeviceContext->VSSetShader(*ppVertexShader, NULL, NULL);

	return true;
}

bool ShaderUtil::CreatePixelShader(ID3D11Device * pDevice, const void * pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage * pClassLinkage, ID3D11PixelShader ** ppPixelShader)
{
	// 픽셀 쉐이더 오브젝트 생성.
	HRESULT hr = pDevice->CreatePixelShader(pShaderBytecode, BytecodeLength, NULL, ppPixelShader);
	if (FAILED(hr))
	{
		OutputDebugString(L"픽셀 쉐이더 바인딩 오류");
		OutputDebugString(L"\n");
		return false;
	}

	return true;
}

bool ShaderUtil::BindPixelShader(ID3D11DeviceContext * pDeviceContext, ID3D11PixelShader ** ppPixelShader)
{
	//// 픽셀 쉐이더 오브젝트 생성.
	//HRESULT hr = pDevice->CreatePixelShader(pShaderBytecode, BytecodeLength, NULL, ppPixelShader);
	//if (FAILED(hr))
	//{
	//	OutputDebugString(L"픽셀 쉐이더 바인딩 오류");
	//	OutputDebugString(L"\n");
	//	return false;
	//}

	// 픽셀 쉐이더 설정.(바인딩).
	pDeviceContext->PSSetShader(*ppPixelShader, NULL, NULL);

	return true;
}