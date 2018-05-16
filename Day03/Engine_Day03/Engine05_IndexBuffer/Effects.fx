struct VS_INPUT
{
	float4 pos : POSITION;
	float4 color : COLOR;
};

struct VS_OUTPUT
{
	// POSITION, SV_POSITION
	// SV�� ���� �ø�ƽ�� 10�����̻�.
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

//vs�� �ƿ��� = ps�� ��ǲ
float4 PS_Main(VS_OUTPUT psInput) : SV_TARGET
{
	return psInput.color.rbga;
//�̷������� ä�κ����Ҽ��մ�. rgba
}