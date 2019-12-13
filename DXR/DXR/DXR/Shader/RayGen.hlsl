#include "RayGen.hlsli"
#include "Infomation.hlsli"
#include "GlobalInfo.hlsl"

// TLASのリソースビュー
RaytracingAccelerationStructure scene : register(t0);
// 出力用テクスチャ
RWTexture2D<float4> outputTexture : register(u0);

// レイジェネレーション
[shader("raygeneration")]
void RayGen()
{
    //スクリーン内の座標取得
    uint3 index = DispatchRaysIndex();
    //D3D12_DISPATCH_RAYS_DESCのWidth,Height,Depthの取得
    uint3 dim   = DispatchRaysDimensions();
    
    float2 d = (float2(index.xy) / float2(dim.xy)) * 2.0f - 1.0f;
    //アスペクト比
    float aspect = float(dim.x) / float(dim.y);
    
    //光源情報
    RayDesc desc;
    desc.Origin    = float3(0, 0, -2);
    desc.Direction = normalize(float3(d.x * aspect, -d.y, 1));
    desc.TMin      = 0;
	desc.TMax      = 100000;
    
    Payload payload;
    TraceRay(scene, 0, 0xff, 0, 0, 0, desc, payload);
    
	outputTexture[index.xy] = float4(payload.color, 1.0f);
}