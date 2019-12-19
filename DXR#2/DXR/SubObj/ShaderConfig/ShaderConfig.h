#pragma once
#include "../SubObj.h"

struct D3D12_RAYTRACING_SHADER_CONFIG;

class ShaderConfig :
	public SubObj
{
public:
	// �R���X�g���N�^
	ShaderConfig(const size_t& payloadSize, const size_t& attributeSize = 8);
	// �f�X�g���N�^
	~ShaderConfig();

private:
	// �T�u�I�u�W�F�N�g�̐ݒ�
	void SetSubObj(const size_t& payloadSize, const size_t& attributeSize);


	// �V�F�[�_�R���t�B�O
	std::unique_ptr<D3D12_RAYTRACING_SHADER_CONFIG>config;
};
