//행렬 넘겨받기 -> 씨멘틱 / 상수버퍼(dx)
cbuffer CBPerObject
{
	matrix world;	// 월드행렬
	matrix view;	// 뷰 행렬
	matrix projection; // 투영 행렬
};

struct VS_INPUT
{
	float4 pos : POSITION;
	//float4 color : COLOR;
	float2 texCoord : TEXCOORD0;
	//float2 texCoord2 : TEXCOORD1; // 요런식으로 여러개 넘겨줄솓있음
};

Texture2D objTexture;
SamplerState objSampler;

struct VS_OUTPUT
{
	float4 pos : SV_POSITION; // SV여야 파이프라인이인식함
	//float4 color : COLOR;
	float2 texCoord : TEXCOORD0;
};

VS_OUTPUT VS_Main(VS_INPUT input)
{
	VS_OUTPUT output;

	// 공간 변환 ( world, view, projection 곱함)
	//output.pos = input.pos;

	// 월드 변환
	output.pos = mul(input.pos, world);
	// 뷰 변환
	output.pos = mul(output.pos, view);
	// 투영 변환
	output.pos = mul(output.pos, projection);

	//output.color = input.color;

	// 정점에서 받은 텍스쳐 좌표 반환.
	output.texCoord = input.texCoord;

	return output;
}

// 정점쉐이더에서 처리해서 PS에 넘겨주면 래스터라이저단계에서 무조건 보간함. 정점에서 처리하면 효율은 좋은데 보간해서 내용이달라짐
// 픽셀 쉐이더에서 텍스쳐 처리하면 각 픽셀단위로 텍스쳐를 입힘.. 만약 픽셀에서 입힌거랑 정점에서 입힌게 똑같으면 정점이 유리하다.

//vs의 아웃붓 = ps의 인풋
float4 PS_Main(VS_OUTPUT psInput) : SV_TARGET
{
	// 텍스쳐를 어떻게 읽을지 - 샘플링
	// 샘플러 스테이트와 텍스쳐 좌표를 이용해서
	float4 texColor = objTexture.Sample(objSampler, psInput.texCoord);

	return texColor;
//이런식으로 채널별로할수잇다. rgba
}