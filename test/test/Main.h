#pragma once
#include <wrl.h>
#include <array>
#include <vector>
#include <d3d12.h>
#include <dxgi1_6.h>

// 機能レベル
const D3D_FEATURE_LEVEL levels[] =
{
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0,
};


namespace okmonn
{
	template <typename T>
	class  Interface
	{
	public:
		// コンストラクタ
		Interface() : ptr(nullptr) {}
		// デストラクタ
		virtual ~Interface()
		{
			if (ptr != nullptr)
			{
				ptr->Release();
				ptr = nullptr;
			}
		}

		T* get(void) const
		{
			return ptr;
		}

		void operator=(const T* ptr)
		{
			this->ptr = ptr;
		}

		T** operator&(void)
		{
			return &ptr;
		}

		T* operator->(void) const
		{
			return ptr;
		}

	private:
		// インターフェース
		T* ptr;
	};

	// フェンス
	class Fence
	{
	public:
		// コンストラクタ
		Fence() : cnt(0) {}
		// デストラクタ
		~Fence() {}

		// 待機
		void Wait(void)
		{
			if (fence->GetCompletedValue() != cnt)
			{
				void* handle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
				auto hr = fence->SetEventOnCompletion(cnt, handle);
				_ASSERT(hr == S_OK);

				WaitForSingleObject(handle, INFINITE);
			}
		}

		ID3D12Fence1* get(void) const
		{
			return fence.get();
		}

		ID3D12Fence1** operator&(void)
		{
			return &fence;
		}

		unsigned __int64 operator++(void)
		{
			cnt += 1;
			return cnt;
		}

	private:
		// フェンスインターフェース
		Interface<ID3D12Fence1>fence;

		// フェンスカウント
		unsigned __int64 cnt;
	};

	// 加速構造
	class Acceleration
	{
	public:
		// コンストラクタ
		Acceleration() : geo({}), input({}) {}
		// デストラクタ
		~Acceleration() {}

		// スクラッチリソースの取得
		okmonn::Interface<ID3D12Resource1> Scratch(void) const
		{
			return rsc[0];
		}

		//リザルトリソースの取得
		okmonn::Interface<ID3D12Resource1> Scratch(void) const
		{
			return rsc[1];
		}

		//インスタンスリソースの取得
		okmonn::Interface<ID3D12Resource1> Scratch(void) const
		{
			return rsc[2];
		}

	private:
		// ジオメトリ情報
		D3D12_RAYTRACING_GEOMETRY_DESC geo;

		// 入力情報
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input;

		// リソース
		std::array<okmonn::Interface<ID3D12Resource1>, 3>rsc;
	};

	// 3次元ベクトル
	class Vec3
	{
	public:
		// コンストラクタ
		Vec3()
		{
			x = y = z = 0.0f;
		}
		Vec3(const float& x, const float& y, const float& z) : 
			x(x), y(y), z(z) {
		}
		Vec3(const Vec3& vec)
		{
			(*this) = vec;
		}
		// デストラクタ
		~Vec3() {}

		void operator=(const Vec3& vec)
		{
			x = vec.x;
			y = vec.y;
			z = vec.z;
		}

	private:
		float x;
		float y;
		float z;
	};
}

// ウィンドウハンドル
void* winHandle = nullptr;

// ファクトリー
okmonn::Interface<IDXGIFactory7>factory;

// デバイス
okmonn::Interface<ID3D12Device5>device;

// コマンドアロケータ
okmonn::Interface<ID3D12CommandAllocator>allo;

// コマンドリスト
okmonn::Interface<ID3D12GraphicsCommandList5>list;

// コマンドキュー
okmonn::Interface<ID3D12CommandQueue>queue;

// フェンス
okmonn::Fence fence;

// スワップチェイン
okmonn::Interface<IDXGISwapChain4>swap;

// レンダーターゲット用ヒープ
okmonn::Interface<ID3D12DescriptorHeap>rtvHeap;

// レンダーターゲット用リソース
std::vector < okmonn::Interface<ID3D12Resource1>>rtvRsc;

// プリミティブ
okmonn::Interface<ID3D12Resource1>triangle;

// ボトムレベル
okmonn::Acceleration bottom;