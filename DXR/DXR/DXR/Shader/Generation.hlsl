#include "Generation.hlsli"
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
void Miss(Payload payload)
{
    payload.hit = false;
}
