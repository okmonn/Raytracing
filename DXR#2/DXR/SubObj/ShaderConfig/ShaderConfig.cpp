#include "ShaderConfig.h"
#include <d3d12.h>

// �R���X�g���N�^
ShaderConfig::ShaderConfig(const size_t& payloadSize, const size_t& attributeSize) :
	config(std::make_unique<D3D12_RAYTRACING_SHADER_CONFIG>())
{
	SetSubObj(payloadSize, attributeSize);
}

// �f�X�g���N�^
ShaderConfig::~ShaderConfig()
{
}

// �T�u�I�u�W�F�N�g�̐ݒ�
void ShaderConfig::SetSubObj(const size_t& payloadSize, const size_t& attributeSize)
{
	config->MaxAttributeSizeInBytes = attributeSize;
	config->MaxPayloadSizeInBytes = payloadSize;

	sub->pDesc = &(*config);
	sub->Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG;
}
