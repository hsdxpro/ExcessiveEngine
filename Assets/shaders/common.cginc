////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	Commonly used shader algorithms and helper functions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef COMMON_CGINC
#define COMMON_CGINC

//------------------------------------------------------------------------------
// Position and Normal coordinate system conversion
//------------------------------------------------------------------------------

// Convert from clip space to world space
float3 GetWorldPosition(float2 screenCoords, float depth, float4x4 invViewProj) {
	screenCoords.x = 2.0f * (screenCoords.x - 0.5f);
	screenCoords.y = -2.0f * (screenCoords.y - 0.5f);
	float4 posH = float4(screenCoords, depth, 1.0f); // NDC space
	float4 posW = mul(posH, invViewProj);
	posW /= posW.w;
	return posW.xyz;
}

float3 GetWorldPosition(float3 camPos, float3 viewRay, float linearDepth) {
	return linearDepth * viewRay + camPos;
}


// Pack/unpack normals
float2 PackNormal(float3 n) {
	float2 ret;
	ret.r = n.x;
	ret.g = 0.5f*n.y + (n.z>0.f ? 0.5f:-0.5f);
	return ret;
}

float3 UnpackNormal(float2 packedNormal) {
	float3 normal;
	normal.x = packedNormal.r;
	normal.y = 2.0f*(packedNormal.g>0.0f ? packedNormal.g-0.5f:packedNormal.g+0.5f);
	normal.z = sqrt(clamp(1.0f - normal.x*normal.x - normal.y*normal.y, 0, 1)) * (packedNormal.g>0.0f ? 1.0f:-1.0f);
	return normal;
}


//------------------------------------------------------------------------------
// G-Buffer storage
//------------------------------------------------------------------------------
struct GBUFFER {
	float4 diffuse : COLOR0;
	float2 normal : COLOR1;
	float4 misc : COLOR2;
};

GBUFFER EncodeGBuffer(float3 diffuse, float3 normal, float glossiness, float specLevel) {
	GBUFFER o;
	o.diffuse.rgb = diffuse; o.diffuse.a = 1.0f;
	o.normal = PackNormal(normalize(normal));
	o.misc.r = glossiness;
	o.misc.g = specLevel;
	o.misc.b = o.misc.a = 0.0f;
	return o;
}

void DecodeGBuffer(GBUFFER gb, out float3 diffuse, out float3 normal, out float glossiness, out float specLevel) {
	diffuse = gb.diffuse.rgb;
	normal = normalize(UnpackNormal(gb.normal.rg));
	glossiness = gb.misc.r;
	specLevel = gb.misc.g;
}


#endif