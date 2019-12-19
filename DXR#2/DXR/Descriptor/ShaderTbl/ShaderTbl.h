#pragma once
#include "../Descriptor.h"
#include <string>

class Pipe;
class Output;

class ShaderTbl :
	public Descriptor
{
public:
	// �R���X�g���N�^
	ShaderTbl(const Pipe* pipe);
	// �f�X�g���N�^
	~ShaderTbl();

	// ���C�W�F�l���[�V����
	void RayGeneration(const Output* output, const std::initializer_list<std::string>& name);

	// �~�X
	void Miss(const std::initializer_list<std::string>& name);

	// �N���[�[�X�g�q�b�g
	void ClosestHit(const std::initializer_list<std::string>& name);

	// �G���g���[�T�C�Y�̎擾
	static size_t EntrySize(void);

private:
	// ���\�[�X�̐���
	void CreateRsc(const size_t& entryNum);


	// �p�C�v
	const Pipe* pipe;
};
