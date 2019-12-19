#include "Render.h"
#include "../../List/List.h"
#include "../../Swap/Swap.h"
#include "../../Device/Device.h"
#include <memory>
#include <d3d12.h>
#include <crtdbg.h>
#include <dxgi1_6.h>

// コンストラクタ
Render::Render(const Swap* swap) : 
	swap(swap)
{
	rsc.resize(swap->BufferNum());
	CreateHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, rsc.size());
	CreateRenderRsc();
}

// デストラクタ
Render::~Render()
{
}

// レンダーリソースの生成
void Render::CreateRenderRsc(void)
{
	for (unsigned int i = 0; i < swap->BufferNum(); ++i)
	{
		auto hr = swap->Get()->GetBuffer(i, IID_PPV_ARGS(&rsc[i]));
		_ASSERT(hr == S_OK);

		RTV(i);
	}
}

// RTVの生成
void Render::RTV(const size_t& index)
{
	D3D12_RENDER_TARGET_VIEW_DESC desc{};
	desc.Format = rsc[index]->GetDesc().Format;
	desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D; 

	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += Device::Get()->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index;
	Device::Get()->CreateRenderTargetView(rsc[index], &desc, handle);
}

// クリア
void Render::Clear(const List* list, const float* color, ID3D12DescriptorHeap* depth)
{
	auto rtv = heap->GetCPUDescriptorHandleForHeapStart();
	rtv.ptr += Device::Get()->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * swap->Index();

	std::unique_ptr<D3D12_CPU_DESCRIPTOR_HANDLE>dsv = nullptr;
	if (depth != nullptr)
	{
		dsv = std::make_unique<D3D12_CPU_DESCRIPTOR_HANDLE>();
		(*dsv) = depth->GetCPUDescriptorHandleForHeapStart();
	}

	list->Get()->OMSetRenderTargets(1, &rtv, false, &(*dsv));
	list->Get()->ClearRenderTargetView(rtv, color, 0, nullptr);
}

// リソースの取得
ID3D12Resource* Render::Rsc(void) const
{
	return rsc[swap->Index()];
}
