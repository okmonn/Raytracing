#include "SubObj.h"
#include <d3d12.h>

// �R���X�g���N�^
SubObj::SubObj() : 
	sub(std::make_unique<D3D12_STATE_SUBOBJECT>())
{
}

// �f�X�g���N�^
SubObj::~SubObj()
{
}

// �T�u�I�u�W�F�N�g�̎擾
D3D12_STATE_SUBOBJECT SubObj::Sub(void)
{
	return *sub;
}
