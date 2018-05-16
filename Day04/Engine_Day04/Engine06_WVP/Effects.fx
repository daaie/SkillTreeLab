//��� �Ѱܹޱ� -> ����ƽ / �������(dx)
cbuffer CBPerObject
{
	matrix world;	// �������
	matrix view;	// �� ���
	matrix projection; // ���� ���
};

struct VS_INPUT
{
	float4 pos : POSITION;
	float4 color : COLOR;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION; // SV���� �������������ν���
	float4 color : COLOR;
};

VS_OUTPUT VS_Main(VS_INPUT input)
{
	VS_OUTPUT output;

	// ���� ��ȯ ( world, view, projection ����)
	//output.pos = input.pos;

	// ���� ��ȯ
	output.pos = mul(input.pos, world);
	// �� ��ȯ
	output.pos = mul(output.pos, view);
	// ���� ��ȯ
	output.pos = mul(output.pos, projection);

	output.color = input.color;

	return output;
}

//vs�� �ƿ��� = ps�� ��ǲ
float4 PS_Main(VS_OUTPUT psInput) : SV_TARGET
{
	return psInput.color.argb;
//�̷������� ä�κ����Ҽ��մ�. rgba
}