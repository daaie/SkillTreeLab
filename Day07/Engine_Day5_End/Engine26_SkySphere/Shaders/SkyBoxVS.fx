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

struct VS_INPUT
{
	float3 pos : POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	
	output.pos = mul(float4(input.pos, 1.0f), world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	// xyzw�� �ƴ϶� xyww�� �����ؼ� ���� ���� ī�޶󿡼� �׻� ���� �ְ� ����.
	output.pos = output.pos.xyww;

	output.texCoord = input.pos;

	return output;
}