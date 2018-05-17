#pragma once
#include "DXUtil.h"
#include "Shader.h"

namespace ShaderUtil
{
	// ���̴� ������.
	bool CompileShader(LPCWSTR shader, LPCSTR entry, LPCSTR profile, ID3DBlob** pOutShaderBuffer);

	// ���ؽ� ���̴� ������Ʈ ����.
	bool CreateVertexShader(ID3D11Device * pDevice, const void * pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage * pClassLinkage, ID3D11VertexShader ** ppVertexShader);

	// ���ؽ� ���̴� ���ε�.
	bool BindVertexShader(ID3D11DeviceContext * pDeviceContext, ID3D11VertexShader ** ppVertexShader);

	// �ȼ� ���̴� ������Ʈ ����.
	bool CreatePixelShader(ID3D11Device * pDevice, const void * pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage * pClassLinkage, ID3D11PixelShader** ppPixelShader);

	// �ȼ� ���̴� ���ε�.
	bool BindPixelShader(ID3D11DeviceContext* pDeviceContext, ID3D11PixelShader **ppPixelShader);
}