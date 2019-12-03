#include "Acceleration.h"
#include "../../Device/Device.h"
#include "../../List/List.h"
#include "../Primitive/Primitive.h"
#include <d3d12.h>

// �{�g�����x���̃��\�[�X��
#define BOTTOM_RSC_NUM 2
// �g�b�v���x���̃��\�[�X��
#define TOP_RSC_NUM 3

// �R���X�g���N�^
Acceleration::Acceleration(const List* list, const Primitive* prim) : list(list), prim(prim),
	buf(nullptr)
{
	rsc.resize(BOTTOM_RSC_NUM);

	CreateBottom();
}

// �R���X�g���N�^
Acceleration::Acceleration(const List* list, const Acceleration* bottom, const size_t& bottomNum) : list(list),
	prim(nullptr), buf(nullptr)
{
	rsc.resize(TOP_RSC_NUM);

	CreateTop(bottom, bottomNum);
}

// �f�X�g���N�^
Acceleration::~Acceleration()
{
	UnMap(rsc.size() - 1);
}

// �{�g�����x���̐���
void Acceleration::CreateBottom(void)
{
	D3D12_RAYTRACING_GEOMETRY_DESC geo{};
	geo.Flags                                = D3D12_RAYTRACING_GEOMETRY_FLAGS::D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
	geo.Triangles.VertexBuffer.StartAddress  = prim->Get()->GetGPUVirtualAddress();
	geo.Triangles.VertexBuffer.StrideInBytes = sizeof(Vec3f);
	geo.Triangles.VertexCount                = unsigned int(prim->Get()->GetDesc().Width / geo.Triangles.VertexBuffer.StrideInBytes);
	geo.Triangles.VertexFormat               = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	geo.Type                                 = D3D12_RAYTRACING_GEOMETRY_TYPE::D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input{};
	input.DescsLayout    = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
	input.Flags          = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
	input.NumDescs       = 1;
	input.pGeometryDescs = &geo;
	input.Type           = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info{};
	Device::Get()->GetRaytracingAccelerationStructurePrebuildInfo(&input, &info);

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

	Build(input);
}

// �g�b�v���x���̐���
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

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input{};
	input.DescsLayout   = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
	input.Flags         = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE;
	input.InstanceDescs = Instance()->GetGPUVirtualAddress();
	input.NumDescs      = unsigned int(instanceNum);
	input.Type          = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info{};
	Device::Get()->GetRaytracingAccelerationStructurePrebuildInfo(&input, &info);

	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	desc.Width = info.ScratchDataSizeInBytes;
	CreateRsc(DefaultProp(), desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, index--);

	desc.Width = info.ResultDataMaxSizeInBytes;
	CreateRsc(DefaultProp(), desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, index);

	Build(input);
}

// �r���h
void Acceleration::Build(const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& input)
{
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC desc{};
	desc.DestAccelerationStructureData    = Result()->GetGPUVirtualAddress();
	desc.Inputs                           = input;
	desc.ScratchAccelerationStructureData = Scratch()->GetGPUVirtualAddress();

	list->Get()->BuildRaytracingAccelerationStructure(&desc, 0, nullptr);
	list->Barrier(Result());
}

// �g�b�v���x���̍X�V
void Acceleration::UpData(const size_t& rayNum)
{
	list->Barrier(Result());

	size_t instanceNum = 0;
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

		instanceNum += bottom[i]->prim->InstanceNum();
	}

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input{};
	input.DescsLayout   = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
	input.Flags         = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE;
	input.InstanceDescs = Instance()->GetGPUVirtualAddress();
	input.NumDescs      = unsigned int(instanceNum);
	input.Type          = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC desc{};
	desc.DestAccelerationStructureData    = Result()->GetGPUVirtualAddress();
	desc.Inputs                           = input;
	desc.Inputs.InstanceDescs             = Instance()->GetGPUVirtualAddress();
	desc.ScratchAccelerationStructureData = Scratch()->GetGPUVirtualAddress();
	desc.SourceAccelerationStructureData  = Result()->GetGPUVirtualAddress();

	list->Get()->BuildRaytracingAccelerationStructure(&desc, 0, nullptr);
	list->Barrier(Result());
}

// ���U���g���\�[�X�̎擾
ID3D12Resource* Acceleration::Result(void) const
{
	return *rsc.begin();
}

// �X�N���b�`���\�[�X�̎擾
ID3D12Resource* Acceleration::Scratch(void) const
{
	return rsc[1];
}

// �C���X�^���X���\�[�X�̎擾
ID3D12Resource* Acceleration::Instance(void) const
{
	return *rsc.rbegin();
}
