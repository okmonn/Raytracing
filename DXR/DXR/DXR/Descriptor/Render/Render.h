#pragma once
#include "../Descriptor.h"

class Swap;

class Render :
	public Descriptor
{
public:
	// �R���X�g���N�^
	Render(const Swap* swap);
	// �f�X�g���N�^
	~Render();

private:
	// �����_�[���\�[�X�̐���
	void CreateRenderRsc(const Swap* swap);
};
