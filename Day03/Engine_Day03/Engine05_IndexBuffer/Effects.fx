struct VS_INPUT
{
	float4 pos : POSITION;
	float4 color : COLOR;
};

struct VS_OUTPUT
{
	// POSITION, SV_POSITION
	// SV가 붙은 시멘틱은 10버전이상.
	// System Value...
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

VS_OUTPUT VS_Main(VS_INPUT input)
{
	VS_OUTPUT output;
	output.pos = input.pos;
	output.color = input.color;

	return output;
}

//vs의 아웃붓 = ps의 인풋
float4 PS_Main(VS_OUTPUT psInput) : SV_TARGET
{
	return psInput.color.rbga;
//이런식으로 채널별로할수잇다. rgba
}