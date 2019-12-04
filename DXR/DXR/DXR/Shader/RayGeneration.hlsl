#include "RayGeneration.hlsli"
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
void Miss(inout Payload payload)
{
    payload.hit = false;
}

// クローゼストヒット
[shader("closesthit")]
void Chs(inout Payload payload, in BuiltInTriangleIntersectionAttributes attribute)
{
    payload.hit = true;
}

