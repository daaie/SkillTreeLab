struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float3 viewDir : TEXCOORD1;
};

TextureCube cubeMap : register(t0);
SamplerState objSampler : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	float3 normal = float3(0, 0, 1);
	float3 viewDir = normalize(input.viewDir);
	float3 viewReflection = reflect(viewDir, normal);

	float3 skybox = cubeMap.Sample(objSampler, viewReflection).rgb;

	return float4(skybox, 1.0f);
}