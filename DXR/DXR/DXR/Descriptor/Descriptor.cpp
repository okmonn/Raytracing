#include "Descriptor.h"
#include "../Device/Device.h"
#include <d3d12.h>
#include <crtdbg.h>

// コンストラクタ
Descriptor::Descriptor() : 
	heap(nullptr)
{
}

// デストラク
Descriptor::~Descriptor()
{
}

// ヒープの生成
void Descriptor::CreateHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const size_t& rscNum, const bool& shader)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	if (shader == false)
	{
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	}
	else
	{
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	}
	desc.NumDescriptors = unsigned int(rscNum);
	desc.Type           = type;

	auto hr = Device::Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	_ASSERT(hr == S_OK);
}

// リソースの生成
void Descriptor::CreateRsc(const D3D12_HEAP_PROPERTIES& prop, const D3D12_RESOURCE_DESC& desc, const D3D12_RESOURCE_STATES& state, 
	const size_t& index, const D3D12_HEAP_FLAGS& flag, const D3D12_CLEAR_VALUE* clear)
{
	auto hr = Device::Get()->CreateCommittedResource1(&prop, flag, &desc, state, clear, nullptr, IID_PPV_ARGS(&rsc[index]));
	_ASSERT(hr == S_OK);
}

// ヒープの取得
ID3D12DescriptorHeap* Descriptor::Heap(void) const
{
	return heap;
}

// リソースの取得
ID3D12Resource* Descriptor::Rsc(const size_t& index) const
{
	return rsc[index];
}

// リソースの取得
std::vector<ID3D12Resource*> Descriptor::Rsc(void) const
{
	return rsc;
}
