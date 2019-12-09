#pragma once
#include "../SubObj.h"
#include <string>

struct ID3D10Blob;
using ID3DBlob = ID3D10Blob;

class Shader :
	public SubObj
{
public:
	// �R���X�g���N�^
	Shader(const std::string& fileName, const std::string& ver, const std::initializer_list<std::string>& func, const std::string& entry = "");
	// �f�X�g���N�^
	~Shader();

	// �V�F�[�_�u���u�̎擾
	ID3DBlob* Get(void) const;

	// �R���N�V�������̎擾
	size_t Num(void) const;

private:
	// �T�u�I�u�W�F�N�g�̐���
	void CreateSub(const std::initializer_list<std::string>& func);


	// �V�F�[�_�u���u
	ID3DBlob* blob;

	// �R���N�V������
	size_t num;
};
