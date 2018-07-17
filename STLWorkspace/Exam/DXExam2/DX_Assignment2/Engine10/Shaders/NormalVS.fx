// ��� ���� ����.
cbuffer CBPerObject : register(b0)
{
	matrix world;		// ���� ���.
	matrix view;		// �� ���.
	matrix projection;	// ���� ���.
};

// ��� ���� - ����Ʈ ����.
cbuffer CBLight : register(b1)
{
	float3 worldLightPosition;
	float3 worldCameraPosition;
};

// ���� �Է� ����ü.
struct VS_INPUT
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
};

// ���� ��� ����ü.
struct VS_OUTPUT
{	
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 lightDir : TEXCOORD1;
	float3 viewDir : TEXCOORD2;
	float3 T : TEXCOORD3;
	float3 B : TEXCOORD4;
	float3 N : TEXCOORD5;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	float4 worldPosition = mul(input.pos, world);
	output.pos = worldPosition;
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	output.uv = input.uv;

	output.lightDir = normalize(worldPosition.xyz - worldLightPosition);
	output.viewDir = normalize(worldPosition.xyz - worldCameraPosition);

	float3 worldNormal = mul(input.normal, (float3x3)world);
	output.N = normalize(worldNormal);

	float3 worldTangent = mul(input.tangent, (float3x3)world);
	output.T = normalize(normalize(worldTangent));

	float3 worldBinormal = mul(input.binormal, (float3x3)world);
	output.B = normalize(normalize(worldBinormal));

	return output;
}