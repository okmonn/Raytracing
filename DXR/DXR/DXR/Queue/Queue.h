#pragma once
#include "../Information.h"

struct ID3D12CommandQueue;

class Queue
{
public:
	// �R���X�g���N�^
	Queue(const DXR::CommandType& type);
	// �f�X�g���N
	~Queue();

	// �R�}���h�L���[�̎擾
	ID3D12CommandQueue* Get(void);

private:
	// �R�}���h�L���[�̐���
	void CreateQueue(const DXR::CommandType& type);


	// �R�}���h�L���[
	ID3D12CommandQueue* queue;
};
