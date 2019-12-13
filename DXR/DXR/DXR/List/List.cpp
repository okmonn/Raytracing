#include "List.h"
#include "../Device/Device.h"
#include "../Allocator/Allocator.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../Descriptor/Descriptor.h"
#include <vector>
#include <d3d12.h>
#include <crtdbg.h>

// コンストラクタ
List::List(const DXR::CommandType& type) : 
	list(nullptr)
{
	CreateList(type);
}

// デストラク
List::~List()
{
	if (list != nullptr)
	{
		list->Release();
		list = nullptr;
	}
}

// コマンドリストのリセット
void List::Reset(const Allocator* allo) const
{
	auto hr = list->Reset(allo->Get(), nullptr);
	_ASSERT(hr == S_OK);
}

// コマンドリストのクローズ
void List::Close(void) const
{
	auto hr = list->Close();
	_ASSERT(hr == S_OK);
}

// バリア
void List::Barrier(ID3D12Resource* rsc, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after) const
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Transition.pResource   = rsc;
	barrier.Transition.StateAfter  = after;
	barrier.Transition.StateBefore = befor;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	list->ResourceBarrier(1, &barrier);
}

// UAVバリア
void List::Barrier(ID3D12Resource* rsc) const
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type          = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.UAV.pResource = rsc;

	list->ResourceBarrier(1, &barrier);
}

// ルートシグネチャのセット
void List::SetRoot(const Root* root)
{
	list->SetComputeRootSignature(root->Get());
}

// パイプラインのセット
void List::SetPipe(const Pipe* pipe)
{
	list->SetPipelineState1(pipe->Get());
}

// ディスクリプターヒープのセット
void List::SetHeap(const Descriptor* descriptor, const size_t& num)
{
	std::vector<ID3D12DescriptorHeap*>heap(num);
	unsigned int index = 0;
	for (auto& i : heap)
	{
		i = descriptor[index++].Heap();
	}

	list->SetDescriptorHeaps(unsigned int(heap.size()), heap.data());
}

// コマンドリストの生成
void List::CreateList(const DXR::CommandType& type)
{
	auto hr = Device::Get()->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE(type), D3D12_COMMAND_LIST_FLAGS::D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&list));
	_ASSERT(hr == S_OK);
}

// コマンドリストの取得
ID3D12GraphicsCommandList5* List::Get(void) const
{
	return list;
}
