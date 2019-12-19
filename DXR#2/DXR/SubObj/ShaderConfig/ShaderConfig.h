#pragma once
#include "../SubObj.h"

struct D3D12_RAYTRACING_SHADER_CONFIG;

class ShaderConfig :
	public SubObj
{
public:
	// コンストラクタ
	ShaderConfig(const size_t& payloadSize, const size_t& attributeSize = 8);
	// デストラクタ
	~ShaderConfig();

private:
	// サブオブジェクトの設定
	void SetSubObj(const size_t& payloadSize, const size_t& attributeSize);


	// シェーダコンフィグ
	std::unique_ptr<D3D12_RAYTRACING_SHADER_CONFIG>config;
};
