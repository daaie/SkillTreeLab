Texture2D objTexture : register(t0);
SamplerState objSampler : register(s0);

// 픽셀 입력 구조체.
struct PS_INPUT
{	
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float3 diffuse : TEXCOORD1;
};

float4 main(PS_INPUT psInput) : SV_TARGET
{
	float3 albedo = objTexture.Sample(objSampler, psInput.texCoord).rgb;

	// 0이하 자르기.
	float3 diffuse = saturate(psInput.diffuse) * 0.5f + 0.5f;
	diffuse *= albedo;

	// 주변광.
	float3 ambient = float3(0.1f, 0.1f, 0.1f);

	// 최종 색상.
	//return float4(diffuse + ambient, 1);
	return float4(diffuse, 1);
}