#include "main.h"

#include <Windows.h>

#include "application.h"

/**
	エントリポイント
*/
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	watchdog::core::Application& app = watchdog::core::Application::GetInstance();
	return app.Execute(hInstance, nShowCmd);
}

