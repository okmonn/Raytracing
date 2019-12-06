GlobalRootSignature global =
{
    " "
};

// ローカルルートシグネチャ
LocalRootSignature local =
{
    "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), visibility = SHADER_VISIBILITY_ALL),"
    "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), visibility = SHADER_VISIBILITY_ALL)"
};

// アソシエーション
SubobjectToExportsAssociation association =
{
    "local",//サブオブジェクト名
    "RayGen"//エクスポート関数
};

// シェーダコンフィグ
RaytracingShaderConfig sConfig =
{
    16,//ペイロードサイズ
    8//アトリビュートサイズ
};

// パイプラインコンフィグ
RaytracingPipelineConfig pConfig =
{
    0//深度の深さ
};