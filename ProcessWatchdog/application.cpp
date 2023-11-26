#include "application.h"

#include <windows.h>
#include <windowsx.h>

#include "def/watchdog.h"
#include "mini_dump.h"

namespace {

/**
	ウィンドウを閉じる
*/
void OnClose(HWND hwnd)
{
	DestroyWindow(hwnd);
}

/**
	ウィンドウを破棄する
*/
void OnDestroy(HWND hwnd)
{
	PostQuitMessage(0);
}

/**
	データコピーが呼ばれたとき
*/
void OnCopyData(HWND hwnd, HWND wParam, PCOPYDATASTRUCT lParam)
{
	const PCOPYDATASTRUCT data = lParam;
	switch (data->dwData)
	{
	case kMiniDumpWriteDump:
	default:
		watchdog::service::module::MiniDump::MiniDumpWriteDump(data);
		break;
	}
}

/**
	ウィンドウプロシージャ
*/
LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, OnClose);
		HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hwnd, WM_COPYDATA, OnCopyData);
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
}

namespace watchdog {
namespace core {

Application::Application()
{}

Application::~Application()
{}

Application& Application::GetInstance()
{
	static Application application;
	return application;
}

int Application::Execute(const HINSTANCE hInstance, const int nShowCmd)
{
	WNDCLASSEX wcex = {};
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainWindowProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = WATCHDOG_WINDOW_CLASS_NAME;
	wcex.lpszMenuName = nullptr;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	RegisterClassEx(&wcex);

	const HWND hwnd = CreateWindowEx(WS_EX_CONTROLPARENT, wcex.lpszClassName, WATCHDOG_WINDOW_NAME
		, (WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX) | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 240, 150, nullptr, nullptr, hInstance, nullptr);

	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		if (!IsDialogMessage(hwnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return static_cast<int>(msg.wParam);
}

} // namespace core
} // namespace watchdog