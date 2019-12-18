#include "Fence.h"
#include "../Device/Device.h"
#include "../Queue/Queue.h"
#include <d3d12.h>
#include <crtdbg.h>

// �R���X�g���N�^
Fence::Fence(const Queue* queue) : queue(queue),
	fence(nullptr), handle(CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS)), cnt(0)
{
	CreateFence();
}

// �f�X�g���N
Fence::~Fence()
{
	if (fence != nullptr)
	{
		fence->Release();
		fence = nullptr;
	}
}

// �t�F���X�̐���
void Fence::CreateFence(void)
{
	auto hr = Device::Get()->CreateFence(cnt, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	_ASSERT(hr == S_OK);
}

// �ҋ@
void Fence::Wait(void)
{
	auto hr = queue->Get()->Signal(fence, ++cnt);
	_ASSERT(hr == S_OK);

	if (fence->GetCompletedValue() != cnt)
	{
		hr = fence->SetEventOnCompletion(cnt, handle);
		_ASSERT(hr == S_OK);

		WaitForSingleObject(handle, INFINITE);
	}
}
