float4 VS_Main(float4 pos : POSITION) : SV_POSITION
				// ��ġ�� �Է�			//�ø�ƽ : �ý��� ��ġ��
{
	return pos;// float4�� x,y,z,������ȯ(0,1)
}

float4 PS_Main(float4 input : POSITION) : SV_TARGET
{
	return float4(1.0f, 1.0f, 0.0f, 1.0f);
}