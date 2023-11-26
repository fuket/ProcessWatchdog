#ifndef WATCHDOG_SERVICE_MODULE_SCOPED_HANDLE_H_
#define WATCHDOG_SERVICE_MODULE_SCOPED_HANDLE_H_

#include <Windows.h>

namespace watchdog {
namespace service {
namespace module {

template<typename T, T INVALID_VALUE, BOOL(WINAPI *CLOSE_METHOD)(T)>
class ScopedHandle
{
public:
	ScopedHandle()
		: handle_(INVALID_VALUE)
	{}

	ScopedHandle(const T& handle)
		: handle_(handle)
	{}

	ScopedHandle& operator=(const T& handle)
	{
		if (handle_ != handle)
		{
			if (!IsInvalid())
			{
				(*CLOSE_METHOD)(handle_);
			}
			handle_ = handle;
		}
		return *this;
	}

	virtual ~ScopedHandle()
	{
		if (!IsInvalid())
		{
			(*CLOSE_METHOD)(handle_);
			handle_ = INVALID_VALUE;
		}
	}

	T Release()
	{
		T handle = handle_;
		handle_ = INVALID_VALUE;
		return handle;
	}

	bool IsInvalid() const
	{
		return (handle_ == INVALID_VALUE);
	}

	operator const T() const
	{
		return handle_;
	}

private:
	T handle_;

	ScopedHandle(const ScopedHandle&) = delete;
	ScopedHandle& operator = (const ScopedHandle&) = delete;
};

} // namespace module
} // namespace service
} // namespace watchdog

#endif //WATCHDOG_SERVICE_MODULE_SCOPED_HANDLE_H_