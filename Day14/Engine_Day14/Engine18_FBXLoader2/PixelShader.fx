// �ȼ� �Է� ����ü.
struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 diffuse : TEXCOORD1;
	float3 viewDir : TEXCOORD2;
	float3 reflection : TEXCOORD3;
};

Texture2D objTexture;
SamplerState objSampler;

float4 main(PS_INPUT psInput) : SV_TARGET
{
	// 0���� �ڸ���.
	float3 diffuse = saturate(psInput.diffuse);
	
	// �ؽ�ó ���ø�.
	float4 albedo = objTexture.Sample(objSampler, psInput.uv);

	// ��ǻ�� x �ؽ�ó ����.
	diffuse = diffuse * albedo.xyz;

	// �����⸦ ��ģ ���� �ٽ� ����.
	float3 reflection = normalize(psInput.reflection);
	float3 viewDir = normalize(psInput.viewDir);
	float3 specular = 0;

	if (diffuse.x > 0)
	{
		// �ݻ籤�� �� ���� ���� ���� ���ϱ�.
		specular = dot(reflection, -viewDir);
		// 0-1 ���̷� �� ����.
		specular = saturate(specular);
		// ���� ���̰� �� ���� ��Ű��.
		specular = pow(specular, 20.0f);
	}

	// �ֺ���.
	float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;

	// ���ݻ籤(��ǻ��) ����.
	float3 diffuseColor = float3(1.0f, 1.0f, 1.0f);

	// ���� ����.
	return float4(diffuse + ambient + specular, 1);
	//return float4(specular, 1);
}