#include "Descriptor.h"
#include "../Device/Device.h"
#include <d3d12.h>
#include <crtdbg.h>

// コンストラクタ
Descriptor::Descriptor() :
	heap(nullptr)
{
	rsc.resize(1);
}

// デストラク
Descriptor::~Descriptor()
{
	for (auto& i : rsc)
	{
		if (i != nullptr)
		{
			i->Release();
			i = nullptr;
		}
	}
	if (heap != nullptr)
	{
		heap->Release();
		heap = nullptr;
	}
}

// ヒープの生成
void Descriptor::CreateHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const size_t& rscNum, const bool& shader)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags = (shader == false) 
		? D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE 
		: D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NumDescriptors = unsigned int(rscNum);
	desc.Type = type;

	auto hr = Device::Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	_ASSERT(hr == S_OK);
}

// デフォルトプロパティの取得
D3D12_HEAP_PROPERTIES Descriptor::DefaultProp(void)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;

	return prop;
}

// アップロードプロパティの取得
D3D12_HEAP_PROPERTIES Descriptor::UploadProp(void)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;

	return prop;
}

// リソースの生成
void Descriptor::CreateRsc(const D3D12_HEAP_PROPERTIES& prop, const D3D12_RESOURCE_DESC& desc, const D3D12_RESOURCE_STATES& state,
	const size_t& index, const D3D12_HEAP_FLAGS& flag, const D3D12_CLEAR_VALUE* clear)
{
	auto hr = Device::Get()->CreateCommittedResource1(&prop, flag, &desc, state, clear, nullptr, IID_PPV_ARGS(&rsc[index]));
	_ASSERT(hr == S_OK);
}

// マップ
void Descriptor::Map(void** buf, const size_t& index)
{
	D3D12_RANGE range{ 0, 1 };
	auto hr = rsc[index]->Map(0, &range, &(*buf));
	_ASSERT(hr == S_OK);
}

// アンマップ
void Descriptor::UnMap(const size_t& index)
{
	D3D12_RANGE range{ 0, 1 };
	rsc[index]->Unmap(0, &range);
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
