TextureCube cubeMap : register(t0);
SamplerState objSampler : register(s0);

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	// 최종 색상
	return cubeMap.Sample(objSampler, input.texCoord);
}