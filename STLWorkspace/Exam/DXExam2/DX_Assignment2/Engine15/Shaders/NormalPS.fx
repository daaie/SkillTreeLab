// �ȼ� �Է� ����ü.
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

	// 0���� �ڸ��� (Half Lambert).
	float3 diffuse = saturate(dot(worldNormal , -lightDir));
	diffuse = diffuse * 0.5f + 0.5f;
	diffuse = pow(diffuse, 2);
	
	// �ؽ�ó ���ø�.
	float4 albedo = diffuseTexture.Sample(objSampler, input.uv);

	// ��ǻ�� x �ؽ�ó ����.
	diffuse = diffuse * albedo.xyz;
	
	float3 specular = 0;
	if (diffuse.x > 0)
	{
		float3 reflection = reflect(lightDir, worldNormal);
		float3 viewDir = normalize(input.viewDir);

		// �ݻ籤�� �� ���� ���� ���� ���ϱ�.
		specular = dot(reflection, -viewDir);
		// 0-1 ���̷� �� ����.
		specular = saturate(specular);
		// ���� ���̰� �� ���� ��Ű��.
		specular = pow(specular, 20.0f);
	}

	// �ֺ���.
	float3 ambient = float3(0.2f, 0.2f, 0.2f) * albedo;

	// ���� ����.
	return float4(diffuse + ambient + specular, 1);
}