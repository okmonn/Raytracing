#include "ShaderConfig.h"
#include <d3d12.h>

// �R���X�g���N�^
ShaderConfig::ShaderConfig(const size_t& payloadSize, const size_t& attributeSize)
{
	CreateSub(attributeSize, payloadSize);
}

// �f�X�g���N�^
ShaderConfig::~ShaderConfig()
{
}

// �T�u�I�u�W�F�N�g�̐���
void ShaderConfig::CreateSub(const size_t& payloadSize, const size_t& attributeSize)
{
	D3D12_RAYTRACING_SHADER_CONFIG config{};
	config.MaxAttributeSizeInBytes = unsigned int(attributeSize);
	config.MaxPayloadSizeInBytes   = unsigned int(payloadSize);

	(*sub).pDesc = &config;
	(*sub).Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG;
}
