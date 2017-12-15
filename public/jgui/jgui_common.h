#ifndef JGUI_COMMON_H
#define JGUI_COMMON_H

#if _WIN32
#pragma comment(lib, "Winmm.lib")

#ifdef JGUI_DLL
#define jgui_export __declspec( dllexport )
#define jgui_internal
#else
#define jgui_export __declspec( dllimport )
#endif
#else

#ifdef JGUI_DLL
#define jgui_internal
#define jgui_export __attribute__((visibility("default")))
#else
#define jgui_export
#endif

#endif

#define TOKENPASTE2(x, y) x ## y
#define TOKENPASTE(x, y) TOKENPASTE2(x, y)

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

namespace jgui
{
	using i8 = signed char;
	using i16 = signed short;
	using i32 = signed int;
	using i64 = signed long long;

	using u8 = unsigned char;
	using u16 = unsigned short;
	using u32 = unsigned int;
	using u64 = unsigned long long;

	using usize = size_t;

	using f32 = float;
	using f64 = double;
}

#endif
