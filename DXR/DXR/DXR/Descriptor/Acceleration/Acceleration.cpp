#include "Acceleration.h"
#include "../../Device/Device.h"
#include "../../List/List.h"
#include "../Primitive/Primitive.h"
#include <d3d12.h>
#include <crtdbg.h>

// ボトムレベルのリソース数
#define BOTTOM_RSC_NUM 2
// トップレベルのリソース数
#define TOP_RSC_NUM 3

// コンストラクタ
Acceleration::Acceleration(const List* list, const Primitive* prim) : prim(prim),
	list(nullptr), buf(nullptr), geo(nullptr), input(std::make_unique<D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS>())
{
	rsc.resize(BOTTOM_RSC_NUM);

	CreateBottom();
	Build(list);
}

// コンストラクタ
Acceleration::Acceleration(const List* list, const Acceleration* bottom, const size_t& bottomNum) : list(list),
	prim(nullptr), buf(nullptr), geo(nullptr), input(std::make_unique<D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS>())
{
	rsc.resize(TOP_RSC_NUM);

	CreateTop(bottom, bottomNum);
	Build(list);
}

// デストラクタ
Acceleration::~Acceleration()
{
	UnMap(rsc.size() - 1);
}

// ボトムレベルの生成
void Acceleration::CreateBottom(void)
{
	geo = std::make_unique<D3D12_RAYTRACING_GEOMETRY_DESC>();
	geo->Flags                                = D3D12_RAYTRACING_GEOMETRY_FLAGS::D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
	geo->Triangles.VertexBuffer.StartAddress  = prim->Get()->GetGPUVirtualAddress();
	geo->Triangles.VertexBuffer.StrideInBytes = sizeof(Vec3f);
	geo->Triangles.VertexCount                = unsigned int(prim->Get()->GetDesc().Width / geo->Triangles.VertexBuffer.StrideInBytes);
	geo->Triangles.VertexFormat               = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	geo->Type                                 = D3D12_RAYTRACING_GEOMETRY_TYPE::D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;

	input->DescsLayout    = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
	input->Flags          = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
	input->NumDescs       = 1;
	input->pGeometryDescs = &(*geo);
	input->Type           = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info{};
	Device::Get()->GetRaytracingAccelerationStructurePrebuildInfo(&(*input), &info);

	D3D12_RESOURCE_DESC desc{};
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height           = 1;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = info.ScratchDataSizeInBytes;

	size_t index = rsc.size() - 1;
	CreateRsc(DefaultProp(), desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, index--);

	desc.Width = info.ResultDataMaxSizeInBytes;
	CreateRsc(DefaultProp(), desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, index);
}

// トップレベルの生成
void Acceleration::CreateTop(const Acceleration* bottom, const size_t& bottomNum)
{
	size_t instanceNum = 0;
	this->bottom.resize(bottomNum);
	for (size_t i = 0; i < bottomNum; ++i)
	{
		this->bottom[i] = &bottom[i];
		instanceNum += bottom[i].prim->InstanceNum();
	}

	D3D12_RESOURCE_DESC desc{};
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height           = 1;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = sizeof(D3D12_RAYTRACING_INSTANCE_DESC) * instanceNum;

	size_t index = rsc.size() - 1;
	CreateRsc(UploadProp(), desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, index);
	Map((void**)&buf, index--);

	input->DescsLayout   = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
	input->Flags         = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE;
	input->InstanceDescs = (*rsc.rbegin())->GetGPUVirtualAddress();
	input->NumDescs      = unsigned int(instanceNum);
	input->Type          = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info{};
	Device::Get()->GetRaytracingAccelerationStructurePrebuildInfo(&(*input), &info);

	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	desc.Width = info.ScratchDataSizeInBytes;
	CreateRsc(DefaultProp(), desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, index--);

	desc.Width = info.ResultDataMaxSizeInBytes;
	CreateRsc(DefaultProp(), desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, index);
}

// ビルド
void Acceleration::Build(const List* list)
{
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC desc{};
	desc.DestAccelerationStructureData    = Result()->GetGPUVirtualAddress();
	desc.Inputs                           = Input();
	desc.ScratchAccelerationStructureData = Scratch()->GetGPUVirtualAddress();

	list->Get()->BuildRaytracingAccelerationStructure(&desc, 0, nullptr);
	list->Barrier(Result());
}

// トップレベルの更新
void Acceleration::UpData(const size_t& rayNum)
{
	list->Barrier(Result());

	for (size_t i = 0; i < bottom.size(); ++i)
	{
		for (size_t n = 0; n < bottom[i]->prim->InstanceNum(); ++n)
		{
			size_t index = i * bottom.size() + n;
			buf[index].AccelerationStructure               = bottom[i]->Result()->GetGPUVirtualAddress();
			buf[index].Flags                               = D3D12_RAYTRACING_INSTANCE_FLAGS::D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
			buf[index].InstanceContributionToHitGroupIndex = unsigned int(index * rayNum);
			buf[index].InstanceID                          = unsigned int(index);
			buf[index].InstanceMask                        = 0xff;

			std::memcpy((void*)buf[index].Transform, bottom[i]->prim->Matrix(n).mat, sizeof(bottom[i]->prim->Matrix(n).mat));
		}
	}

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC desc{};
	desc.DestAccelerationStructureData    = Result()->GetGPUVirtualAddress();
	desc.Inputs                           = Input();
	desc.Inputs.Flags                    |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE;
	desc.Inputs.InstanceDescs             = Instance()->GetGPUVirtualAddress();
	desc.ScratchAccelerationStructureData = Scratch()->GetGPUVirtualAddress();
	desc.SourceAccelerationStructureData  = Result()->GetGPUVirtualAddress();

	list->Get()->BuildRaytracingAccelerationStructure(&desc, 0, nullptr);
	list->Barrier(Result());
}

// リザルトリソースの取得
ID3D12Resource* Acceleration::Result(void) const
{
	return *rsc.begin();
}

// スクラッチリソースの取得
ID3D12Resource* Acceleration::Scratch(void) const
{
	return rsc[1];
}

// インスタンスリソースの取得
ID3D12Resource* Acceleration::Instance(void) const
{
	return *rsc.rbegin();
}

// 加速構造入力情報の取得
D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS Acceleration::Input(void) const
{
	return *input;
}
