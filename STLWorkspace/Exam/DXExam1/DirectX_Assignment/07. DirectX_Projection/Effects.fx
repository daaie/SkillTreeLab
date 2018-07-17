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
	float4 color : COLOR;
};

// 정점 출력 구조체.
struct VS_OUTPUT
{	
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

VS_OUTPUT VS_Main(VS_INPUT input)
{
	VS_OUTPUT output;
	//output.pos = input.pos;

	// 월드 변환.
	output.pos = mul(input.pos, world);
	// 뷰 변환.
	output.pos = mul(output.pos, view);
	// 투영 변환.
	output.pos = mul(output.pos, projection);

	output.color = input.color;

	return output;
}

float4 PS_Main(VS_OUTPUT psInput) : SV_TARGET
{
	return psInput.color;
}