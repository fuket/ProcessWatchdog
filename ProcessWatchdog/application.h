#ifndef WATCHDOG_CORE_APPLICATION_H_
#define WATCHDOG_CORE_APPLICATION_H_

#include <Windows.h>

namespace watchdog {
namespace core {

class Application final
{
public:
	virtual ~Application();

	static Application& GetInstance();
	int Execute(const HINSTANCE hInstance, const int nShowCmd);

private:
	Application();

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(Application&&) = delete;
};

} // namespace core
} // namespace watchdog

#endif //WATCHDOG_CORE_APPLICATION_H_