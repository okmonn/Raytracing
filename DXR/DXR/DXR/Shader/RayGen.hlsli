// ���[�J�����[�g�V�O�l�`��
LocalRootSignature rayGenLocal =
{
    "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), visibility = SHADER_VISIBILITY_ALL),"
    "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), visibility = SHADER_VISIBILITY_ALL)"
};

// �A�\�V�G�[�V����
SubobjectToExportsAssociation rayGenAsso =
{
	"rayGenLocal", //�T�u�I�u�W�F�N�g��
    "RayGen"//�G�N�X�|�[�g�֐�
};

// ��ԍ\���t���O
StateObjectConfig rayGenState =
{
	STATE_OBJECT_FLAGS_ALLOW_LOCAL_DEPENDENCIES_ON_EXTERNAL_DEFINITONS
};