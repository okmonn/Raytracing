#pragma once
#include "Function/Function.h"
#include <vector>

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
