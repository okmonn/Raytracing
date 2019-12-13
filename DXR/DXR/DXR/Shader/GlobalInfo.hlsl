// グローバルルートシグネチャ
GlobalRootSignature global =
{
    " "
};

// シェーダコンフィグ
RaytracingShaderConfig sConfig =
{
    16, //ペイロードサイズ
    8 //アトリビュートサイズ
};

// パイプラインコンフィグ
RaytracingPipelineConfig pConfig =
{
    1 //クローゼストヒットの回数
};

// 状態構成フラグ
StateObjectConfig globalState =
{
	//外部に提供する場合
	STATE_OBJECT_FLAGS_ALLOW_EXTERNAL_DEPENDENCIES_ON_LOCAL_DEFINITIONS
	
	//外部の提供を受けいれる場合
	//STATE_OBJECT_FLAGS_ALLOW_LOCAL_DEPENDENCIES_ON_EXTERNAL_DEFINITONS
};