// ローカルルートシグネチャ
LocalRootSignature triangleHitLocal =
{
    "CBV(b0)"
};

// ヒットグループ
TriangleHitGroup triangleHit =
{
    "",//anyhit
    "TriangleChs"//closesthit
};

// アソシエーション
SubobjectToExportsAssociation triangleAsso =
{
	"triangleHitLocal", //サブオブジェクト名
    "triangleHit"//エクスポート関数
};

// 状態構成フラグ
StateObjectConfig triHitState =
{
	STATE_OBJECT_FLAGS_ALLOW_LOCAL_DEPENDENCIES_ON_EXTERNAL_DEFINITONS
};
