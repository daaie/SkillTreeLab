// 정점 출력 구조체.
struct VS_OUTPUT
{	
	float4 pos : SV_POSITION;
	//float4 pos : POSITION;
	float4 color : COLOR;
};

VS_OUTPUT VS_Main(float4 pos : POSITION, float4 color : COLOR)
{
	VS_OUTPUT output;
	output.pos = pos;
	output.color = color;

	return output;
}

float4 PS_Main(VS_OUTPUT psInput) : SV_TARGET
{
	//return float4(1.0f, 1.0f, 0.0f, 1.0f);
	return psInput.color;
}