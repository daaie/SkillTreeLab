#include "Shader.h"

ShaderUtil::Shader::Shader(LPCWSTR fileName, LPCSTR functionName, LPCSTR profile)
{
	this->fileName = fileName;
	this->functionName = functionName;
	this->profile = profile;
	pShaderBuffer = NULL;
}

ShaderUtil::Shader::~Shader()
{
}

LPCWSTR ShaderUtil::Shader::GetFileName() const
{
	return fileName;
}

LPCSTR ShaderUtil::Shader::GetEntryName() const
{
	return functionName;
}

LPCSTR ShaderUtil::Shader::GetProfile() const
{
	return profile;
}

ID3DBlob * ShaderUtil::Shader::GetShaderBuffer()
{
	return pShaderBuffer;
}

bool ShaderUtil::Shader::CreateRasterizerState(ID3D11Device * pDevice, D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode)
{
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode = fillMode;
	rsDesc.CullMode = cullMode;

	// 래스터라이저 상태 생성.
	if (FAILED(pDevice->CreateRasterizerState(&rsDesc, &rsState)))
	{
		OutputDebugString(L"래스터라이저 상태 생성 실패");
		OutputDebugString(L"\n");
		return false;
	}
	
	return true;
}

void ShaderUtil::Shader::BindRasterizerState(ID3D11DeviceContext * pDeviceContext)
{
	// 래스터라이저 스테이트 바인딩.
	pDeviceContext->RSSetState(rsState);
}

bool ShaderUtil::Shader::CreateBlendState(ID3D11Device * pDevice)
{
	// 블렌드 스테이드 서술자 설정.
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// 블렌드 스테이트 생성.
	if (FAILED(pDevice->CreateBlendState(&blendDesc, &alphaBlendState)))
	{
		OutputDebugString(L"블렌드 스테이트 생성 실패\n");
		return false;
	}

	return true;
}

void ShaderUtil::Shader::BindBlendState(ID3D11DeviceContext * pDeviceContext)
{
	// 블렌드 스테이트 바인딩.
	pDeviceContext->OMSetBlendState(alphaBlendState, 0, 0xffffffff);
}

void ShaderUtil::Shader::Release()
{
	Memory::SafeRelease(pShaderBuffer);
	Memory::SafeRelease(cbBuffer);
	Memory::SafeRelease(rsState);
	Memory::SafeRelease(alphaBlendState);
}

ShaderUtil::VertexShader::VertexShader() : Shader()
{
	pVertexShader = NULL;
}

ShaderUtil::VertexShader::VertexShader(LPCWSTR fileName, LPCSTR functionName, LPCSTR profile)
	: Shader(fileName, functionName, profile)
{
	pVertexShader = NULL;
}

ShaderUtil::VertexShader::~VertexShader()
{
	
}

bool ShaderUtil::VertexShader::CompileShader()
{
	if (!ShaderUtil::CompileShader(fileName, functionName, profile, &pShaderBuffer))
	{
		OutputDebugString(L"정점 쉐이더 컴파일 실패");
		OutputDebugString(L"\n");
		return false;
	}

	return true;
}

bool ShaderUtil::VertexShader::CreateShader(ID3D11Device * pDevice)
{
	if (!ShaderUtil::CreateVertexShader(pDevice, pShaderBuffer->GetBufferPointer(),
		pShaderBuffer->GetBufferSize(), NULL, &pVertexShader))
	{
		OutputDebugString(L"정점 쉐이더 생성 실패");
		OutputDebugString(L"\n");
		return false;
	}	

	return true;
}

bool ShaderUtil::VertexShader::BindShader(ID3D11DeviceContext* pDeviceContext)
{
	if (!ShaderUtil::BindVertexShader(pDeviceContext, &pVertexShader))
	{
		OutputDebugString(L"버텍스 쉐이더 바인딩 실패");
		OutputDebugString(L"\n");
		return false;
	}

	return true;
}

void ShaderUtil::VertexShader::Release()
{
	Shader::Release();
	Memory::SafeRelease(pVertexShader);
}

ShaderUtil::PixelShader::PixelShader() : Shader()
{
	pPixelShader = NULL;
}

ShaderUtil::PixelShader::PixelShader(LPCWSTR fileName, LPCSTR functionName, LPCSTR profile)
	: Shader(fileName, functionName, profile)
{
	pPixelShader = NULL;
}

ShaderUtil::PixelShader::~PixelShader()
{
	
}

bool ShaderUtil::PixelShader::CompileShader()
{
	if (!ShaderUtil::CompileShader(fileName, functionName, profile, &pShaderBuffer))
	{
		OutputDebugString(L"픽셀 쉐이더 컴파일 실패");
		OutputDebugString(L"\n");
		return false;
	}

	return true;
}

bool ShaderUtil::PixelShader::CreateShader(ID3D11Device * pDevice)
{
	if (!ShaderUtil::CreatePixelShader(pDevice, pShaderBuffer->GetBufferPointer(),
		pShaderBuffer->GetBufferSize(), NULL, &pPixelShader))
	{
		OutputDebugString(L"픽셀 쉐이더 생성 실패");
		OutputDebugString(L"\n");
		return false;
	}	

	return true;
}

bool ShaderUtil::PixelShader::BindShader(ID3D11DeviceContext* pDeviceContext)
{
	if (!ShaderUtil::BindPixelShader(pDeviceContext, &pPixelShader))
	{
		OutputDebugString(L"픽셀 쉐이더 바인딩 실패");
		OutputDebugString(L"\n");
		return false;
	}	

	return true;
}

bool ShaderUtil::PixelShader::LoadTextures(ID3D11Device* pDevice)
{
	for (int ix = 0; ix < pTextures.size(); ++ix)
	{
		HRESULT hr;
		// 텍스처 파일 로드.
		hr = D3DX11CreateShaderResourceViewFromFile(pDevice, pTextures[ix].fileName, NULL, NULL,
			&pTextures[ix].pTextureResource, NULL);
		if (FAILED(hr))
		{
			OutputDebugString(L"텍스처 로드 실패");
			OutputDebugString(fileName);
			OutputDebugString(L"\n");

			return false;
		}
	}

	return true;
}

void ShaderUtil::PixelShader::SetTextures(ID3D11DeviceContext * pDeviceContext)
{
	int textureIndex = 0;
	for (int ix = 0; ix < pTextures.size(); ++ix)
	{
		pDeviceContext->PSSetShaderResources(textureIndex, 1, &pTextures[ix].pTextureResource);
		++textureIndex;
	}
}

bool ShaderUtil::PixelShader::CreateSamplerState(ID3D11Device* pDevice)
{
	// 샘플러 상태 서술자 설정.
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// 샘플러 상태(State) 생성.
	HRESULT hr = pDevice->CreateSamplerState(&samplerDesc, &pSamplerState);
	if (FAILED(hr))
	{
		OutputDebugString(L"샘플러 스테이트 생성 실패");
		OutputDebugString(L"\n");

		return false;
	}

	return true;
}

void ShaderUtil::PixelShader::SetSamplerState(ID3D11DeviceContext * pDeviceContext)
{
	pDeviceContext->PSSetSamplers(0, 1, &pSamplerState);
}

void ShaderUtil::PixelShader::Release()
{
	Shader::Release();
	Memory::SafeRelease(pPixelShader);
}