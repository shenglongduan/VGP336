// Description: Terrain effect to render terrain objects

cbuffer TransformBuffer : register(b0)
{
	matrix wvp;
    matrix lwvp;
	matrix world;
	float3 viewPosition;
}
cbuffer LightBuffer : register(b1)
{
	float3 lightDirection;
	float4 lightAmbient;
	float4 lightDiffuse;
	float4 lightSpecular;
}

cbuffer MaterialBuffer : register(b2)
{
    float4 materialAmbient;
    float4 materialDiffuse;
    float4 materialSpecular;
    float4 materialEmissive;
    float materialPower;
}

cbuffer SettingsBuffer : register(b3)
{
	bool useNormalMap;
	bool useSpecMap;
    bool useShadowMap;
    bool useBlendMap;
    float depthBias;
    float blendHeight;
}

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D specMap : register(t2);
Texture2D blendMap : register(t3);
Texture2D shadowMap : register(t4);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
    float3 worldNormal : NORMAL;
	float3 worldTagent : TANGENT;
	float2 texCoord : TEXCOORD0;
	float3 dirToLight : TEXCOORD1;
	float3 dirToView : TEXCOORD2;
    float4 lightNDCPosition : TEXCOORD3;
    float4 worldPosition : TEXCOORD4;
};

VS_OUTPUT VS(VS_INPUT input)
{
	matrix toWorld = world;
	matrix toNDC = wvp;

	float3 localPosition = input.position;
	
	VS_OUTPUT output;
	output.position = mul(float4(localPosition, 1.0f), toNDC);
	output.worldNormal = mul(input.normal, (float3x3) toWorld);
	output.worldTagent = mul(input.tangent, (float3x3) toWorld);
	output.texCoord = input.texCoord;
	output.dirToLight = -lightDirection;
	output.dirToView = normalize(viewPosition - mul(float4(localPosition, 1.0f), world).xyz);
    if (useShadowMap)
    {
        output.lightNDCPosition = mul(float4(localPosition, 1.0f), lwvp);
    }
    output.worldPosition = mul(float4(localPosition, 1.0f), toWorld);

	
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 n = normalize(input.worldNormal);
	
	//use normal map to get the new normal
	if (useNormalMap)
	{
		float3 t = normalize(input.worldTagent);
		float3 b = normalize(cross(n, t));
		float3x3 tbnw = float3x3(t, b, n);
		float4 normalMapColor = normalMap.Sample(textureSampler, input.texCoord);
		float3 unpackedNormalMap = normalize(float3((normalMapColor.xy * 2.0f) - 1.0f, normalMapColor.z));
		n = normalize(mul(unpackedNormalMap, tbnw));
	}

	float3 light = normalize(input.dirToLight);
	float3 view = normalize(input.dirToView);

	float4 ambient = lightAmbient * materialAmbient;

	float d = saturate(dot(light, n));
	float4 diffuse = d * lightDiffuse * materialDiffuse;

	float3 r = reflect(-light, n);
	float base = saturate(dot(r, view));
	float s = pow(base, materialPower);
	float4 specular = s * lightSpecular * materialSpecular;
	
    float4 emissive = materialEmissive;
	
	float4 diffuseMapColor = diffuseMap.Sample(textureSampler, input.texCoord);
	float4 specMapColor = (useSpecMap)? specMap.Sample(textureSampler, input.texCoord).r : 1.0f;
    float4 colorToUse = diffuseMapColor;
    if (useBlendMap)
    {
        if (input.worldPosition.y > blendHeight)
        {
            colorToUse = blendMap.Sample(textureSampler, input.texCoord);
        }
        else
        {
            float blendWidth = 5;
            float transitionHeight = blendHeight - blendWidth;
            float t = saturate((input.worldPosition.y - transitionHeight) / blendWidth);
            colorToUse = blendMap.Sample(textureSampler, input.texCoord);
            colorToUse = lerp(diffuseMapColor, colorToUse, t);
        }
    }
	
    float4 finalColor = (ambient + diffuse + emissive) * colorToUse + (specular * specMapColor);
	
    if (useShadowMap)
    {
        float actualDepth = 1.0f - (input.lightNDCPosition.z / input.lightNDCPosition.w);
        float2 shadowUV = input.lightNDCPosition.xy / input.lightNDCPosition.w;
        float u = (shadowUV.x + 1.0f) * 0.5f;
        float v = (1.0f - (shadowUV.y + 1.0f)) * 0.5f;

        if (saturate(u) == u && saturate(v) == v)
        {
            float4 savedColor = shadowMap.Sample(textureSampler, float2(u, v));
            float savedDepth = savedColor.r;

            if (savedDepth > actualDepth + depthBias)
            {
                finalColor = (ambient + materialEmissive) * diffuseMapColor;
            }
        }
    }

	return finalColor;
}