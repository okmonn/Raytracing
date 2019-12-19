#pragma once
#include "../SubObj.h"

struct D3D12_RAYTRACING_PIPELINE_CONFIG;

class PipeConfig :
	public SubObj
{
public:
	// コンストラクタ
	PipeConfig(const size_t& traceNum);
	// デストラクタ
	~PipeConfig();

private:
	// サブオブジェクトの設定
	void SetSubObj(const size_t& traceNum);


	// パイプラインコンフィグ
	std::unique_ptr<D3D12_RAYTRACING_PIPELINE_CONFIG>config;
};
