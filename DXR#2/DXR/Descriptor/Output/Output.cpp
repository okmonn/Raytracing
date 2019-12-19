#include "Output.h"
#include "../../Window/Window.h"
#include "../Acceleration/Acceleration.h"
#include "../../Device/Device.h"
#include <d3d12.h>

// ビューの数
#define VIEW_NUM 2

// コンストラクタ
Output::Output(const Window* win, const Acceleration* top)
{
	CreateHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, VIEW_NUM, true);
	CreateRsc(win);

	size_t index = 0;
	UAV(index++);
	SRV(top, index);
}

// デストラクタ
Output::~Output()
{
}

// リソースの生成
void Output::CreateRsc(const Window* win)
{
	D3D12_RESOURCE_DESC desc{};
	desc.DepthOrArraySize = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Height = win->WinSize().y;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.MipLevels = 1;
	desc.SampleDesc = { 1, 0 };
	desc.Width = win->WinSize().x;

	Descriptor::CreateRsc(DefaultProp(), desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE);
}

// UAVの生成
void Output::UAV(const size_t& index)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2D;
	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += Device::Get()->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index;
	Device::Get()->CreateUnorderedAccessView(*rsc.begin(), nullptr, &desc, handle);
}

// SRVの生成
void Output::SRV(const Acceleration* top, const size_t& index)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.RaytracingAccelerationStructure.Location = top->Result()->GetGPUVirtualAddress();
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	desc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += Device::Get()->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index;
	Device::Get()->CreateShaderResourceView(nullptr, &desc, handle);
}
