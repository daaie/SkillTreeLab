
struct VS_OUTPUT
{
	// POSITION, SV_POSITION
	// SV�� ���� �ø�ƽ�� 10�����̻�.
	// System Value...
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

VS_OUTPUT VS_Main(float4 pos : POSITION, float4 color : COLOR)
{
	VS_OUTPUT output;
	output.pos = pos;
	output.color = color;

	return output;
}

//vs�� �ƿ��� = ps�� ��ǲ
float4 PS_Main(VS_OUTPUT psInput) : SV_TARGET
{
	return psInput.color;
}