#pragma once
#include <memory>

struct D3D12_STATE_SUBOBJECT;

class SubObj
{
public:
	// �R���X�g���N�^
	SubObj();
	// �f�X�g���N�^
	virtual ~SubObj();

	// �T�u�I�u�W�F�N�g�̎擾
	D3D12_STATE_SUBOBJECT& Get(void) const;

protected:
	// �T�u�I�u�W�F�N�g
	std::unique_ptr<D3D12_STATE_SUBOBJECT>sub;
};
