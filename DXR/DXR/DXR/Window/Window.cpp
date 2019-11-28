#include "Window.h"
#include <crtdbg.h>
#include <Windows.h>

// �E�B���h�E�R�[���o�b�N
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

// �R���X�g���N�^
Window::Window(const Vec2& winSize) : 
	handle(nullptr)
{
	CreateWin(winSize);
	ShowWindow(HWND(handle), SW_SHOW);
}

// �f�X�g���N
Window::~Window()
{
}

// �E�B���h�E����
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
	wnd.lpszClassName = L"��������";
	wnd.lpszMenuName  = L"��������";
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

// �E�B���h�E�n���h���̎擾
HWND__* Window::Get(void) const
{
	return HWND(handle);
}

// �E�B���h�E�T�C�Y�̎擾
Vec2 Window::WinSize(void) const
{
	RECT rect{};
	GetClientRect(HWND(handle), &rect);

	return Vec2(int(rect.right), int(rect.bottom));
}

// ���b�Z�[�W�̊m�F
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

