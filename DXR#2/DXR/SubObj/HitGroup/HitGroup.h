#pragma once
#include "../SubObj.h"
#include <string>

struct D3D12_HIT_GROUP_DESC;

class HitGroup :
	public SubObj
{
public:
	// �R���X�g���N�^
	HitGroup(const std::string& hitName, const std::string& closestHit);
	// �f�X�g���N�^
	~HitGroup();
	
private:
	// �T�u�I�u�W�F�N�g�̐ݒ�
	void SetSubObj(void);


	// �q�b�g�O���[�v��
	std::wstring hitName;

	// �N���[�[�X�g��
	std::wstring closest;

	// �q�b�g�O���[�v���
	std::unique_ptr<D3D12_HIT_GROUP_DESC>desc;
};
