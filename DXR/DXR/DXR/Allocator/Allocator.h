#pragma once
#include "../Information.h"

struct ID3D12CommandAllocator;

class Allocator
{
public:
	// �R���X�g���N�^
	Allocator(const DXR::CommandType& type);
	// �f�X�g���N
	~Allocator();

	// �R�}���h�A���P�[�^
	void Reset(void) const;

	// �R�}���h�A���P�[�^�̎擾
	ID3D12CommandAllocator* Get(void) const;

private:
	// �R�}���h�A���P�[�^�̐���
	void CreateAllo(const DXR::CommandType& type);


	// �R�}���h�A���P�[�^
	ID3D12CommandAllocator* allo;
};
