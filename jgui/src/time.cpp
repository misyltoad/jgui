#include <jgui/time.h>

#ifdef WIN32
#include <Windows.h>
#endif

namespace jgui
{
	const Time Time::Zero{0};
	const Time Time::Never{-1};

	f32 Time::AsSeconds() const
	{
		return m_microseconds / 1000000.0f;
	}

	i64 Time::AsMilliseconds() const
	{
		return (i64)(m_microseconds / 1000);
	}

	i64 Time::AsMicroseconds() const
	{
		return (i64)(m_microseconds);
	}

#ifdef WIN32
	LARGE_INTEGER frequency;
#endif

	void InitTime()
	{
#ifdef WIN32
		QueryPerformanceFrequency(&frequency);
#endif
	}

	Time Time::Now()
	{
#ifdef WIN32
		HANDLE currentThread = GetCurrentThread();
		DWORD_PTR affinityMask = SetThreadAffinityMask(currentThread, 1);

		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);

		SetThreadAffinityMask(currentThread, affinityMask);

		return Time::FromMicroseconds(1000000 * time.QuadPart / frequency.QuadPart);
#else
		auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
		return Time::FromMicroseconds(std::chrono::duration_cast<std::chrono::microseconds>(now).count());
#endif
	}

	void Time::Sleep(Time time)
	{
		if (time.AsMicroseconds() <= 0)
			return;

#ifdef WIN32
		TIMECAPS tc;
		timeGetDevCaps(&tc, sizeof(TIMECAPS));
		timeBeginPeriod(tc.wPeriodMin);

		::Sleep((DWORD) time.AsMilliseconds());

		timeBeginPeriod(tc.wPeriodMin);
#else
		std::this_thread::sleep_for(std::chrono::microseconds(time.AsMicroseconds()));
#endif
	}

	Time::Time(i64 microseconds)
		: m_microseconds{ microseconds }
	{
	}

	Time Time::FromSeconds(f32 amount) { return Time{ (i64)(amount * 1000000) }; }

	Time Time::FromMilliseconds(i64 amount) { return Time{ (i64)(amount * 1000) }; }

	Time Time::FromMicroseconds(i64 amount) { return Time{ amount }; }

	bool operator==(Time left, Time right)
	{
		return left.AsMicroseconds() == right.AsMicroseconds();
	}

	bool operator!=(Time left, Time right)
	{
		return left.AsMicroseconds() != right.AsMicroseconds();
	}

	bool operator<(Time left, Time right)
	{
		return left.AsMicroseconds() < right.AsMicroseconds();
	}

	bool operator>(Time left, Time right)
	{
		return left.AsMicroseconds() > right.AsMicroseconds();
	}

	bool operator<=(Time left, Time right)
	{
		return left.AsMicroseconds() <= right.AsMicroseconds();
	}

	bool operator>=(Time left, Time right)
	{
		return left.AsMicroseconds() >= right.AsMicroseconds();
	}

	Time operator-(Time right) { return Time::FromMicroseconds(-right.AsMicroseconds()); }

	Time operator+(Time left, Time right)
	{
		return Time::FromMicroseconds(left.AsMicroseconds() + right.AsMicroseconds());
	}

	Time operator-(Time left, Time right)
	{
		return Time::FromMicroseconds(left.AsMicroseconds() - right.AsMicroseconds());
	}

	Time& operator+=(Time& left, Time right) { return left = left + right; }

	Time& operator-=(Time& left, Time right) { return left = left - right; }

	Time operator*(Time left, f32 right)
	{
		return Time::FromSeconds(left.AsSeconds() * right);
	}

	Time operator*(Time left, i64 right)
	{
		return Time::FromMicroseconds(left.AsMicroseconds() * right);
	}

	Time operator*(f32 left, Time right)
	{
		return Time::FromSeconds(left * right.AsSeconds());
	}

	Time operator*(i64 left, Time right)
	{
		return Time::FromMicroseconds(right.AsMicroseconds() * left);
	}

	Time& operator*=(Time& left, f32 right) { return left = left * right; }

	Time& operator*=(Time& left, i64 right) { return left = left * right; }

	Time operator/(Time left, f32 right)
	{
		return Time::FromSeconds(left.AsSeconds() / right);
	}

	Time operator/(Time left, i64 right)
	{
		return Time::FromMicroseconds(left.AsMicroseconds() / right);
	}

	Time& operator/=(Time& left, f32 right) { return left = left / right; }

	Time& operator/=(Time& left, i64 right) { return left = left / right; }

	f32 operator/(Time left, Time right)
	{
		return left.AsSeconds() / right.AsSeconds();
	}

	Time operator%(Time left, Time right)
	{
		return Time::FromMicroseconds(left.AsMicroseconds() % right.AsMicroseconds());
	}

	Time& operator%=(Time& left, Time right) { return left = left % right; }
}