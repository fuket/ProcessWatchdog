#ifndef WATCHDOG_SERVICE_MODULE_MINI_DUMP_H_
#define WATCHDOG_SERVICE_MODULE_MINI_DUMP_H_

#include <Windows.h>

namespace watchdog {
namespace service {
namespace module {

class MiniDump
{
public:
	static bool MiniDumpWriteDump(const PCOPYDATASTRUCT data);
};

} // namespace module
} // namespace service
} // namespace watchdog

#endif //WATCHDOG_SERVICE_MODULE_MINI_DUMP_H_