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

	// �߰�.
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

// ���� ��� ����ü.
struct VS_OUTPUT
{	
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 lightDir : TEXCOORD1;
	float3 viewDir : TEXCOORD2;
	//float3 reflection : TEXCOORD3;

	// �߰�.
	float3 T : TEXCOORD3;
	float3 B : TEXCOORD4;
	float3 N : TEXCOORD5;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	// ���� ���� ������ ��ġ ��ȯ.
	float4 worldPos = mul(input.pos, world);
	output.pos = worldPos;
	// �亯ȯ/������ȯ.
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	// UV���� �״�� ����.
	output.uv = input.uv;

	// �� ���� ���ϱ�.
	output.lightDir = normalize(worldPos - worldLightPosition);

	// ī�޶� ���� ���ϱ�.
	output.viewDir = normalize(worldPos - worldCameraPosition);

	// ���� ���� ���� ��� ���.
	float3 worldNormal = mul(input.normal, (float3x3)world);
	output.N = normalize(worldNormal);

	// ���� ���� ���� ���̳�� ���.
	float3 worldBinormal = mul(input.binormal, (float3x3)world);
	output.B = normalize(worldBinormal);

	// ���� ���� ���� ź��Ʈ ���.
	float3 worldTangent = mul(input.tangent, (float3x3)world);
	output.T = normalize(worldTangent);
	
	return output;
}