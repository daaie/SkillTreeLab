// 픽셀 출력 구조체.
struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float3 viewDir : TEXCOORD1;
};

TextureCube skyboxTexture;
SamplerState skyboxSampler;

float4 main(PS_INPUT input) : SV_TARGET
{
	float3 normal = float3(0, 0, 1);
	float3 viewDir = normalize(input.viewDir);
	float3 reflection = reflect(viewDir, normal);

	return skyboxTexture.Sample(skyboxSampler, reflection);
}