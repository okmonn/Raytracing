#include "Window.h"
#include <crtdbg.h>
#include <Windows.h>

// ウィンドウコールバック
#ifdef _WIN64
__int64 __stdcall WindowProc(void* hWnd, unsigned int message, unsigned __int64 wParam, __int64 lParam)
#else
long __stdcall WindowProc(void* hWnd, unsigned int message, unsigned int wParam, long lParam)
#endif
{
	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(HWND(hWnd));
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return DefWindowProc(HWND(hWnd), message, wParam, lParam);
}

// コンストラクタ
Window::Window(const Vec2& winSize) : 
	handle(nullptr)
{
	CreateWin(winSize);
	ShowWindow(HWND(handle), SW_SHOW);
}

// デストラク
Window::~Window()
{
}

// ウィンドウ生成
void Window::CreateWin(const Vec2& winSize)
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
	rect.bottom = winSize.y;
	rect.right  = winSize.x;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	handle = CreateWindowEx(WS_EX_ACCEPTFILES, wnd.lpszClassName, L"DXR", WS_OVERLAPPEDWINDOW, 0x80000000, 0x80000000,
		(rect.right - rect.left), (rect.bottom - rect.top), nullptr, nullptr, wnd.hInstance, nullptr);
	_ASSERT(handle != nullptr);
}

// ウィンドウハンドルの取得
HWND__* Window::Get(void) const
{
	return HWND(handle);
}

// ウィンドウサイズの取得
Vec2 Window::WinSize(void) const
{
	RECT rect{};
	GetClientRect(HWND(handle), &rect);

	return Vec2(int(rect.right), int(rect.bottom));
}

// メッセージの確認
bool Window::CheckMsg(void)
{
	static MSG msg{};
	static const wchar_t* name = nullptr;
	static void* instance      = nullptr;

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		switch (msg.message)
		{
		case WM_CREATE:
		{
			auto wnd = (LPWNDCLASSEX)GetWindowLongPtr(msg.hwnd, GWLP_HINSTANCE);
			name     = wnd->lpszClassName;
			instance = wnd->hInstance;
			break;
		}
		case WM_QUIT:
			UnregisterClass(name, HINSTANCE(instance));
			return false;
		default:
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

