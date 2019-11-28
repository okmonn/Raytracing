#include "List.h"
#include "../Device/Device.h"
#include <d3d12.h>
#include <crtdbg.h>

// コンストラクタ
List::List(const DXR::CommandType& type) : 
	list(nullptr)
{
	CreateList(type);
}

// デストラク
List::~List()
{
	if (list != nullptr)
	{
		list->Release();
		list = nullptr;
	}
}

// コマンドリストの生成
void List::CreateList(const DXR::CommandType& type)
{
	auto hr = Device::Get()->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE(type), D3D12_COMMAND_LIST_FLAGS::D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&list));
	_ASSERT(hr == S_OK);
}

// コマンドリストの取得
ID3D12GraphicsCommandList5* List::Get(void) const
{
	return list;
}
