#include "RayGen.hlsli"
#include "Infomation.hlsli"
#include "GlobalInfo.hlsl"

// TLAS�̃��\�[�X�r���[
RaytracingAccelerationStructure scene : register(t0);
// �o�͗p�e�N�X�`��
RWTexture2D<float4> outputTexture : register(u0);

// ���C�W�F�l���[�V����
[shader("raygeneration")]
void RayGen()
{
    //�X�N���[�����̍��W�擾
    uint3 index = DispatchRaysIndex();
    //D3D12_DISPATCH_RAYS_DESC��Width,Height,Depth�̎擾
    uint3 dim   = DispatchRaysDimensions();
    
    float2 d = (float2(index.xy) / float2(dim.xy)) * 2.0f - 1.0f;
    //�A�X�y�N�g��
    float aspect = float(dim.x) / float(dim.y);
    
    //�������
    RayDesc desc;
    desc.Origin    = float3(0, 0, -2);
    desc.Direction = normalize(float3(d.x * aspect, -d.y, 1));
    desc.TMin      = 0;
	desc.TMax      = 100000;
    
    Payload payload;
    TraceRay(scene, 0, 0xff, 0, 0, 0, desc, payload);
    
	outputTexture[index.xy] = float4(payload.color, 1.0f);
}