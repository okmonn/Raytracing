#include "RayGen.hlsli"
#include "Infomation.hlsli"

// TLAS�̃��\�[�X�r���[
RaytracingAccelerationStructure scene : register(t0);
// �o�͗p�e�N�X�`��
RWTexture2D<float4> outputTexture : register(u0);

// ���C�W�F�l���[�V����
[shader("raygeneration")]
void RayGen()
{
    uint3 index = DispatchRaysIndex();
    outputTexture[index.xy] = 1.0f;
}