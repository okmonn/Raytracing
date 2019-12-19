#pragma once
#include "../Descriptor.h"

class Window;
class Acceleration;

class Output :
	public Descriptor
{
public:
	// �R���X�g���N�^
	Output(const Window* win, const Acceleration* top);
	// �f�X�g���N�^
	~Output();

private:
	// ���\�[�X�̐���
	void CreateRsc(const Window* win);

	// UAV�̐���
	void UAV(const size_t& index);

	// SRV�̐���
	void SRV(const Acceleration* top, const size_t& index);
};
