// ��� ���� ����.
cbuffer CBPerObject
{
	matrix world;		// ���� ���.
	matrix view;		// �� ���.
	matrix projection;	// ���� ���.
};

// ���� �Է� ����ü.
struct VS_INPUT
{
	float4 pos : POSITION;
	float4 color : COLOR;
};

Texture2D objTexture;
SamplerState objSampler;

// ���� ��� ����ü.
struct VS_OUTPUT
{	
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

VS_OUTPUT VS_Main(VS_INPUT input)
{
	VS_OUTPUT output;
	//output.pos = input.pos;

	// ���� ��ȯ.
	// ���� ��ȯ.
	output.pos = mul(input.pos, world);
	// �� ��ȯ.
	output.pos = mul(output.pos, view);
	// ���� ��ȯ.
	output.pos = mul(output.pos, projection);

	// �������� �Է¹��� �ؽ�ó ��ǥ ��ȯ.
	output.color = input.color;

	return output;
}

float4 PS_Main(VS_OUTPUT psInput) : SV_TARGET
{
	// �ؽ�ó ���� ��ȯ.
	return psInput.color;
}