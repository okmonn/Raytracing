// ローカルルートシグネチャ
LocalRootSignature local =
{
    "CBV(b0)"
};

// ヒットグループ
TriangleHitGroup hit =
{
    "",//anyhit
    "Main"//closesthit
};

// アソシエーション
SubobjectToExportsAssociation association =
{
    "local", //サブオブジェクト名
    "TriangleHit"//エクスポート関数
};
