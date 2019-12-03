// ルートシグネチャ
#define RS "RootFlags(LOCAL_ROOT_SIGNATURE),"\
		   "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), visibility = SHADER_VISIBILITY_ALL),"\
		   "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), visibility = SHADER_VISIBILITY_ALL)"

// TLASのリソースビュー
RaytracingAccelerationStructure scene : register(t0);
// 出力用テクスチャ
RWTexture2D<float4> outputTexture : register(u0);
