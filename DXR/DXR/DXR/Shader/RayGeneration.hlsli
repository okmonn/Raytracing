GlobalRootSignature global =
{
    " "
};

// ���[�J�����[�g�V�O�l�`��
LocalRootSignature local =
{
    "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), visibility = SHADER_VISIBILITY_ALL),"
    "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), visibility = SHADER_VISIBILITY_ALL)"
};

// �A�\�V�G�[�V����
SubobjectToExportsAssociation association =
{
    "local",//�T�u�I�u�W�F�N�g��
    "RayGen"//�G�N�X�|�[�g�֐�
};

// �V�F�[�_�R���t�B�O
RaytracingShaderConfig sConfig =
{
    16,//�y�C���[�h�T�C�Y
    8//�A�g���r���[�g�T�C�Y
};

// �p�C�v���C���R���t�B�O
RaytracingPipelineConfig pConfig =
{
    0//�[�x�̐[��
};