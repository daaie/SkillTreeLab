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
	//float4 color : COLOR;
	float2 texCoord : TEXCOORD0;
	//float2 texCoord2 : TEXCOORD1; // �䷱������ ������ �Ѱ��ٙ�����
};

Texture2D objTexture;
SamplerState objSampler;

struct VS_OUTPUT
{
	float4 pos : SV_POSITION; // SV���� �������������ν���
	//float4 color : COLOR;
	float2 texCoord : TEXCOORD0;
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

	//output.color = input.color;

	// �������� ���� �ؽ��� ��ǥ ��ȯ.
	output.texCoord = input.texCoord;

	return output;
}

// �������̴����� ó���ؼ� PS�� �Ѱ��ָ� �����Ͷ������ܰ迡�� ������ ������. �������� ó���ϸ� ȿ���� ������ �����ؼ� �����̴޶���
// �ȼ� ���̴����� �ؽ��� ó���ϸ� �� �ȼ������� �ؽ��ĸ� ����.. ���� �ȼ����� �����Ŷ� �������� ������ �Ȱ����� ������ �����ϴ�.

//vs�� �ƿ��� = ps�� ��ǲ
float4 PS_Main(VS_OUTPUT psInput) : SV_TARGET
{
	// �ؽ��ĸ� ��� ������ - ���ø�
	// ���÷� ������Ʈ�� �ؽ��� ��ǥ�� �̿��ؼ�
	float4 texColor = objTexture.Sample(objSampler, psInput.texCoord);

	return texColor;
//�̷������� ä�κ����Ҽ��մ�. rgba
}