#pragma once
#include "../SubObj.h"

class PipeConfig :
	public SubObj
{
public:
	// �R���X�g���N�^
	PipeConfig(const size_t& depth);
	// �f�X�g���N�^
	~PipeConfig();

private:
	// �T�u�I�u�W�F�N�g�̐���
	void CreateSub(const size_t& depth);
};
