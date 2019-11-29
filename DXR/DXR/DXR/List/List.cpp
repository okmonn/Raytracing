#include "List.h"
#include "../Device/Device.h"
#include <d3d12.h>
#include <crtdbg.h>

// �R���X�g���N�^
List::List(const DXR::CommandType& type) : 
	list(nullptr)
{
	CreateList(type);
}

// �f�X�g���N
List::~List()
{
	if (list != nullptr)
	{
		list->Release();
		list = nullptr;
	}
}

// UAV�o���A
void List::Barrier(ID3D12Resource* rsc) const
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type          = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.UAV.pResource = rsc;

	list->ResourceBarrier(1, &barrier);
}

// �R�}���h���X�g�̐���
void List::CreateList(const DXR::CommandType& type)
{
	auto hr = Device::Get()->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE(type), D3D12_COMMAND_LIST_FLAGS::D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&list));
	_ASSERT(hr == S_OK);
}

// �R�}���h���X�g�̎擾
ID3D12GraphicsCommandList5* List::Get(void) const
{
	return list;
}
