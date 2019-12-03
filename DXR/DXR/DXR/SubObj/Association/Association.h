#pragma once
#include "../SubObj.h"

class Association :
	public SubObj
{
public:
	// �R���X�g���N�^
	Association(const SubObj* obj, const std::initializer_list<const char*>& name);
	// �f�X�g���N�^
	~Association();

private:
	// �T�u�I�u�W�F�N�g�̐���
	void CreateSub(const SubObj* obj, const wchar_t** name, const size_t& num);
};
