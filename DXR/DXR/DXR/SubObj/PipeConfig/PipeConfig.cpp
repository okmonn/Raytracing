#include "PipeConfig.h"
#include <d3d12.h>

// コンストラクタ
PipeConfig::PipeConfig(const size_t& depth)
{
	CreateSub(depth);
}

// デストラクタ
PipeConfig::~PipeConfig()
{
}

// サブオブジェクトの生成
void PipeConfig::CreateSub(const size_t& depth)
{
	D3D12_RAYTRACING_PIPELINE_CONFIG config{};
	config.MaxTraceRecursionDepth = unsigned int(depth);

	(*sub).pDesc = &config;
	(*sub).Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG;
}
