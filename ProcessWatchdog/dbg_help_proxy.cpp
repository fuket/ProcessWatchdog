#include "dbg_help_proxy.h"

namespace watchdog {
namespace service {
namespace dll_proxy {

DbgHelpProxy::DbgHelpProxy()
	: module_(LoadLibrary(L"DbgHelp.dll"))
	, pf_mini_dump_write_dump_(reinterpret_cast<pMiniDumpWriteDump>(GetProcAddress(module_, "MiniDumpWriteDump")))
{}

DbgHelpProxy::~DbgHelpProxy()
{
	if (module_ != NULL)
	{
		FreeLibrary(module_);
	}
}

DbgHelpProxy& DbgHelpProxy::GetInstance()
{
	static DbgHelpProxy proxy;
	return proxy;
}

bool DbgHelpProxy::MiniDumpWriteDump(
	HANDLE hProcess
	, DWORD ProcessId
	, HANDLE hFile
	, MINIDUMP_TYPE DumpType
	, PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam
	, PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam
	, PMINIDUMP_CALLBACK_INFORMATION CallbackParam
)
{
	if (pf_mini_dump_write_dump_ != NULL)
	{
		return (pf_mini_dump_write_dump_(hProcess, ProcessId, hFile, DumpType, ExceptionParam, UserStreamParam, CallbackParam) == TRUE);
	}
	return false;
}

} // namespace dll_proxy
} // namespace service
} // namespace watchdog