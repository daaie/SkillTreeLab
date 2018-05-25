#pragma once
#include "DXUtil.h"
#include "Shader.h"

namespace ShaderUtil
{
	// ���̴� ������.
	bool CompileShader(LPCWSTR shader, LPCSTR entry, LPCSTR profile, ID3DBlob** pOutShaderBuffer);

	// ���ؽ� ���̴� ���ε�.
	bool BindVertexShader(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pDeviceContext,
		const void *pShaderBytecode,
		SIZE_T BytecodeLength,
		ID3D11ClassLinkage *pClassLinkage,
		ID3D11VertexShader **ppVertexShader);

	// �ȼ� ���̴� ���ε�.
	bool BindPixelShader(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pDeviceContext,
		const void *pShaderBytecode,
		SIZE_T BytecodeLength,
		ID3D11ClassLinkage *pClassLinkage,
		ID3D11PixelShader **ppPixelShader);
}