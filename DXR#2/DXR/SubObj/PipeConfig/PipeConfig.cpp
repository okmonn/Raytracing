#include "PipeConfig.h"
#include <d3d12.h>

// �R���X�g���N�^
PipeConfig::PipeConfig(const size_t& traceNum) : 
	config(std::make_unique<D3D12_RAYTRACING_PIPELINE_CONFIG>())
{
	SetSubObj(traceNum);
}

// �f�X�g���N�^
PipeConfig::~PipeConfig()
{
}

// �T�u�I�u�W�F�N�g�̐ݒ�
void PipeConfig::SetSubObj(const size_t& traceNum)
{
	config->MaxTraceRecursionDepth = traceNum;

	sub->pDesc = &(*config);
	sub->Type = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG;
}
