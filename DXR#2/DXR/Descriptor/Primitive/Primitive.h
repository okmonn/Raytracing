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

	// �C���X�^���X���̎擾
	size_t InstanceNum(void) const;

	// �s��̎擾
	Matrix3x4 Matrix(const size_t& index) const;

private:
	// ���\�[�X�̐���
	void CreateRsc(const size_t& size);

	// ���_�f�[�^�̍X�V
	void UpData(const Vec3f* vertex);


	// �s��
	std::vector<Matrix3x4>matrix;
};
