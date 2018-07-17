#include "ShaderUtil.h"

bool ShaderUtil::CompileShader(LPCWSTR shader, LPCSTR entry, LPCSTR profile, ID3DBlob ** pOutShaderBuffer)
{
	HRESULT hr;
	hr = D3DX11CompileFromFile(shader, NULL, NULL, entry, profile, NULL, NULL, NULL, pOutShaderBuffer, NULL, NULL);
	if (FAILED(hr))
	{
		OutputDebugString(L"���̴� ������ ����");
		OutputDebugString(shader);
		OutputDebugString(L"\n");
		return false;
	}

	return true;
}

bool ShaderUtil::CreateVertexShader(ID3D11Device * pDevice, const void * pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage * pClassLinkage, ID3D11VertexShader ** ppVertexShader)
{
	// ���� ���̴� ������Ʈ ����.
	HRESULT hr = pDevice->CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);
	if (FAILED(hr))
	{
		OutputDebugString(L"���ؽ� ���̴� ���� ����");
		OutputDebugString(L"\n");
		return false;
	}

	return true;
}

bool ShaderUtil::BindVertexShader(ID3D11DeviceContext * pDeviceContext, ID3D11VertexShader ** ppVertexShader)
{
	//// ���� ���̴� ������Ʈ ����.
	//HRESULT hr = pDevice->CreateVertexShader(pShaderBytecode, BytecodeLength, NULL, ppVertexShader);
	//if (FAILED(hr))
	//{
	//	OutputDebugString(L"���ؽ� ���̴� ���ε� ����");
	//	OutputDebugString(L"\n");
	//	return false;
	//}

	// ���� ���̴� ����.(���ε�).
	pDeviceContext->VSSetShader(*ppVertexShader, NULL, NULL);

	return true;
}

bool ShaderUtil::CreatePixelShader(ID3D11Device * pDevice, const void * pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage * pClassLinkage, ID3D11PixelShader ** ppPixelShader)
{
	// �ȼ� ���̴� ������Ʈ ����.
	HRESULT hr = pDevice->CreatePixelShader(pShaderBytecode, BytecodeLength, NULL, ppPixelShader);
	if (FAILED(hr))
	{
		OutputDebugString(L"�ȼ� ���̴� ���ε� ����");
		OutputDebugString(L"\n");
		return false;
	}

	return true;
}

bool ShaderUtil::BindPixelShader(ID3D11DeviceContext * pDeviceContext, ID3D11PixelShader ** ppPixelShader)
{
	//// �ȼ� ���̴� ������Ʈ ����.
	//HRESULT hr = pDevice->CreatePixelShader(pShaderBytecode, BytecodeLength, NULL, ppPixelShader);
	//if (FAILED(hr))
	//{
	//	OutputDebugString(L"�ȼ� ���̴� ���ε� ����");
	//	OutputDebugString(L"\n");
	//	return false;
	//}

	// �ȼ� ���̴� ����.(���ε�).
	pDeviceContext->PSSetShader(*ppPixelShader, NULL, NULL);

	return true;
}