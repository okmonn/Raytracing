// ���[�J�����[�g�V�O�l�`��
LocalRootSignature triangleHitLocal =
{
    "CBV(b0)"
};

// �q�b�g�O���[�v
TriangleHitGroup triangleHit =
{
    "",//anyhit
    "TriangleChs"//closesthit
};

// �A�\�V�G�[�V����
SubobjectToExportsAssociation triangleAsso =
{
	"triangleHitLocal", //�T�u�I�u�W�F�N�g��
    "triangleHit"//�G�N�X�|�[�g�֐�
};

// ��ԍ\���t���O
StateObjectConfig triHitState =
{
	STATE_OBJECT_FLAGS_ALLOW_LOCAL_DEPENDENCIES_ON_EXTERNAL_DEFINITONS
};
