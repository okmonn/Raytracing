#pragma once
#include "../Descriptor.h"

class Pipe;

class ShaderTbl :
	public Descriptor
{
public:
	// �R���X�g���N�^
	ShaderTbl(const Pipe* pipe);
	// �f�X�g���N�^
	~ShaderTbl();

private:
	// �V�F�[�_�e�[�u���T�C�Y
};
