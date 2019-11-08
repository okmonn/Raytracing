#pragma once
#include "ShaderLibrary.h"
#include <wrl.h>
#include <array>
#include <dxgi1_6.h>

// メモリ開放
#define Release(X) { if((X) != nullptr) (X)->Release(); (X) = nullptr; }

// ウィンドウサイズX
#define WINSIZE_X 640
// ウィンドウサイズY
#define WINSIZE_Y 480

// バックバッファーカウント
#define BACK_BUFFER 3

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

// ルートシグネチャ
struct RootSignature
{
	ID3D12RootSignature* root;
	std::vector<D3D12_DESCRIPTOR_RANGE>range;
	std::vector<D3D12_ROOT_PARAMETER>param;

	RootSignature() {
		root = nullptr;
		range = {};
		param = {};
	}
};

// 当たり情報
struct Hit
{
	std::wstring name;
	D3D12_HIT_GROUP_DESC desc;
	D3D12_STATE_SUBOBJECT sub;

	Hit(LPCWSTR anyHit, LPCWSTR closestHit, const std::wstring& name)
		: name(name)
	{
		desc.AnyHitShaderImport       = anyHit;
		desc.ClosestHitShaderImport   = closestHit;
		desc.HitGroupExport           = name.c_str();
		desc.IntersectionShaderImport = nullptr;
		desc.Type                     = D3D12_HIT_GROUP_TYPE::D3D12_HIT_GROUP_TYPE_TRIANGLES;

		sub.pDesc = &desc;
		sub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP;
	}
};

// 
struct Association
{
	D3D12_STATE_SUBOBJECT sub;
	D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION association;

	Association(LPCWSTR name, const unsigned int& cnt, const D3D12_STATE_SUBOBJECT* subObject) {
		association.NumExports            = cnt;
		association.pExports              = &name;
		association.pSubobjectToAssociate = subObject;

		sub.pDesc = &association;
		sub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
	}
};

void CreateRoot(ID3D12Device5* device, RootSignature& rootsignature, const D3D12_ROOT_SIGNATURE_DESC& desc);

// ローカルルートシグネチャ
struct LocalRoot
{
	RootSignature rootsignature;
	D3D12_STATE_SUBOBJECT sub;

	LocalRoot() {
		sub.pDesc = &rootsignature.root;
		sub.Type = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
	}
	LocalRoot(ID3D12Device5* device, const D3D12_ROOT_SIGNATURE_DESC& desc) {
		CreateRoot(device, rootsignature, desc);

		sub.pDesc = &rootsignature.root;
		sub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
	}
	~LocalRoot() {
		Release(rootsignature.root);
	}
};

// グローバルルートシグネチャ
struct GlobalRoot
{
	RootSignature rootsignature;
	D3D12_STATE_SUBOBJECT sub;

	GlobalRoot() {
		sub.Type = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
	}
	GlobalRoot(ID3D12Device5* device, const D3D12_ROOT_SIGNATURE_DESC& desc) {
		CreateRoot(device, rootsignature, desc);

		sub.pDesc = &rootsignature.root;
		sub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
	}
	~GlobalRoot() {
		Release(rootsignature.root);
	}
};

// シェーダ構成
struct ShaderConfig
{
	D3D12_RAYTRACING_SHADER_CONFIG config;
	D3D12_STATE_SUBOBJECT sub;

	ShaderConfig(const unsigned int& attributeSize, const unsigned int& payloadSize) {
		config.MaxAttributeSizeInBytes = attributeSize;
		config.MaxPayloadSizeInBytes   = payloadSize;

		sub.pDesc = &config;
		sub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG;
	}
};

// パイプライン構成
struct PipeConfig
{
	D3D12_RAYTRACING_PIPELINE_CONFIG config;
	D3D12_STATE_SUBOBJECT sub;

	PipeConfig(const unsigned int& traceDepth) {
		config.MaxTraceRecursionDepth = traceDepth;

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

// クリアカラー
float color[] = {
	1.0f, 1.0f, 1.0f, 1.0f
};
