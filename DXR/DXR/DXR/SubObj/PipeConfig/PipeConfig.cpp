#include "PipeConfig.h"
#include <d3d12.h>

// �R���X�g���N�^
PipeConfig::PipeConfig(const size_t& depth)
{
	CreateSub(depth);
}

// �f�X�g���N�^
PipeConfig::~PipeConfig()
{
}

// �T�u�I�u�W�F�N�g�̐���
void PipeConfig::CreateSub(const size_t& depth)
{
	D3D12_RAYTRACING_PIPELINE_CONFIG config{};
	config.MaxTraceRecursionDepth = unsigned int(depth);

	(*sub).pDesc = &config;
	(*sub).Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG;
}
