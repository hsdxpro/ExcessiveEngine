////////////////////////////////////////////////////////////////////////////////
//	G-buffer material processing and pixel shader
//
////////////////////////////////////////////////////////////////////////////////

#include "common.cginc"

struct {
	// basic parameters
	float3 diffuseColor;
	float glossiness;
	float specularLevel;

	// texture properties
	bool hasDiffuseMap;
	bool hasNormalMap;
	bool hasGlossinessMap;
	bool hasSpecLevelMap;
	bool useCutout;
} mtlProperties : register(c20);


float3x3 calcWsCotangentFrame(float3 wsNormal, float3 VertexToCamWS, float2 tsCoord)
{
	// get edge vectors of the pixel triangle
	float3 dp1 = ddx(VertexToCamWS);
	float3 dp2 = ddy(VertexToCamWS);
	float2 duv1 = ddx(tsCoord);
	float2 duv2 = ddy(tsCoord);

	// solve the linear system
	float3 dp2perp = cross(dp2, wsNormal);
	float3 dp1perp = cross(wsNormal, dp1);
	float3 T = dp2perp * duv1.x + dp1perp * duv2.x;
	float3 B = dp2perp * duv1.y + dp1perp * duv2.y;

	// construct and return a scale-invariant cotangent frame
	float invmax = rsqrt(max(dot(T, T), dot(B, B)));
	return float3x3(T * invmax, B * invmax, wsNormal);
}

//------------------------------------------------------------------------------
//	Pixel shader for no normal maps
//------------------------------------------------------------------------------

GBUFFER PixelShader_Simple (float3 normal, float2 texCoord0) {
	// fill basic from shader const
	float3 diffuse = float3(1, 1, 1);// mtlProperties.diffuseColor;
	float gloss = mtlProperties.glossiness;
	float specLvl = mtlProperties.specularLevel;

	// fill textures
	float4 mapSample;

	// diffuse & cutout
	if (mtlProperties.hasDiffuseMap) {
		mapSample = tex2D(diffuseMap, texCoord0);
		if (mtlProperties.useCutout && mapSample.a < 0.5f)
			discard;
		diffuse *= mapSample.rgb;
	}

	// specular
	if (mtlProperties.hasGlossinessMap) {
		float value = tex2D(glossinessMap, texCoord0).r;
		gloss *= value;
	}
	if (mtlProperties.hasSpecLevelMap) {
		float value = tex2D(specLevelMap, texCoord0).r;
		specLvl *= value;
	}

	return EncodeGBuffer(diffuse, normal, gloss, specLvl);
}

//------------------------------------------------------------------------------
//	Pixel shader WITH normal maps
//------------------------------------------------------------------------------

GBUFFER PixelShader_NormalMap(float3x3 TBN, float2 texCoord)
{
	// normal
	float3 normal =	tex2D(normalMap, texCoord).xyz * 2.0f - float3(1.f,1.f,1.f);

	normal = normalize(mul(normal, TBN));
	return PixelShader_Simple(normal, texCoord);
}