Texture2D objTexture;
SamplerState objSampler;

// 픽셀 쉐이더 입력 구조체.
struct PS_INPUT
{
	float4 pos : SV_POSITION;
	//float2 texCoord : TEXCOORD;
	float3 diffuse : TEXCOORD1;

	float3 viewDir : TEXCOORD2;
	float3 reflection : TEXCOORD3;
};

float4 main(PS_INPUT psInput) : SV_TARGET
{
	// 0이하 자르기.
	float3 diffuse = saturate(psInput.diffuse);

	// 보간기를 거친 값을 다시 정렬.
	float3 reflection = normalize(psInput.reflection);
	float3 viewDir = normalize(psInput.viewDir);
	float3 specular = 0;

	if (diffuse.x > 0)
	{
		// 반사광과 뷰 벡터 간의 내적 구하기.
		specular = dot(reflection, -viewDir);
		// 0-1 사이로 값 고정.
		specular = saturate(specular);
		// 영역 줄이고 빛 증폭 시키기.
		specular = pow(specular, 20.0f);
	}

	// 주변광.
	float3 ambient = float3(0.1f, 0.1f, 0.1f);

	// 난반사광(디퓨즈) 색상.
	float3 diffuseColor = float3(1.0f, 1.0f, 1.0f);

	// 최종 색상.
	return float4(diffuseColor * diffuse + ambient + specular, 1);
	//return float4(specular, 1);
}