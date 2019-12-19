#pragma once

struct ID3D12Fence1;
class Queue;

class Fence
{
public:
	// �R���X�g���N�^
	Fence(const Queue* queue);
	// �f�X�g���N
	~Fence();

	// �ҋ@
	void Wait(void);

private:
	// �t�F���X�̐���
	void CreateFence(void);


	// �L���[
	const Queue* queue;

	// �t�F���X
	ID3D12Fence1* fence;

	// �C�x���g�n���h��
	void* handle;

	// �t�F���X�J�E���g
	unsigned __int64 cnt;
};
