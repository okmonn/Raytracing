#pragma once
#include "../Descriptor.h"
#include <string>

class Output;
class Pipe;

class ShaderTbl :
	public Descriptor
{
public:
	// �R���X�g���N�^
	ShaderTbl(const Pipe* pipe);
	// �f�X�g���N�^
	~ShaderTbl();

	// ���C�W�F�l���[�V�����p
	void RayGeneration(const std::string& name, const Output* output);

	// �~�X�p
	void Miss(const std::string& name);

	// �N���[�[�X�g�q�b�g�p
	void Closest(const std::string& name);

	// �V�F�[�_�G���g���[�T�C�Y�̎擾
	static size_t EntrySize(void);

private:
	// ���\�[�X�̐���
	void CreateRsc(const size_t& num);


	// �p�C�v
	Pipe* pipe;
};
