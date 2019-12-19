#include "PipeConfig.h"
#include <d3d12.h>

// コンストラクタ
PipeConfig::PipeConfig(const size_t& traceNum) : 
	config(std::make_unique<D3D12_RAYTRACING_PIPELINE_CONFIG>())
{
	SetSubObj(traceNum);
}

// デストラクタ
PipeConfig::~PipeConfig()
{
}

// サブオブジェクトの設定
void PipeConfig::SetSubObj(const size_t& traceNum)
{
	config->MaxTraceRecursionDepth = traceNum;

	sub->pDesc = &(*config);
	sub->Type = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG;
}
