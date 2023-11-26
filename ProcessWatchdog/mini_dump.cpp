#include "mini_dump.h"

#include "def/watchdog.h"
#include "dbg_help_proxy.h"
#include "scoped_handle.h"

namespace watchdog {
namespace service {
namespace module {

bool MiniDump::MiniDumpWriteDump(const PCOPYDATASTRUCT data)
{
	const DWORD thread_id = static_cast<ExceptionInfo*>(data->lpData)->thread_id;
	ScopedHandle<HANDLE, NULL, CloseHandle> hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, thread_id);
	if (hThread == NULL)
	{
		return false;
	}
	CONTEXT context = { 0 };
	context.ContextFlags = CONTEXT_FULL;
	if (!GetThreadContext(hThread, &context))
	{
		return false;
	}
	EXCEPTION_POINTERS exception_pointers = { nullptr, nullptr };
	exception_pointers.ContextRecord = &context;
	PEXCEPTION_RECORD &exception_record = exception_pointers.ExceptionRecord;
	{
		ExceptionInfo info = *static_cast<ExceptionInfo*>(data->lpData);
		for (int i = 0; i < info.exception_record_count; i++)
		{
			exception_record = &info.exception_record[i];
			exception_record = exception_record->ExceptionRecord;
			exception_record = nullptr;
		}
	}
	MINIDUMP_EXCEPTION_INFORMATION exception;
	exception.ThreadId = thread_id;
	exception.ExceptionPointers = &exception_pointers;
	exception.ClientPointers = FALSE;
	const DWORD process_id = static_cast<ExceptionInfo*>(data->lpData)->process_id;
	const ScopedHandle<HANDLE, NULL, CloseHandle> hProcess = OpenProcess(
		PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_DUP_HANDLE, FALSE, process_id
	);
	if (!hProcess)
	{
		return false;
	}

	// write dump
	ScopedHandle<HANDLE, INVALID_HANDLE_VALUE, CloseHandle> hFile = CreateFile(
		static_cast<ExceptionInfo*>(data->lpData)->dump_path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
	);
	const bool result = watchdog::service::dll_proxy::DbgHelpProxy::GetInstance().MiniDumpWriteDump(
		hProcess, process_id, hFile, MiniDumpWithFullMemory, &exception, NULL, NULL
	);

	return result;
}

} // namespace module
} // namespace service
} // namespace watchdog