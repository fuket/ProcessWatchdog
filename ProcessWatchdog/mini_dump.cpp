#include "mini_dump.h"

#include "def/watchdog.h"
#include "dbg_help_proxy.h"
#include "scoped_handle.h"

namespace watchdog {
namespace service {
namespace module {

bool MiniDump::MiniDumpWriteDump(const PCOPYDATASTRUCT data)
{
	MINIDUMP_EXCEPTION_INFORMATION exception;
	exception.ThreadId = static_cast<ExceptionInfo*>(data->lpData)->thread_id;
	exception.ExceptionPointers = static_cast<ExceptionInfo*>(data->lpData)->ex_info;
	exception.ClientPointers = TRUE; //If in-process dump generation sets FALSE
	const DWORD process_id = static_cast<ExceptionInfo*>(data->lpData)->process_id;
	const ScopedHandle<HANDLE, NULL, CloseHandle> process_handle = OpenProcess(
		PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_DUP_HANDLE, FALSE, process_id
	);
	if (!process_handle)
	{
		return false;
	}

	// write dump
	ScopedHandle<HANDLE, INVALID_HANDLE_VALUE, CloseHandle> dump_file_handle = CreateFile(
		static_cast<ExceptionInfo*>(data->lpData)->dump_path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
	);
	const bool result = watchdog::service::dll_proxy::DbgHelpProxy::GetInstance().MiniDumpWriteDump(
		process_handle, process_id, dump_file_handle, static_cast<MINIDUMP_TYPE>(MiniDumpWithFullMemory | MiniDumpWithHandleData), &exception, NULL, NULL
	);

	return result;
}

} // namespace module
} // namespace service
} // namespace watchdog