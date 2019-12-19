#include "HitGroup.h"
#include "../../Func/Func.h"
#include <d3d12.h>

// コンストラクタ
HitGroup::HitGroup(const std::string& hitName, const std::string& closestHit) : 
	hitName(DXR::ChangeCode(hitName)), closest(DXR::ChangeCode(closestHit)), desc(std::make_unique<D3D12_HIT_GROUP_DESC>())
{
	SetSubObj();
}

// デストラクタ
HitGroup::~HitGroup()
{
}

// サブオブジェクトの設定
void HitGroup::SetSubObj(void)
{
	desc->AnyHitShaderImport = nullptr;
	desc->ClosestHitShaderImport = closest.c_str();
	desc->HitGroupExport = hitName.c_str();
	desc->IntersectionShaderImport = nullptr;

	sub->pDesc = &(*desc);
	sub->Type = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP;
}
