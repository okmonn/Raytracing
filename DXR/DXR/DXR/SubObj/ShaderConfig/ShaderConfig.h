#pragma once
#include "../SubObj.h"

class ShaderConfig :
	public SubObj
{
public:
	// �R���X�g���N�^
	ShaderConfig(const size_t& payloadSize, const size_t& attributeSize = sizeof(float) * 2);
	// �f�X�g���N�^
	~ShaderConfig();
	
private:
	// �T�u�I�u�W�F�N�g�̐���
	void CreateSub(const size_t& payloadSize, const size_t& attributeSize);
};
