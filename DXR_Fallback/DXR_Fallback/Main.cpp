#include "Main.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "FallbackLayer.lib")
#pragma comment(lib, "WinPixEventRuntime.lib")

namespace {
	const float color[] = {
		1.0f, 1.0f, 1.0f, 1.0f, 
	};
}

int main() {
	CreateWnd(&win, 640, 480);
	CreateDevice(&dev);
	CreateDevice(&f_dev, dev.Get());
	CreateAllocator(&allo, dev.Get());
	CreateQueue(&queue, dev.Get());
	CreateList(&list, dev.Get());
	CreateList(&f_list, f_dev.Get(), list.Get());
	CreateFence(&fence, dev.Get());
	CreateSwapChain(&swap, win, queue.Get());
	rtvRsc.resize(GetBackBuffNum(swap.Get()));
	CreateHeap(&rtvHeap, dev.Get(), D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, std::uint32_t(rtvRsc.size()));
	for (std::uint32_t i = 0; i < rtvRsc.size(); ++i) {
		CreateRsc(&rtvRsc[i], swap.Get(), i);
		RTV(rtvRsc[i].Get(), dev.Get(), rtvHeap.Get(), i);
	}
	
	while (CheckMsg() && !(GetKeyState(VK_ESCAPE) & 0x80)) {
		Reset(allo.Get());
		Reset(list.Get(), allo.Get());

		Barrier(rtvRsc[GetBuffIndex(swap.Get())].Get(), list.Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
		Clear(rtvHeap.Get(), dev.Get(), list.Get(), GetBuffIndex(swap.Get()), color);
		Barrier(rtvRsc[GetBuffIndex(swap.Get())].Get(), list.Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);
		
		Close(list.Get());
		Execution(queue.Get(), { list.Get() });
		Wait(fence.Get(), queue.Get(), swap.Get(), fenceCnt);
	}

	return 0;
}

