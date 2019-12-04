#include "RayGeneration.hlsli"
#include "Infomation.hlsli"

// ���C�W�F�l���[�V����
[RootSignature(RS)]
[shader("raygeneration")]
void RayGen()
{
    uint3 index = DispatchRaysIndex();
    outputTexture[index.xy] = 1.0f;
}

// �~�X
[shader("miss")]
void Miss(inout Payload payload)
{
    payload.hit = false;
}

// �N���[�[�X�g�q�b�g
[shader("closesthit")]
void Chs(inout Payload payload, in BuiltInTriangleIntersectionAttributes attribute)
{
    payload.hit = true;
}

