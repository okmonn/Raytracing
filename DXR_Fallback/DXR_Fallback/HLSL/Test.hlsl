/*�����\��*/
RaytracingAccelerationStructure AS : register(t0);
/*�o�̓f�[�^*/
RWTexture2D<float4> output : register(u0);

/*�y�C���[�h*/
struct Payload {
	bool hit;
};

/*���C�W�F�l���[�V����*/
[shader("raygeneration")]
void RayGen(void) {
	uint3 launchIndex = DispatchRaysIndex();
	uint3 launchDim   = DispatchRaysDimensions();

	output[launchIndex.xy] = float4(1.0f, 0.0f, 0.0f, 1.0f);
}

/*�~�X*/
[shader("miss")]
void Miss(inout Payload payload) {
	payload.hit = false;
}

/*�N���[�[�X�g�q�b�g*/
[shader("closesthit")]
void ClosestHit(inout Payload payload, in BuiltInTriangleIntersectionAttributes attribute) {
	payload.hit = true;
}
