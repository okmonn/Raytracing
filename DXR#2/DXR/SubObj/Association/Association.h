#pragma once
#include "../SubObj.h"
#include <string>
#include <vector>

struct D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION;

class Association :
	public SubObj
{
public:
	// �R���X�g���N�^
	Association(const SubObj* obj, const std::initializer_list<const wchar_t*>& exportName);
	// �f�X�g���N�^
	~Association();

private:
	// �T�u�I�u�W�F�N�g�̐ݒ�
	void SetSubObj(const SubObj* obj, const std::initializer_list<const wchar_t*>& exportName);


	// �֘A�t��
	std::unique_ptr<D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION>asso;

	// �G�N�X�|�[�g��
	std::vector<const wchar_t*>name;
};
