#include "HitGroup.h"
#include "../../Func/Func.h"
#include <d3d12.h>

// �R���X�g���N�^
HitGroup::HitGroup(const std::string& hitName, const std::string& closestHit) : 
	hitName(DXR::ChangeCode(hitName)), closest(DXR::ChangeCode(closestHit)), desc(std::make_unique<D3D12_HIT_GROUP_DESC>())
{
	SetSubObj();
}

// �f�X�g���N�^
HitGroup::~HitGroup()
{
}

// �T�u�I�u�W�F�N�g�̐ݒ�
void HitGroup::SetSubObj(void)
{
	desc->AnyHitShaderImport = nullptr;
	desc->ClosestHitShaderImport = closest.c_str();
	desc->HitGroupExport = hitName.c_str();
	desc->IntersectionShaderImport = nullptr;

	sub->pDesc = &(*desc);
	sub->Type = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP;
}
