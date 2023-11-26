#include <Windows.h>

static const wchar_t *WATCHDOG_WINDOW_CLASS_NAME = L"{6ACE8D87-17E8-4F97-BE06-B412A08E4E22}";
static const wchar_t *WATCHDOG_WINDOW_NAME = L"Watchdog";

enum WatchdogAction
{
	kMiniDumpWriteDump,
};

struct ExceptionInfo
{
	DWORD process_id;
	DWORD thread_id;
	int exception_record_count;
	EXCEPTION_RECORD exception_record[16];
	wchar_t dump_path[MAX_PATH];
};
