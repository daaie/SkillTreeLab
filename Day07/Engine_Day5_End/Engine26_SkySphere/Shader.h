#pragma once
#include "ShaderUtil.h"

namespace ShaderUtil
{
	// ��� ���ۿ� ����ü ����.
	struct CBPerObject
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	// ����Ʈ ó���� ���� ����ü ����.
	struct CBLight
	{
		// ���� ��ġ(3���� ��ġ ��).
		XMVECTOR lightPosition;
		XMVECTOR cameraPosition;
	};

	struct Texture
	{
		LPCWSTR fileName;
		ID3D11ShaderResourceView* pTextureResource = NULL;
	};

	class Shader
	{
	public:

		LPCWSTR fileName;
		LPCSTR functionName;
		LPCSTR profile;
		ID3DBlob* pShaderBuffer = NULL;

		// ��� ���� ������ ����� ����.
		CBPerObject cbWVP;
		CBLight cbLight;

		// ��� ����.
		ID3D11Buffer* cbBuffer;

		// �ؽ�ó.
		std::vector<Texture> pTextures;

		// ���÷� ������Ʈ.
		ID3D11SamplerState* pSamplerState;

		Shader() { }
		Shader(LPCWSTR fileName, LPCSTR functionName, LPCSTR profile);
		virtual ~Shader();

	public:

		Shader * GetShader() { return this; }
		LPCWSTR GetFileName() const;
		LPCSTR GetEntryName() const;
		LPCSTR GetProfile() const;
		
		ID3DBlob* GetShaderBuffer();
		
		virtual bool CompileShader() { return true; };
		virtual bool CreateShader(ID3D11Device* pDevice) { return true; };
		virtual bool BindShader(ID3D11DeviceContext* pDeviceContext) { return true; };
		
		bool CreateSamplerState(ID3D11Device* pDevice);
		void SetSamplerState(ID3D11DeviceContext* pDeviceContext);

		bool LoadTextures(ID3D11Device* pDevice);
		void SetTextures(ID3D11DeviceContext* pDeviceContext);

		virtual void Release() { Memory::SafeRelease(pShaderBuffer); }
	};

	class VertexShader : public Shader
	{
	public:

		ID3D11VertexShader * pVertexShader = NULL;

		VertexShader();
		VertexShader(LPCWSTR fileName, LPCSTR functionName, LPCSTR profile);
		~VertexShader();

		bool CompileShader() override;
		virtual bool CreateShader(ID3D11Device* pDevice) override;
		virtual bool BindShader(ID3D11DeviceContext* pDeviceContext) override;
		virtual void Release() override;
	};

	class PixelShader : public Shader
	{
	public:

		ID3D11PixelShader * pPixelShader = NULL;

		PixelShader();
		PixelShader(LPCWSTR fileName, LPCSTR functionName, LPCSTR profile);
		~PixelShader();

		bool CompileShader() override;
		virtual bool CreateShader(ID3D11Device* pDevice) override;
		virtual bool BindShader(ID3D11DeviceContext* pDeviceContext) override;
		virtual void Release() override;
	};
}