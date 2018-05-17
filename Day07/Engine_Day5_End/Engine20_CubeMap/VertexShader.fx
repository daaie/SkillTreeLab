// 상수 버퍼 선언.
cbuffer CBPerObject : register(b0)
{
	matrix world;		// 월드 행렬.
	matrix view;		// 뷰 행렬.
	matrix projection;	// 투영 행렬.
};

// 상수 버퍼 - 라이트 정보.
cbuffer CBLight : register(b1) // 레지스터 안해주면 알아서순서대로해줌
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
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

// 정점 출력 구조체.
struct VS_OUTPUT
{	
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 lightDir : TEXCOORD1; 
	// 픽셀쉐이더에서 반사벡터를 구하기위해선 빛방향이랑 노멀필요하ㅏㅁ.

	float3 viewDir : TEXCOORD2;
	// float3 reflection : TEXCOORD3;
	// 노멀이 텍스쳐라서 이걸 픽셀쉐이더로 넘겨야함..
	// 그래서 반사벡터를 구할수가없다 지웠음.

	float3 T : TEXCOORD3;
	float3 B : TEXCOORD4;
	float3 N : TEXCOORD5; // 시멘틱이업엉..그래서 텍스쳐 좌표 씀

};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	float4 worldPos = mul(input.pos, world);
	output.pos = worldPos;

	// 뷰 변환/ 투영 변환
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	output.uv = input.uv;

	//빛 방향 설정
	output.lightDir = normalize(worldPos - worldLightPosition.xyz);
	//카메라 방향 설정
	output.viewDir = normalize(worldPos - worldCameraPosition.xyz);
	
	// 월드 공간 기준 노멀 계산
	float3 worldNormal = mul(input.normal, (float3x3)world);
	output.N = normalize(worldNormal);

	// 월드 공간 기준 바이 노멀 계산
	float3 worldBinormal = mul(input.binormal, (float3x3)world);
	output.B = normalize(worldBinormal);

	// 월드 공간 기준 탄젠트 계산
	float3 worldTangent = mul(input.tangent, (float3x3)world);
	output.T = normalize(worldTangent);

	// 보통 TBN 순서 노멀이 z니까 -> 3x3행렬

	return output;
}
