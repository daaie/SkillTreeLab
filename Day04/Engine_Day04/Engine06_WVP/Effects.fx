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
	float4 color : COLOR;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION; // SV여야 파이프라인이인식함
	float4 color : COLOR;
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

	output.color = input.color;

	return output;
}

//vs의 아웃붓 = ps의 인풋
float4 PS_Main(VS_OUTPUT psInput) : SV_TARGET
{
	return psInput.color.argb;
//이런식으로 채널별로할수잇다. rgba
}