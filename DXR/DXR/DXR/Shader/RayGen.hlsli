// ローカルルートシグネチャ
LocalRootSignature rayGenLocal =
{
    "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), visibility = SHADER_VISIBILITY_ALL),"
    "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), visibility = SHADER_VISIBILITY_ALL)"
};

// アソシエーション
SubobjectToExportsAssociation rayGenAsso =
{
	"rayGenLocal", //サブオブジェクト名
    "RayGen"//エクスポート関数
};

// 状態構成フラグ
StateObjectConfig rayGenState =
{
	STATE_OBJECT_FLAGS_ALLOW_LOCAL_DEPENDENCIES_ON_EXTERNAL_DEFINITONS
};