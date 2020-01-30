/*加速構造*/
RaytracingAccelerationStructure AS : register(t0);
/*出力データ*/
RWTexture2D<float4> output : register(u0);

/*ペイロード*/
struct Payload {
	bool hit;
};

/*レイジェネレーション*/
[shader("raygeneration")]
void RayGen(void) {
	uint3 launchIndex = DispatchRaysIndex();
	uint3 launchDim   = DispatchRaysDimensions();

	output[launchIndex.xy] = float4(1.0f, 0.0f, 0.0f, 1.0f);
}

/*ミス*/
[shader("miss")]
void Miss(inout Payload payload) {
	payload.hit = false;
}

/*クローゼストヒット*/
[shader("closesthit")]
void ClosestHit(inout Payload payload, in BuiltInTriangleIntersectionAttributes attribute) {
	payload.hit = true;
}
