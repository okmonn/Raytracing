#include "Association.h"
#include "../../Func/Func.h"
#include <vector>
#include <d3d12.h>

// コンストラクタ
Association::Association(const SubObj* obj, const std::initializer_list<const char*>& name)
{
	std::vector<const char*>tmp;
	for (auto& i : name)
	{
		tmp.push_back(i);
	}

	CreateSub(obj, DXR::ChangeCode(tmp.data(), tmp.size()), tmp.size());
}

// デストラクタ
Association::~Association()
{
}

// サブオブジェクトの生成
void Association::CreateSub(const SubObj* obj, const wchar_t** name, const size_t& num)
{
	D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION expo{};
	expo.NumExports            = unsigned int(num);
	expo.pExports              = name;
	expo.pSubobjectToAssociate = &obj->Sub();

	(*sub).pDesc = &expo;
	(*sub).Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
}
