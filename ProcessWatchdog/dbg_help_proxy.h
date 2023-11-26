#ifndef WATCHDOG_SERVICE_DLL_PROXY_DBG_HELP_PROXY_H_
#define WATCHDOG_SERVICE_DLL_PROXY_DBG_HELP_PROXY_H_

#include <Windows.h>
#include <dbghelp.h>

namespace watchdog {
namespace service {
namespace dll_proxy {

class DbgHelpProxy final
{
public:
	virtual ~DbgHelpProxy();

	static DbgHelpProxy& GetInstance();

	bool MiniDumpWriteDump(
		HANDLE hProcess
		, DWORD ProcessId
		, HANDLE hFile
		, MINIDUMP_TYPE DumpType
		, PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam
		, PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam
		, PMINIDUMP_CALLBACK_INFORMATION CallbackParam
	);

private:
	DbgHelpProxy();

	DbgHelpProxy(const DbgHelpProxy&) = delete;
	DbgHelpProxy& operator=(const DbgHelpProxy&) = delete;
	DbgHelpProxy(DbgHelpProxy&&) = delete;
	DbgHelpProxy& operator=(DbgHelpProxy&&) = delete;

	HMODULE module_;

	typedef BOOL(WINAPI* pMiniDumpWriteDump)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, PMINIDUMP_EXCEPTION_INFORMATION, PMINIDUMP_USER_STREAM_INFORMATION, PMINIDUMP_CALLBACK_INFORMATION);
	pMiniDumpWriteDump pf_mini_dump_write_dump_;
};

} // namespace dll_proxy
} // namespace service
} // namespace watchdog

#endif //WATCHDOG_SERVICE_DLL_PROXY_DBG_HELP_PROXY_H_