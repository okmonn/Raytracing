#include "Header.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

// ウィンドウコールバック
#ifdef _WIN64
__int64 __stdcall WindowProc(void* hWnd, unsigned int message, unsigned __int64 wParam, __int64 lParam)
#else
long __stdcall WindowProc(void* hWnd, unsigned int message, unsigned int wParam, long lParam)
#endif
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		DestroyWindow(HWND(hWnd));
		auto wnd = (LPWNDCLASSEX)GetWindowLongPtr(HWND(hWnd), GWLP_HINSTANCE);
		UnregisterClass(wnd->lpszClassName, wnd->hInstance);
		return 0;
	}
	default:
		break;
	}

	return DefWindowProc(HWND(hWnd), message, wParam, lParam);
}

// メッセージの確認
bool CheckMsg(void)
{
	static MSG msg{};
	if (msg.message == WM_QUIT)
	{
		return false;
	}

	//呼び出し側スレッドが所有しているウィンドウに送信されたメッセージの保留されている物を取得
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		//仮想キーメッセージを文字メッセージに変換
		TranslateMessage(&msg);
		//1つのウィドウプロシージャにメッセージを送出する
		DispatchMessage(&msg);
	}

	return true;
}

// コマンドキュー生成
void CreateCommandQueue(ID3D12Device5* device, ID3D12CommandQueue** queue, const D3D12_COMMAND_LIST_TYPE& type)
{
	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Flags    = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type     = type;

	auto hr = device->CreateCommandQueue(&desc, IID_PPV_ARGS(&(*queue)));
	_ASSERT(hr == S_OK);
}

// スワップチェイン生成
void CreateSwapChain(IDXGIFactory7* factory, ID3D12CommandQueue* queue, void* winHandle, IDXGISwapChain4** swap, const unsigned int& bufferCnt)
{
	RECT rect{};
	GetClientRect(HWND(winHandle), &rect);

	DXGI_SWAP_CHAIN_DESC1 desc{};
	desc.AlphaMode   = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;
	desc.BufferCount = bufferCnt;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Flags       = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.Format      = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Height      = rect.bottom;
	desc.SampleDesc  = { 1, 0 };
	desc.Scaling     = DXGI_SCALING::DXGI_SCALING_STRETCH;
	desc.Stereo      = false;
	desc.SwapEffect  = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Width       = rect.right;

	auto hr = factory->CreateSwapChainForHwnd(queue, HWND(winHandle), &desc, nullptr, nullptr, (IDXGISwapChain1**)&(*swap));
	_ASSERT(hr == S_OK);
}

// エントリーポイント
#ifdef _DEBUG
int main()
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	//ウィンドウ生成
	void* winHandle = nullptr;
	{
		WNDCLASSEX wnd{};
		wnd.cbSize        = sizeof(WNDCLASSEX);
		wnd.hbrBackground = CreateSolidBrush(COLOR_APPWORKSPACE);
		wnd.hCursor       = LoadCursor(nullptr, IDC_ARROW);
		wnd.hIcon         = nullptr;
		wnd.hIconSm       = nullptr;
		wnd.hInstance     = GetModuleHandle(0);
		wnd.lpfnWndProc   = WNDPROC(WindowProc);
		wnd.lpszClassName = L"おかもん";
		wnd.lpszMenuName  = L"おかもん";
		wnd.style         = CS_HREDRAW | CS_VREDRAW;
		RegisterClassEx(&wnd);

		RECT rect{};
		rect.bottom = WINSIZE_Y;
		rect.right  = WINSIZE_X;
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

		winHandle = CreateWindowEx(WS_EX_ACCEPTFILES, wnd.lpszClassName, L"Raytracing", WS_OVERLAPPEDWINDOW, 0x80000000, 0x80000000,
			(rect.right - rect.left), (rect.bottom - rect.top), nullptr, nullptr, wnd.hInstance, nullptr);
		_ASSERT(winHandle != nullptr);

		ShowWindow(HWND(winHandle), SW_SHOW);
	}

#ifdef _DEBUG
	ID3D12Debug* debug = nullptr;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
	Release(debug);
#endif

	//ファクトリー生成
	IDXGIFactory7* factory = nullptr;
	{
		auto hr = CreateDXGIFactory(IID_PPV_ARGS(&factory));
		_ASSERT(hr == S_OK);
	}

	//デバイス生成
	ID3D12Device5* device  = nullptr;
	{
		Microsoft::WRL::ComPtr<IDXGIAdapter1>adaptor = nullptr;
		unsigned int index    = 0;
		unsigned int revision = 0;
		for (unsigned int i = 0; factory->EnumAdapters1(i, &adaptor) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			for (const D3D_FEATURE_LEVEL& level : levels)
			{
				if (SUCCEEDED(D3D12CreateDevice(adaptor.Get(), level, IID_PPV_ARGS(&device))))
				{
					goto Check;
				}
			}

		Check:
			//レイトレーシングのサポートチェック
			D3D12_FEATURE_DATA_D3D12_OPTIONS5 option{};
			auto hr = device->CheckFeatureSupport(D3D12_FEATURE::D3D12_FEATURE_D3D12_OPTIONS5, &option, sizeof(option));
			_ASSERT(hr == S_OK);

			if (option.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED)
			{
				break;
			}
		}
	}

	//コマンドキュー生成
	ID3D12CommandQueue* queue = nullptr;
	/*{
		CreateCommandQueue(device, &queue, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
	}*/

	// スワップチェイン生成
	IDXGISwapChain4* swap = nullptr;
	/*{
		CreateSwapChain(factory, queue, winHandle, &swap, 3);
	}*/

	while (CheckMsg() == true && !(GetKeyState(VK_ESCAPE) & 0x80))
	{

	}

	//終了
	{
		Release(swap);
		Release(queue);
		Release(device);
		Release(factory);
	}

	return 0;
}
