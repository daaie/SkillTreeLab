Texture2D objTexture : register(t0);
SamplerState objSampler : register(s0);

// �ȼ� �Է� ����ü.
struct PS_INPUT
{	
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float3 diffuse : TEXCOORD1;
};

float4 main(PS_INPUT psInput) : SV_TARGET
{
	float3 albedo = objTexture.Sample(objSampler, psInput.texCoord).rgb;

	// 0���� �ڸ���.
	float3 diffuse = saturate(psInput.diffuse) * 0.5f + 0.5f;
	diffuse *= albedo;

	// �ֺ���.
	float3 ambient = float3(0.1f, 0.1f, 0.1f);

	// ���� ����.
	//return float4(diffuse + ambient, 1);
	return float4(diffuse, 1);
}