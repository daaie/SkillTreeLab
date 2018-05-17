// 상수 버퍼 선언.
cbuffer CBPerObject
{
	matrix world;		// 월드 행렬.
	matrix view;		// 뷰 행렬.
	matrix projection;	// 투영 행렬.
};

// 정점 입력 구조체.
struct VS_INPUT
{
	float4 pos : POSITION;
	float2 texCoord : TEXCOORD;
};

Texture2D objTexture;
SamplerState objSampler;

// 정점 출력 구조체.
struct VS_OUTPUT
{	
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS_Main(VS_INPUT input)
{
	VS_OUTPUT output;
	//output.pos = input.pos;

	// 공간 변환.
	// 월드 변환.
	output.pos = mul(input.pos, world);
	// 뷰 변환.
	output.pos = mul(output.pos, view);
	// 투영 변환.
	output.pos = mul(output.pos, projection);

	// 정점에서 입력받은 텍스처 좌표 반환.
	output.texCoord = input.texCoord;

	return output;
}

float4 PS_Main(VS_OUTPUT psInput) : SV_TARGET
{
	// 텍스처 샘플링.
	float4 texColor = objTexture.Sample(objSampler, psInput.texCoord);

	// 텍스처 색상 반환.
	return texColor;
}