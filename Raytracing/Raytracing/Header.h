#pragma once
#include <wrl.h>
#include <array>
#include <vector>
#include <d3d12.h>
#include <dxgi1_6.h>

// メモリ開放
#define Release(X) { if((X) != nullptr) (X)->Release(); (X) = nullptr; }

// ウィンドウサイズX
#define WINSIZE_X 640
// ウィンドウサイズY
#define WINSIZE_Y 480

// バックバッファーカウント
#define BACK_BUFFER 3

// 床のインスタンス数
#define PLANE_INSTANCE 1

// 三角形のインスタンス数
#define TRIANGLE_INSTANCE 3

// プリミティブ数
#define PRIMITIVE 2

// 機能レベル一覧
const D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0,
};

// フェンス
struct Fence
{
	//フェンス
	ID3D12Fence* ptr;
	//カウント
	unsigned __int32 cnt;
	//イベント
	void* event;

	Fence() {
		ptr   = nullptr;
		cnt   = 0;
		event = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
	}
};

// レンダーターゲット
struct Render
{
	//ヒープ
	ID3D12DescriptorHeap* heap;
	//リソース
	std::vector<ID3D12Resource*>rsc;

	Render() {
		heap = nullptr;
	}
};

// 加速構造
struct Acceleration
{
	ID3D12Resource* scratch;
	ID3D12Resource* result;
	ID3D12Resource* instance;
	D3D12_RAYTRACING_GEOMETRY_DESC geoDesc;
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input;

	Acceleration() {
		scratch = result = instance = nullptr;
		geoDesc = {};
		input = {};
	}
};

// シェーダ情報
struct ShaderInfo
{
	D3D12_DXIL_LIBRARY_DESC desc;
	D3D12_STATE_SUBOBJECT sub;
	ID3DBlob* blob;
	std::vector<D3D12_EXPORT_DESC>expo;
	std::vector<std::wstring>name;

	ShaderInfo() : blob(nullptr) {
		desc = {};
		sub  = {};
	}
	~ShaderInfo() {
		//Release(blob);
	}
};

// ルートシグネチャ情報
struct RootDesc
{
	D3D12_ROOT_SIGNATURE_DESC desc;
	std::vector<D3D12_DESCRIPTOR_RANGE>range;
	std::vector<D3D12_ROOT_PARAMETER>param;

	RootDesc(const size_t& rangeNum, const size_t& paramNum) {
		desc = {};
		range.resize(rangeNum);
		param.resize(paramNum);
	}
};

// 
struct Hit
{
	std::wstring name;
	D3D12_HIT_GROUP_DESC desc;
	D3D12_STATE_SUBOBJECT sub;

	Hit(LPCWSTR anyHit, LPCWSTR ClosestHit, const std::wstring& name) :
		name(name) {
		desc = {};
		desc.AnyHitShaderImport     = anyHit;
		desc.ClosestHitShaderImport = ClosestHit;
		desc.HitGroupExport         = this->name.c_str();

		sub.pDesc = &desc;
		sub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP;
	}
};

// 
struct Association
{
	D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION asso;
	D3D12_STATE_SUBOBJECT sub;

	Association(const wchar_t* name[], const size_t& num, D3D12_STATE_SUBOBJECT* sub) {
		asso.NumExports            = num;
		asso.pExports              = name;
		asso.pSubobjectToAssociate = sub;

		this->sub.pDesc = &asso;
		this->sub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
	}
};

// ルートシグネチャ
struct Root
{
	ID3D12RootSignature* root;
	D3D12_STATE_SUBOBJECT sub;

	Root() {
		root = nullptr;
		sub = {};
	}
	~Root() {
		Release(root);
	}
};

// 
struct ShaderConfig
{
	D3D12_RAYTRACING_SHADER_CONFIG config;
	D3D12_STATE_SUBOBJECT sub;

	ShaderConfig(const size_t& attributeSize, const size_t& payloadSize) {
		config.MaxAttributeSizeInBytes = attributeSize;
		config.MaxPayloadSizeInBytes   = payloadSize;

		sub.pDesc = &config;
		sub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG;
	}
};

// 
struct PipeConfig
{
	D3D12_RAYTRACING_PIPELINE_CONFIG config;
	D3D12_STATE_SUBOBJECT sub;

	PipeConfig(const size_t& depth) {
		config.MaxTraceRecursionDepth = depth;

		sub.pDesc = &config;
		sub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG;
	}
};

// 3次元
struct Vector3
{
	float x;
	float y;
	float z;

	Vector3() {
		x = y = z = 0.0f;
	}
	Vector3(const float& x, const float& y, const float& z) : 
		x(x), y(y), z(z){}
	Vector3(const Vector3& vec) {
		(*this) = vec;
	}
	void operator=(const Vector3& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}
};

// 4次元
struct Vector4
{
	float x;
	float y;
	float z;
	float w;

	Vector4() {
		x = y = z = w = 0.0f;
	}
	Vector4(const float& x, const float& y, const float& z, const float& w) :
		x(x), y(y), z(z), w(w) {}
	Vector4(const Vector4& vec) {
		(*this) = vec;
	}
	void operator=(const Vector4& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
	}
};

static const WCHAR* kRayGenShader = L"rayGen";
static const WCHAR* kMissShader = L"miss";
static const WCHAR* kTriangleChs = L"triangleChs";
static const WCHAR* kPlaneChs = L"planeChs";
static const WCHAR* kTriangleHitGroup = L"TriangleHitGroup";
static const WCHAR* kPlaneHitGroup = L"PlaneHitGroup";
