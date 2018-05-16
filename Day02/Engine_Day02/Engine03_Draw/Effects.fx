float4 VS_Main(float4 pos : POSITION) : SV_POSITION
				// 위치값 입력			//시멘틱 : 시스템 위치값
{
	return pos;// float4는 x,y,z,동차변환(0,1)
}

float4 PS_Main(float4 input : POSITION) : SV_TARGET
{
	return float4(1.0f, 1.0f, 0.0f, 1.0f);
}