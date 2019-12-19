#include "Allocator.h"
#include "../Device/Device.h"
#include <d3d12.h>
#include <crtdbg.h>

// コンストラクタ
Allocator::Allocator(const DXR::CommandType& type) :
	allo(nullptr)
{
	CreateAllo(type);
}

// デストラク
Allocator::~Allocator()
{
	if (allo != nullptr)
	{
		allo->Release();
		allo = nullptr;
	}
}

// コマンドアロケータの生成
void Allocator::CreateAllo(const DXR::CommandType& type)
{
	auto hr = Device::Get()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE(type), IID_PPV_ARGS(&allo));
	_ASSERT(hr == S_OK);
}

// コマンドアロケータ
void Allocator::Reset(void) const
{
	auto hr = allo->Reset();
	_ASSERT(hr == S_OK);
}

// コマンドアロケータの取得
ID3D12CommandAllocator* Allocator::Get(void) const
{
	return allo;
}
