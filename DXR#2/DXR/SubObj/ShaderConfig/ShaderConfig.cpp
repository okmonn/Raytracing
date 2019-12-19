#include "ShaderConfig.h"
#include <d3d12.h>

// コンストラクタ
ShaderConfig::ShaderConfig(const size_t& payloadSize, const size_t& attributeSize) :
	config(std::make_unique<D3D12_RAYTRACING_SHADER_CONFIG>())
{
	SetSubObj(payloadSize, attributeSize);
}

// デストラクタ
ShaderConfig::~ShaderConfig()
{
}

// サブオブジェクトの設定
void ShaderConfig::SetSubObj(const size_t& payloadSize, const size_t& attributeSize)
{
	config->MaxAttributeSizeInBytes = attributeSize;
	config->MaxPayloadSizeInBytes = payloadSize;

	sub->pDesc = &(*config);
	sub->Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG;
}
