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

	// 추가.
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

// 정점 출력 구조체.
struct VS_OUTPUT
{	
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 lightDir : TEXCOORD1;
	float3 viewDir : TEXCOORD2;
	//float3 reflection : TEXCOORD3;

	// 추가.
	float3 T : TEXCOORD3;
	float3 B : TEXCOORD4;
	float3 N : TEXCOORD5;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	// 월드 공간 기준의 위치 변환.
	float4 worldPos = mul(input.pos, world);
	output.pos = worldPos;
	// 뷰변환/투영변환.
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	// UV값은 그대로 전달.
	output.uv = input.uv;

	// 빛 방향 구하기.
	output.lightDir = normalize(worldPos - worldLightPosition);

	// 카메라 방향 구하기.
	output.viewDir = normalize(worldPos - worldCameraPosition);

	// 월드 공간 기준 노멀 계산.
	float3 worldNormal = mul(input.normal, (float3x3)world);
	output.N = normalize(worldNormal);

	// 월드 공간 기준 바이노멀 계산.
	float3 worldBinormal = mul(input.binormal, (float3x3)world);
	output.B = normalize(worldBinormal);

	// 월드 공간 기준 탄젠트 계산.
	float3 worldTangent = mul(input.tangent, (float3x3)world);
	output.T = normalize(worldTangent);
	
	return output;
}