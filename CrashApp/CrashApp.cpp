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
		{
			PEXCEPTION_RECORD exception_record = ep->ExceptionRecord;
			for (int i = 0; i < sizeof(info.exception_record) / sizeof(info.exception_record[0]); i++)
			{
				if (exception_record == nullptr)
				{
					break;
				}
				info.exception_record[i] = *exception_record;
				info.exception_record_count = (i + 1);
				exception_record = exception_record->ExceptionRecord;
			}
		}
		wcscpy_s(info.dump_path, L".\\crash_dump_with_exception_info.dmp");
		COPYDATASTRUCT copy_data;
		copy_data.cbData = sizeof(ExceptionInfo);
		copy_data.dwData = kMiniDumpWriteDump;
		copy_data.lpData = &info;
		SendMessage(hWatchdog, WM_COPYDATA, (WPARAM)(HWND)nullptr, (LPARAM)(LPVOID)&copy_data);
	}
	return EXCEPTION_EXECUTE_HANDLER;
}

/**
	Simply output a dump file
*/
void write_dump()
{
	const HWND hWatchdog = FindWindow(WATCHDOG_WINDOW_CLASS_NAME, WATCHDOG_WINDOW_NAME);
	if (hWatchdog)
	{
		ExceptionInfo info;
		info.process_id = GetCurrentProcessId();
		info.thread_id = GetCurrentThreadId();
		info.exception_record_count = 0;
		wcscpy_s(info.dump_path, L".\\crash_dump.dmp");
		COPYDATASTRUCT copy_data;
		copy_data.cbData = sizeof(ExceptionInfo);
		copy_data.dwData = kMiniDumpWriteDump;
		copy_data.lpData = &info;
		SendMessage(hWatchdog, WM_COPYDATA, (WPARAM)(HWND)nullptr, (LPARAM)(LPVOID)&copy_data);
	}
}

int main()
{
	printf("ready...\r\n");
	getchar();

	//To simply output a dump file
	write_dump();

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
