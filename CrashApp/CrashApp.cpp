#include <iostream>

#include <Windows.h>
#include <dbghelp.h>

#include "../ProcessWatchdog/def/watchdog.h"

/**
	Output dump file with exception information
*/
int filter(unsigned int code, EXCEPTION_POINTERS *ep)
{
	const HWND hWatchdog = FindWindow(WATCHDOG_WINDOW_CLASS_NAME, WATCHDOG_WINDOW_NAME);
	if (hWatchdog)
	{
		ExceptionInfo info;
		info.process_id = GetCurrentProcessId();
		info.thread_id = GetCurrentThreadId();
		info.ex_info = ep;
		wcscpy_s(info.dump_path, L".\\crash_dump_with_exception_info.dmp");

		COPYDATASTRUCT copy_data;
		copy_data.cbData = sizeof(ExceptionInfo);
		copy_data.dwData = kMiniDumpWriteDump;
		copy_data.lpData = &info;
		SendMessage(hWatchdog, WM_COPYDATA, (WPARAM)(HWND)nullptr, (LPARAM)(LPVOID)&copy_data);
	}
	return EXCEPTION_EXECUTE_HANDLER;
}

int main()
{
	printf("ready...\r\n");
	getchar();

	//To output a dump file containing exception information
	__try
	{
		int a = 1;
		int b = 0;
		int c = a / b;
		printf("%d\n", c);
	}
	__except (filter(GetExceptionCode(), GetExceptionInformation()))
	{
		printf("in except.\n");
	}
	printf("done.\n");
	getchar();

	return 0;
}
