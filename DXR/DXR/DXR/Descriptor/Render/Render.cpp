#include "Render.h"
#include "../../Swap/Swap.h"
#include <d3d12.h>
#include <crtdbg.h>
#include <dxgi1_6.h>

// コンストラクタ
Render::Render(const Swap* swap)
{
	rsc.resize(swap->BufferNum());
	CreateHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, rsc.size());
	CreateRenderRsc(swap);
}

// デストラクタ
Render::~Render()
{
}

// レンダーリソースの生成
void Render::CreateRenderRsc(const Swap* swap)
{
	for (unsigned int i = 0; i < swap->BufferNum(); ++i)
	{
		auto hr = swap->Get()->GetBuffer(i, IID_PPV_ARGS(&rsc[i]));
		_ASSERT(hr == S_OK);
	}
}
