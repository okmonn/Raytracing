#include "RayGen.hlsli"
#include "Infomation.hlsli"

// TLASのリソースビュー
RaytracingAccelerationStructure scene : register(t0);
// 出力用テクスチャ
RWTexture2D<float4> outputTexture : register(u0);

// レイジェネレーション
[shader("raygeneration")]
void RayGen()
{
    uint3 index = DispatchRaysIndex();
    outputTexture[index.xy] = 1.0f;
}