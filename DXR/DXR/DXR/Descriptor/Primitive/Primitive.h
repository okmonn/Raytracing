#pragma once
#include "../Descriptor.h"
#include "../../Matrix.h"
#include <vector>

class Primitive :
	public Descriptor
{
public:
	// �R���X�g���N�^
	Primitive(const Vec3f* vertex, const size_t& vertexNum, const size_t& instanceNum = 1);
	// �f�X�g���N�^
	~Primitive();

	// �ړ�
	void Translation(const size_t& instanceID, const Vec3f& vec);

	// ���_���\�[�X�̎擾
	ID3D12Resource* Get(void) const;

	// �C���X�^���X���̎擾
	size_t InstanceNum(void) const;
	
	// �s��̎擾
	Matrix3x4 Matrix(const size_t& instanceID) const;

private:
	// ���_���\�[�X�̐���
	void CreateVertexRsc(const size_t& size);

	// ���_�f�[�^���X�V����
	void UpData(void** buf, const Vec3f* vertex);


	// �C���X�^���X��
	size_t instanceNum;

	// �s��
	std::vector<Matrix3x4>matrix;
};
