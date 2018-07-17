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

// 정점 입력 구조체.
struct VS_INPUT
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
};

// 정점 출력 구조체.
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 viewDir : TEXCOORD1;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	output.pos = mul(input.pos, world);
	output.viewDir = normalize(output.pos.xyz - worldCameraPosition);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	return output;
}