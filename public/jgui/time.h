#ifndef JGUI_TIME_H
#define JGUI_TIME_H

#include <jgui/jgui_common.h>
#include <chrono>

namespace jgui
{
	class jgui_export Time
	{
	public:
		Time() = default;

		f32 AsSeconds() const;
		i64 AsMilliseconds() const;
		i64 AsMicroseconds() const;

		static const Time Zero;
		static const Time Never;
		static Time Now();
		static void Sleep(Time time);

		inline bool IsZero() { return m_microseconds == 0; }
		inline bool IsNegative() { return m_microseconds < 0; }

		static Time FromSeconds(f32 seconds);
		static Time FromMilliseconds(i64 millis);
		static Time FromMicroseconds(i64 micros);

	private:
		explicit Time(i64 microseconds);

		i64 m_microseconds = { 0 };
	};

	jgui_export bool operator==(Time left, Time right);
	jgui_export bool operator!=(Time left, Time right);

	jgui_export bool operator<(Time left, Time right);
	jgui_export bool operator>(Time left, Time right);

	jgui_export bool operator<=(Time left, Time right);
	jgui_export bool operator>=(Time left, Time right);

	jgui_export Time operator-(Time right);

	jgui_export Time operator+(Time left, Time right);
	jgui_export Time operator-(Time left, Time right);

	jgui_export Time& operator+=(Time& left, Time right);
	jgui_export Time& operator-=(Time& left, Time right);

	jgui_export Time operator*(Time left, f32 right);
	jgui_export Time operator*(Time left, i64 right);
	jgui_export Time operator*(f32 left, Time right);
	jgui_export Time operator*(i64 left, Time right);

	jgui_export Time& operator*=(Time& left, f32 right);
	jgui_export Time& operator*=(Time& left, i64 right);

	jgui_export Time operator/(Time left, f32 right);
	jgui_export Time operator/(Time left, i64 right);

	jgui_export Time& operator/=(Time& left, f32 right);
	jgui_export Time& operator/=(Time& left, i64 right);

	jgui_export f32 operator/(Time left, Time right);

	jgui_export Time operator%(Time left, Time right);
	jgui_export Time& operator%=(Time& left, Time right);
}

#endif