#include "Queue.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include <vector>
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

// コマンドの実行
void Queue::Execution(const List* list, const size_t& num)
{
	std::vector<ID3D12CommandList*>tmp(num);
	unsigned int index = 0;
	for (auto& i : tmp)
	{
		i = list[index].Get();
	}

	queue->ExecuteCommandLists(unsigned int(tmp.size()), tmp.data());
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
