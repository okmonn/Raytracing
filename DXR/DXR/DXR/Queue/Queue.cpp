#include "Queue.h"
#include "../Device/Device.h"
#include <d3d12.h>
#include <crtdbg.h>

// コンストラクタ
Queue::Queue(const DXR::CommandType& type) : 
	queue(nullptr)
{
	CreateQueue(type);
}

// デストラク
Queue::~Queue()
{
	if (queue != nullptr)
	{
		queue->Release();
		queue = nullptr;
	}
}

// コマンドキューの生成
void Queue::CreateQueue(const DXR::CommandType& type)
{
	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Flags    = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type     = D3D12_COMMAND_LIST_TYPE(type);

	auto hr = Device::Get()->CreateCommandQueue(&desc, IID_PPV_ARGS(&queue));
	_ASSERT(hr == S_OK);
}

// コマンドキューの取得
ID3D12CommandQueue* Queue::Get(void) const
{
	return queue;
}
