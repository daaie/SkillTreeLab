// 픽셀 입력 구조체.
struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 lightDir : TEXCOORD1;
	float3 viewDir : TEXCOORD2;
	float3 T : TEXCOORD3;
	float3 B : TEXCOORD4;
	float3 N : TEXCOORD5;
};

Texture2D diffuseTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState objSampler : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	float3 tangentNormal = normalTexture.Sample(objSampler, input.uv).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(input.T), normalize(input.B), normalize(input.N));
	TBN = transpose(TBN);
	float3 worldNormal = normalize(mul(TBN, tangentNormal));
	
	float3 lightDir = normalize(input.lightDir);

	// 0이하 자르기 (Half Lambert).
	float3 diffuse = saturate(dot(worldNormal , -lightDir));
	diffuse = diffuse * 0.5f + 0.5f;
	diffuse = pow(diffuse, 2);
	
	// 텍스처 샘플링.
	float4 albedo = diffuseTexture.Sample(objSampler, input.uv);

	// 디퓨즈 x 텍스처 색상.
	diffuse = diffuse * albedo.xyz;
	
	float3 specular = 0;
	if (diffuse.x > 0)
	{
		float3 reflection = reflect(lightDir, worldNormal);
		float3 viewDir = normalize(input.viewDir);

		// 반사광과 뷰 벡터 간의 내적 구하기.
		specular = dot(reflection, -viewDir);
		// 0-1 사이로 값 고정.
		specular = saturate(specular);
		// 영역 줄이고 빛 증폭 시키기.
		specular = pow(specular, 20.0f);
	}

	// 주변광.
	float3 ambient = float3(0.2f, 0.2f, 0.2f) * albedo;

	// 최종 색상.
	return float4(diffuse + ambient + specular, 1);
}