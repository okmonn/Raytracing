#pragma once
#include "Function/Function.h"

template <typename T>
class Obj {
public:
	/*コンストラクタ*/
	Obj() : obj(nullptr) {}
	/*デストラクタ*/
	~Obj(){
		if (obj != nullptr) {
			obj->Release();
			obj = nullptr;
		}
	}
	T* Get(void) const {
		return obj;
	}
	T* operator->(void) const {
		return Get();
	}
	T** operator&(void) {
		return &obj;
	}

private:
	T* obj;
};

/*ウィンドウハンドル*/
void* win = nullptr;
/*デバイス*/
Obj<ID3D12Device5>dev;
/*フォールバック用デバイス*/
Obj<ID3D12RaytracingFallbackDevice>f_dev;
/*コマンドアロケータ*/
Obj<ID3D12CommandAllocator>allo;
/*コマンドキュー*/
Obj<ID3D12CommandQueue>queue;
/*コマンドリスト*/
Obj<ID3D12GraphicsCommandList4>list;
/*フォールバック用コマンドリスト*/
Obj<ID3D12RaytracingFallbackCommandList>f_list;
/*フェンス*/
Obj<ID3D12Fence1>fence;
/*フェンスカウント*/
std::uint64_t fenceCnt = 0;
/*スワップチェイン*/
Obj<IDXGISwapChain4>swap;
/*レンダーターゲット用リソース配列*/
std::vector<Obj<ID3D12Resource1>>rtvRsc;
/*レンダーターゲット用ヒープ*/
Obj<ID3D12DescriptorHeap>rtvHeap;
/*三角形プリミティブ用リソース*/
Obj<ID3D12Resource1>triangle;
/*加速構造(ボトムレベル)用ヒープ*/
Obj<ID3D12DescriptorHeap>bottomHeap;
/*加速構造(ボトムレベル)用スクラッチリソース*/
Obj<ID3D12Resource1>bottomScratch;
/*加速構造(ボトムレベル)用リザルトリソース*/
Obj<ID3D12Resource1>bottomResult;
/*加速構造(トップレベル)用ヒープ*/
Obj<ID3D12DescriptorHeap>topHeap;
/*加速構造(トップレベル)用スクラッチリソース*/
Obj<ID3D12Resource1>topScratch;
/*加速構造(トップレベル)用リザルトリソース*/
Obj<ID3D12Resource1>topResult;
/*加速構造(トップレベル)用インスタンスリソース*/
Obj<ID3D12Resource1>topInstance;
/*サブオブジェクト*/
std::vector<D3D12_STATE_SUBOBJECT>subObj;
/*グローバルルートシグネチャ*/
Obj<ID3D12RootSignature>global;
/*シェーダ情報(1シェーダに1つ)*/
Obj<IDxcBlob>shader;
/*DXILライブラリ情報(1シェーダに1つ)*/
D3D12_DXIL_LIBRARY_DESC dxil{};
/*読み込み関数情報(1シェーダに1つ)*/
std::vector<D3D12_EXPORT_DESC>exportDesc;
/*読み込み関数名(1シェーダに1つ)*/
const wchar_t* exportName[3];
/*ヒットグループ情報(1ヒットグループに1つ)*/
D3D12_HIT_GROUP_DESC hitDesc{};
/*ヒットグループ用クローゼストヒット関数名*/
std::wstring closestHitName;
/*ヒットグループ名*/
std::wstring hitName;
/*ローカルルートシグネチャ(1読み込みに1つ)*/
std::vector<Obj<ID3D12RootSignature>>local;
/*読み込みとの関連付け情報(1読み込みに1つ)*/
std::vector<D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION>association;
/*シェーダ設定(1シェーダに1つ)*/
D3D12_RAYTRACING_SHADER_CONFIG shaderConfig{};
/*パイプライン設定*/
D3D12_RAYTRACING_PIPELINE_CONFIG pipeConfig{};
/*状態オブジェクト*/
Obj<ID3D12RaytracingFallbackStateObject>pipe;
/*シェーダテーブル用リソース(1読み込みに1つ)*/
std::vector<Obj<ID3D12Resource1>>shaderTbl;
