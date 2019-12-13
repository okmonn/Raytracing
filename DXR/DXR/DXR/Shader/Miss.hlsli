// ローカルルートシグネチャ
LocalRootSignature missLocal =
{
    " "
};

// アソシエーション
SubobjectToExportsAssociation missAsso =
{
	"missLocal", //サブオブジェクト名
    "Miss"//エクスポート関数
};

// 状態構成フラグ
StateObjectConfig missState =
{
	STATE_OBJECT_FLAGS_ALLOW_LOCAL_DEPENDENCIES_ON_EXTERNAL_DEFINITONS
};