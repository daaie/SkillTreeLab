#pragma once
#include "DXUtil.h"
#include "Shader.h"

namespace ShaderUtil
{
	// 쉐이더 컴파일.
	bool CompileShader(LPCWSTR shader, LPCSTR entry, LPCSTR profile, ID3DBlob** pOutShaderBuffer);

	// 버텍스 쉐이더 오브젝트 생성.
	bool CreateVertexShader(ID3D11Device * pDevice, const void * pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage * pClassLinkage, ID3D11VertexShader ** ppVertexShader);

	// 버텍스 쉐이더 바인딩.
	bool BindVertexShader(ID3D11DeviceContext * pDeviceContext, ID3D11VertexShader ** ppVertexShader);

	// 픽셀 쉐이더 오브젝트 생성.
	bool CreatePixelShader(ID3D11Device * pDevice, const void * pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage * pClassLinkage, ID3D11PixelShader** ppPixelShader);

	// 픽셀 쉐이더 바인딩.
	bool BindPixelShader(ID3D11DeviceContext* pDeviceContext, ID3D11PixelShader **ppPixelShader);
}