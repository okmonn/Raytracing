// ���[�g�V�O�l�`��
#define RS "RootFlags(LOCAL_ROOT_SIGNATURE),"\
		   "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), visibility = SHADER_VISIBILITY_ALL),"\
		   "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), visibility = SHADER_VISIBILITY_ALL)"

// TLAS�̃��\�[�X�r���[
RaytracingAccelerationStructure scene : register(t0);
// �o�͗p�e�N�X�`��
RWTexture2D<float4> outputTexture : register(u0);
