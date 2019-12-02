#include "Generation.hlsli"
#include "Infomation.hlsli"

// レイジェネレーション
[RootSignature(RS)]
[shader("raygeneration")]
void RayGen()
{
    uint3 index = DispatchRaysIndex();
    outputTexture[index.xy] = 1.0f;
}

// ミス
[shader("miss")]
void Miss(Payload payload)
{
    payload.hit = false;
}
