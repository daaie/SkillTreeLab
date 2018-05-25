// �ȼ� �Է� ����ü.
struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 lightDir : TEXCOORD1;
	float3 viewDir : TEXCOORD2;
	//float3 reflection : TEXCOORD3;

	// �߰�.
	float3 T : TEXCOORD3;
	float3 B : TEXCOORD4;
	float3 N : TEXCOORD5;
};

Texture2D objTexture : register(t0);
Texture2D normalTexture : register(t1);
// ť��� �߰�.
TextureCube cubemapTexture : register(t2);
SamplerState objSampler : register(s0);

// �ȼ� ���̴� �Լ�.
float4 main(PS_INPUT input) : SV_TARGET
{
	// ź��Ʈ ��� �� �о���� (��ָ� �ؽ�ó����).
	float3 tangentNormal = normalTexture.Sample(objSampler, input.uv);
	
	// ��� �� ���.
	tangentNormal = normalize(tangentNormal * 2.0f -1.0f);

	// ���� ���� ��ȯ ��� �����.
	float3x3 TBN = float3x3(normalize(input.T), normalize(input.B), normalize(input.N));

	// ���� ��� ���ϱ�.
	float3 worldNormal = normalize(mul(tangentNormal, TBN));

	float3 lightDir = normalize(input.lightDir);

	// ���ݻ�(��ǻ��) ���ϱ� / 0���� �� �ڸ���.
	float3 diffuse = saturate(dot(worldNormal, -lightDir));

	// ��ǻ��� �о����.
	float3 albedo = objTexture.Sample(objSampler, input.uv);
	// ��ǻ�� ���� ����� ��ǻ��� �� ���ϱ�.
	diffuse = diffuse * albedo;

	// ���ݻ�(����ŧ��).
	float3 specular = 0;
	// ī�޶� ���⺤��.
	float3 viewDir = normalize(input.viewDir);
	if (diffuse.x > 0)
	{
		// �ݻ� ���� ���ϱ�.
		float3 reflection = reflect(lightDir, worldNormal);

		// �ݻ� ���Ϳ� �交���� ���� ���ϱ�.
		specular = dot(reflection, -viewDir);
		
		// 0 - 1 ������ ������ ���� / ���� ���̰� �� ���� ����.
		specular = pow(saturate(specular), 20.0f);
	}

	// �ֺ���(ambient).
	float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;

	// ī�޶� ������ �ݻ� ����.
	float3 viewReflect = reflect(viewDir, worldNormal);

	// ť���.
	float3 environment = cubemapTexture.Sample(objSampler, viewReflect).rgb;

	// ���� ����.
	return float4(diffuse + specular + ambient + environment * 0.5f, 1.0f);
	//return float4(environment, 1.0f);
}