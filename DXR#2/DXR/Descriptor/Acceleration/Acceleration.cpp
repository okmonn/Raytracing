#include "Acceleration.h"
#include "../../List/List.h"
#include "../Primitive/Primitive.h"
#include "../../Device/Device.h"
#include <d3d12.h>

// リソース数
#define RSC_NUM 3

// コンストラクタ
Acceleration::Acceleration(const List* list, const Primitive* prim) :
	list(list), prim(prim)
{
	rsc.resize(RSC_NUM);

	CreateBottom();
}

// コンストラクタ
Acceleration::Acceleration(const List* list, const std::initializer_list<Acceleration*>& bottom) : 
	list(list), prim(nullptr)
{
	rsc.resize(RSC_NUM);
	this->bottom = bottom;

	CreateTop();
}

// デストラクタ
Acceleration::~Acceleration()
{
}

// ボトムレベルの生成
void Acceleration::CreateBottom(void)
{
	D3D12_RAYTRACING_GEOMETRY_DESC geo{};
	geo.Flags = D3D12_RAYTRACING_GEOMETRY_FLAGS::D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
	geo.Triangles.VertexBuffer.StartAddress  = prim->Rsc()->GetGPUVirtualAddress();
	geo.Triangles.VertexBuffer.StrideInBytes = sizeof(Vec3f);
	geo.Triangles.VertexCount = unsigned int(prim->Rsc()->GetDesc().Width / geo.Triangles.VertexBuffer.StrideInBytes);
	geo.Triangles.VertexFormat = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	geo.Type = D3D12_RAYTRACING_GEOMETRY_TYPE::D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input{};
	input.DescsLayout = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
	input.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
	input.NumDescs = 1;
	input.pGeometryDescs = &geo;
	input.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info{};
	Device::Get()->GetRaytracingAccelerationStructurePrebuildInfo(&input, &info);

	D3D12_RESOURCE_DESC desc{};
	desc.Alignment = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels = 1;
	desc.SampleDesc = { 1, 0 };
	desc.Width = info.ScratchDataSizeInBytes;

	size_t index = 0;
	CreateRsc(DefaultProp(), desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, index++);

	desc.Width = info.ResultDataMaxSizeInBytes;
	CreateRsc(DefaultProp(), desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, index);

	Build(input);
}

// トップレベルの生成
void Acceleration::CreateTop()
{
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input{};
	input.DescsLayout = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
	input.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
	input.NumDescs = 1;
	input.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info{};
	Device::Get()->GetRaytracingAccelerationStructurePrebuildInfo(&input, &info);

	D3D12_RESOURCE_DESC desc{};
	desc.Alignment = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels = 1;
	desc.SampleDesc = { 1, 0 };
	desc.Width = info.ScratchDataSizeInBytes;

	size_t index = 0;
	CreateRsc(DefaultProp(), desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, index++);

	desc.Width = info.ResultDataMaxSizeInBytes;
	CreateRsc(DefaultProp(), desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, index++);

	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Width = sizeof(D3D12_RAYTRACING_INSTANCE_DESC);
	CreateRsc(UploadProp(), desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, index);

	input.InstanceDescs = Instance()->GetGPUVirtualAddress();
	Build(input);
}

//	ビルド
void Acceleration::Build(const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& input)
{
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC desc{};
	desc.DestAccelerationStructureData = Result()->GetGPUVirtualAddress();
	desc.Inputs = input;
	desc.ScratchAccelerationStructureData = Scratch()->GetGPUVirtualAddress();

	list->Get()->BuildRaytracingAccelerationStructure(&desc, 0, nullptr);
	list->Barrier(Result());
}

// データのセット
void Acceleration::UpData(const size_t& depth)
{
	D3D12_RAYTRACING_INSTANCE_DESC* buf = nullptr;
	auto hr = Instance()->Map(0, nullptr, (void**)&buf);
	_ASSERT(hr == S_OK);
	for (size_t i = 0; i < bottom.size(); ++i)
	{
		for (size_t n = 0; n < bottom[i]->prim->InstanceNum(); ++n)
		{
			size_t index = i * bottom[i]->prim->InstanceNum() + n;
			buf[index].AccelerationStructure = bottom[i]->Result()->GetGPUVirtualAddress();
			buf[index].Flags = D3D12_RAYTRACING_INSTANCE_FLAGS::D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
			buf[index].InstanceContributionToHitGroupIndex = index * depth;
			buf[index].InstanceID = index;
			buf[index].InstanceMask = 0xff;
			std::memcpy(buf[index].Transform, bottom[i]->prim->Matrix(n).mat, sizeof(buf[index].Transform));
		}
	}
	Instance()->Unmap(0, nullptr);
}

// スクラッチリソースの取得
ID3D12Resource* Acceleration::Scratch(void) const
{
	return rsc[0];
}


ID3D12Resource* Acceleration::Result(void) const
{
	return rsc[1];
}

// インスタンスリソースの取得
ID3D12Resource* Acceleration::Instance(void) const
{
	return rsc[2];
}
