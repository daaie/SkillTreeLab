// 상수 버퍼 선언.
cbuffer CBPerObject : register(b0)
{
	matrix world;		// 월드 행렬.
	matrix view;		// 뷰 행렬.
	matrix projection;	// 투영 행렬.
};

// 상수 버퍼 - 라이트 정보.
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

	// xyzw가 아니라 xyww로 설정해서 뎁스 값을 카메라에서 항상 가장 멀게 설정.
	output.pos = output.pos.xyww;

	output.texCoord = input.pos;

	return output;
}