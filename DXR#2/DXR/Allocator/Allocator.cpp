#include "Allocator.h"
#include "../Device/Device.h"
#include <d3d12.h>
#include <crtdbg.h>

// �R���X�g���N�^
Allocator::Allocator(const DXR::CommandType& type) :
	allo(nullptr)
{
	CreateAllo(type);
}

// �f�X�g���N
Allocator::~Allocator()
{
	if (allo != nullptr)
	{
		allo->Release();
		allo = nullptr;
	}
}

// �R�}���h�A���P�[�^�̐���
void Allocator::CreateAllo(const DXR::CommandType& type)
{
	auto hr = Device::Get()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE(type), IID_PPV_ARGS(&allo));
	_ASSERT(hr == S_OK);
}

// �R�}���h�A���P�[�^
void Allocator::Reset(void) const
{
	auto hr = allo->Reset();
	_ASSERT(hr == S_OK);
}

// �R�}���h�A���P�[�^�̎擾
ID3D12CommandAllocator* Allocator::Get(void) const
{
	return allo;
}
