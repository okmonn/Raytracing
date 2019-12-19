#include "List.h"
#include "../Device/Device.h"
#include "../Window/Window.h"
#include "../Allocator/Allocator.h"
#include "../Pipe/Pipe.h"
#include "../SubObj/Root/Root.h"
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

// コマンドリストの生成
void List::CreateList(const DXR::CommandType& type)
{
	auto hr = Device::Get()->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE(type), D3D12_COMMAND_LIST_FLAGS::D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&list));
	_ASSERT(hr == S_OK);
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
	barrier.Transition.pResource = rsc;
	barrier.Transition.StateAfter = after;
	barrier.Transition.StateBefore = befor;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	list->ResourceBarrier(1, &barrier);
}

// UAVバリア
void List::Barrier(ID3D12Resource* rsc) const
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.UAV.pResource = rsc;

	list->ResourceBarrier(1, &barrier);
}

// ビューポートのセット
void List::Viewport(const Window* win)
{
	D3D12_VIEWPORT view{};
	view.Height = float(win->WinSize().y);
	view.MaxDepth = 1.0f;
	view.Width = float(win->WinSize().x);

	list->RSSetViewports(1, &view);
}

// シザーのセット
void List::Scissor(const Window* win)
{
	RECT scissor{};
	scissor.bottom = win->WinSize().y;
	scissor.right = win->WinSize().x;

	list->RSSetScissorRects(1, &scissor);
}

// パイプラインのセット
void List::SetPipe(const Pipe* pipe)
{
	list->SetPipelineState1(pipe->Get());
}

// ルートシグネチャのセット
void List::SetRoot(const Root* root)
{
	list->SetComputeRootSignature(root->RtSig());
}

// コマンドリストの取得
ID3D12GraphicsCommandList5* List::Get(void) const
{
	return list;
}
