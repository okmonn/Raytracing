/*出力データ*/
RWTexture2D<float4> output : register(u0);
/*加速構造*/
RaytracingAccelerationStructure AS : register(t0);

/*ペイロード*/
struct Payload {
	float3 color;
};

/*レイジェネレーション*/
[shader("raygeneration")]
void RayGen(void) {
	uint3 launchIndex = DispatchRaysIndex();
	uint3 launchDim   = DispatchRaysDimensions();

    float2 crd  = float2(launchIndex.xy);
    float2 dims = float2(launchDim.xy);

    float2 d = ((crd / dims) * 2.f - 1.f);
    float aspectRatio = dims.x / dims.y;

    RayDesc ray;
    ray.Origin = float3(0, 0, -2);
    ray.Direction = normalize(float3(d.x * aspectRatio, -d.y, 1));

    ray.TMin = 0;
    ray.TMax = 100000;

    Payload payload;
    TraceRay(AS, 0 /*rayFlags*/, 0xFF, 0 /* ray index*/, 0, 0, ray, payload);
    output[launchIndex.xy] = float4(payload.color, 1.0f);
}

/*ミス*/
[shader("miss")]
void Miss(inout Payload payload) {
	payload.color = float3(0.4f, 0.6f, 0.2f);
}

/*クローゼストヒット*/
[shader("closesthit")]
void ClosestHit(inout Payload payload, in BuiltInTriangleIntersectionAttributes attribute) {
	payload.color = float3(1.0f, 0.0f, 0.0f);
}
