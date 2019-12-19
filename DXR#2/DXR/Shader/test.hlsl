// �O���[�o�����[�g�V�O�l�`��
GlobalRootSignature global =
{
	" "
};

// ���C�W�F�l���[�V�����p���[�g�V�O�l�`��
LocalRootSignature rayGen =
{
	"DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), visibility = SHADER_VISIBILITY_ALL),"
    "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), visibility = SHADER_VISIBILITY_ALL)"
};

// ���C�W�F�l���[�V�����̊֘A�t��
SubobjectToExportsAssociation rayGenAsso =
{
	"rayGen",//�T�u�I�u�W�F�N�g��
	"RayGen"//�֘A�t���֐�
};

// �~�X�p���[�g�V�O�l�`��
LocalRootSignature miss =
{
	" "
};

// �~�X�̊֘A�t��
SubobjectToExportsAssociation missAsso =
{
	"miss",//�T�u�I�u�W�F�N�g��
	"Miss",//�֘A�t���֐�
};

// �N���[�[�X�g�q�b�g�p���[�g�V�O�l�`��
LocalRootSignature closest =
{
	" "
};

// �q�b�g�O���[�v
TriangleHitGroup Hit =
{
	"", //anyhit
	"Chs"//closesthit
};

// �N���[�[�X�g�q�b�g�̊֘A�t��
SubobjectToExportsAssociation closestAsso =
{
	"closest", //�T�u�I�u�W�F�N�g��
	"Hit"//�֘A�t���֐�
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
	0 //�N���[�[�X�g�q�b�g�̉�
};

// �����\���̏��
RaytracingAccelerationStructure acceleration : register(t0);
// �o�̓e�N�X�`��
RWTexture2D<float4> output : register(u0);

// �y�C���[�h
struct Payload
{
	bool hit;
};

float3 linearToSrgb(float3 c)
{
    // Based on http://chilliant.blogspot.com/2012/08/srgb-approximations-for-hlsl.html
	float3 sq1 = sqrt(c);
	float3 sq2 = sqrt(sq1);
	float3 sq3 = sqrt(sq2);
	float3 srgb = 0.662002687 * sq1 + 0.684122060 * sq2 - 0.323583601 * sq3 - 0.0225411470 * c;
	return srgb;
}

// ���C�W�F�l���[�V����
[shader("raygeneration")]
void RayGen()
{
	uint3 index = DispatchRaysIndex();
	output[index.xy] = float4(linearToSrgb(float3(0.4, 0.6, 0.2)), 1.0);
}

// �~�X
[shader("miss")]
void Miss(inout Payload payload)
{
	payload.hit = false;
}

// �N���[�[�X�g�q�b�g
[shader("closesthit")]
void Chs(inout Payload payload, in BuiltInTriangleIntersectionAttributes attribute)
{
	payload.hit = true;
}
