// グローバルルートシグネチャ
GlobalRootSignature global =
{
	" "
};

// レイジェネレーション用ルートシグネチャ
LocalRootSignature rayGen =
{
	"DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), visibility = SHADER_VISIBILITY_ALL),"
    "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), visibility = SHADER_VISIBILITY_ALL)"
};

// レイジェネレーションの関連付け
SubobjectToExportsAssociation rayGenAsso =
{
	"rayGen",//サブオブジェクト名
	"RayGen"//関連付け関数
};

// ミス用ルートシグネチャ
LocalRootSignature miss =
{
	" "
};

// ミスの関連付け
SubobjectToExportsAssociation missAsso =
{
	"miss",//サブオブジェクト名
	"Miss",//関連付け関数
};

// クローゼストヒット用ルートシグネチャ
LocalRootSignature closest =
{
	" "
};

// ヒットグループ
TriangleHitGroup Hit =
{
	"", //anyhit
	"Chs"//closesthit
};

// クローゼストヒットの関連付け
SubobjectToExportsAssociation closestAsso =
{
	"closest", //サブオブジェクト名
	"Hit"//関連付け関数
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
	0 //クローゼストヒットの回数
};

// 加速構造の情報
RaytracingAccelerationStructure acceleration : register(t0);
// 出力テクスチャ
RWTexture2D<float4> output : register(u0);

// ペイロード
struct Payload
{
	bool hit;
};

float3 linearToSrgb(float3 c)
{
    // Based on http://chilliant.blogspot.com/2012/08/srgb-approximations-for-hlsl.html
	float3 sq1 = sqrt(c);
	float3 sq2 = sqrt(sq1);
	float3 sq3 = sqrt(sq2);
	float3 srgb = 0.662002687 * sq1 + 0.684122060 * sq2 - 0.323583601 * sq3 - 0.0225411470 * c;
	return srgb;
}

// レイジェネレーション
[shader("raygeneration")]
void RayGen()
{
	uint3 index = DispatchRaysIndex();
	output[index.xy] = float4(linearToSrgb(float3(0.4, 0.6, 0.2)), 1.0);
}

// ミス
[shader("miss")]
void Miss(inout Payload payload)
{
	payload.hit = false;
}

// クローゼストヒット
[shader("closesthit")]
void Chs(inout Payload payload, in BuiltInTriangleIntersectionAttributes attribute)
{
	payload.hit = true;
}
