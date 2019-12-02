#pragma once
#include "../SubObj.h"
#include <string>

struct D3D12_HIT_GROUP_DESC;

class Hit :
	public SubObj
{
public:
	// �R���X�g���N�^
	Hit(const std::string& hitName, const char* closest, const char* any = nullptr);
	// �f�X�g���N�^
	~Hit();

private:
	// �T�u�I�u�W�F�N�g�̐���
	void CreateSub(const wchar_t* closest, const wchar_t* any);


	// �q�b�g�O���[�v���
	std::unique_ptr<D3D12_HIT_GROUP_DESC>group;

	// �q�b�g�O���[�v��
	std::wstring hitName;
};
