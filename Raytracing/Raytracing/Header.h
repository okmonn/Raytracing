#pragma once
#include <wrl.h>
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
