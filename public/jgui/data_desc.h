#ifndef JGUI_DATA_DESC_H
#define JGUI_DATA_DESC_H

#include <unordered_map>
#include "jgui_common.h"
#include <jgui/time.h>

#include <algorithm>
#include <string> 
#include <cctype>
#include <clocale>
#include <stdlib.h>

namespace jgui
{
	class Panel;
	struct DataDesc;

	typedef void(Panel::*DataDescFunc)(const char*);
	typedef DataDesc&(*ParentDesc)();

	using FunctionDesc = std::unordered_map<std::string, DataDescFunc>;

	struct jgui_export DataDesc
	{
		ParentDesc Parent;
		FunctionDesc* DataFuncs;
	};

#define IMPL_QUICK_SET_TYPE_IMP(classType, x, type) \
	void classType :: Set##x (type value)

#define IMPL_QUICK_SET_TYPE_OPEN_UNCLOSED(x, type) \
	void Set##x (type value)

#define IMPL_MAIN_QUICK_SET_STRING(x) \
	IMPL_QUICK_SET_TYPE_OPEN_UNCLOSED(x, const char*) \
	{ \
		Set##x(std::string(value)); \
	} \
	IMPL_QUICK_SET_TYPE_OPEN_UNCLOSED(x, const std::string&)

#define IMPL_MAIN_QUICK_SET_BOOL(x) \
	void Set##x (const char* value) \
	{ \
		std::string data = std::string(value); \
		std::transform(data.begin(), data.end(), data.begin(), ::tolower); \
		if (data[0] == 'y' || data[0] == 't') \
			Set##x(true); \
		else \
			Set##x(false); \
	} \
	IMPL_QUICK_SET_TYPE_OPEN_UNCLOSED(x, bool)

#define IMPL_MAIN_QUICK_SET_INT64(x) \
	void Set##x (const char* value) \
	{ \
		Set##x(atoi(value)); \
	} \
	IMPL_QUICK_SET_TYPE_OPEN_UNCLOSED(x, ::jgui::i64)

#define IMPL_MAIN_QUICK_SET_INT32(x) \
	void Set##x (const char* value) \
	{ \
		Set##x(atoi(value)); \
	} \
	IMPL_QUICK_SET_TYPE_OPEN_UNCLOSED(x, ::jgui::i32)

#define IMPL_MAIN_QUICK_SET_FLOAT(x) \
	void Set##x (const char* value) \
	{ \
		Set##x((f32)atof(value)); \
	} \
	IMPL_QUICK_SET_TYPE_OPEN_UNCLOSED(x, ::jgui::f32)

#define IMPL_MAIN_QUICK_SET_COLOUR(x) \
	void Set##x (const char* value) \
	{ \
		Set##x(jgui::Colour(value)); \
	} \
	IMPL_QUICK_SET_TYPE_OPEN_UNCLOSED(x, ::jgui::Colour)

	// GET

#define IMPL_QUICK_GET_TYPE(x, type) \
	type Get##x ()  \
	{  \
		return m_ ## x;  \
	}

#define IMPL_QUICK_GET_STRING_TOSTRING(x) \
	std::string Get##x##AsString() const  \
	{   \
		return std::to_string( m_ ##x ); \
	}

#define IMPL_QUICK_GET_STRING_IMPL_TOSTRING(x) \
	std::string Get##x##AsString() const  \
	{   \
		return m_ ## x .ToString(); \
	}

#define IMPL_QUICK_GET_STRING_BOOL(x) \
	std::string Get##x##AsString() const   \
	{   \
		if (m_ ## x)   \
			return "true";   \
		else   \
			return "false";   \
	}

#define IMPL_MAIN_QUICK_GET_FLOAT(x) \
	IMPL_QUICK_GET_TYPE(x, ::jgui::f32)  \
	IMPL_QUICK_GET_STRING_TOSTRING(x)

#define IMPL_MAIN_QUICK_GET_STRING(x) \
	std::string Get##x ()  \
	{  \
		return m_##x;  \
	} \
	std::string Get##x##AsString ()  \
	{  \
		return m_##x;  \
	}

#define IMPL_MAIN_QUICK_GET_INT32(x) \
	IMPL_QUICK_GET_TYPE(x, ::jgui::i32)  \
	IMPL_QUICK_GET_STRING_TOSTRING(x)

#define IMPL_MAIN_QUICK_GET_INT64(x) \
	IMPL_QUICK_GET_TYPE(x, ::jgui::i64)  \
	IMPL_QUICK_GET_STRING_TOSTRING(x)

#define IMPL_MAIN_QUICK_GET_COLOUR(x) \
	IMPL_QUICK_GET_TYPE(x, ::jgui::Colour)  \
	IMPL_QUICK_GET_STRING_IMPL_TOSTRING(x)

#define IMPL_MAIN_QUICK_GET_BOOL(x) \
	IMPL_QUICK_GET_TYPE(x, bool)  \
	IMPL_QUICK_GET_STRING_BOOL(x)

#define QUICK_GETSET_BOOL(x) \
	IMPL_MAIN_QUICK_GET_BOOL(x) \
	IMPL_MAIN_QUICK_SET_BOOL(x)

#define QUICK_GETSET_FLOAT(x) \
	IMPL_MAIN_QUICK_GET_FLOAT(x) \
	IMPL_MAIN_QUICK_SET_FLOAT(x)

#define QUICK_GETSET_INT32(x) \
	IMPL_MAIN_QUICK_GET_INT32(x) \
	IMPL_MAIN_QUICK_SET_INT32(x)

#define QUICK_GETSET_INT64(x) \
	IMPL_MAIN_QUICK_GET_INT64(x) \
	IMPL_MAIN_QUICK_SET_INT64(x)

#define QUICK_GETSET_COLOUR(x) \
	IMPL_MAIN_QUICK_GET_COLOUR(x) \
	IMPL_MAIN_QUICK_SET_COLOUR(x)

#define QUICK_GETSET_STRING(x) \
	IMPL_MAIN_QUICK_GET_STRING(x) \
	IMPL_MAIN_QUICK_SET_STRING(x)

#define IMPL_QUICK_MEMBER_TYPE(x, y) \
	protected: \
		y m_##x ; \
	public:

#define QUICK_MEMBER_STRING(x) \
	IMPL_QUICK_MEMBER_TYPE(x, std::string) \
	QUICK_GETSET_STRING(x)

#define QUICK_MEMBER_BOOL(x) \
	IMPL_QUICK_MEMBER_TYPE(x, bool) \
	QUICK_GETSET_BOOL(x)

#define QUICK_MEMBER_INT32(x) \
	IMPL_QUICK_MEMBER_TYPE(x, ::jgui::i32) \
	QUICK_GETSET_INT32(x)

#define QUICK_MEMBER_INT64(x) \
	IMPL_QUICK_MEMBER_TYPE(x, ::jgui::i64) \
	QUICK_GETSET_INT64(x)

#define QUICK_MEMBER_FLOAT(x) \
	IMPL_QUICK_MEMBER_TYPE(x, ::jgui::f32) \
	QUICK_GETSET_FLOAT(x)

#define QUICK_MEMBER_COLOUR(x) \
	IMPL_QUICK_MEMBER_TYPE(x, ::jgui::Colour) \
	QUICK_GETSET_COLOUR(x)

#define QUICK_MEMBER_IMPLEMENT_BOOL(classType, x) \
		IMPL_QUICK_SET_TYPE_IMP(classType, x, bool)

#define QUICK_MEMBER_IMPLEMENT_STRING(classType, x) \
		IMPL_QUICK_SET_TYPE_IMP(classType, x, const std::string&)

#define QUICK_MEMBER_IMPLEMENT_FLOAT(classType, x) \
		IMPL_QUICK_SET_TYPE_IMP(classType, x, ::jgui::f32)

#define QUICK_MEMBER_IMPLEMENT_INT32(classType, x) \
		IMPL_QUICK_SET_TYPE_IMP(classType, x, ::jgui::i32)

#define QUICK_MEMBER_IMPLEMENT_INT64(classType, x) \
		IMPL_QUICK_SET_TYPE_IMP(classType, x, ::jgui::i64)

#define QUICK_MEMBER_IMPLEMENT_COLOUR(classType, x) \
		IMPL_QUICK_SET_TYPE_IMP(classType, x, ::jgui::Colour)

#define QUICK_MEMBER_IMPL_VALUE_SET(x) \
		{ \
			m_##x = value; \
		}

#define QUICK_MEMBER_IMPLEMENT_BOOL_SIMPLE(classType, x) \
		IMPL_QUICK_SET_TYPE_IMP(classType, x, bool)	\
		QUICK_MEMBER_IMPL_VALUE_SET(x)

#define QUICK_MEMBER_IMPLEMENT_STRING_SIMPLE(classType, x) \
		IMPL_QUICK_SET_TYPE_IMP(classType, x, const std::string&) \
		QUICK_MEMBER_IMPL_VALUE_SET(x)

#define QUICK_MEMBER_IMPLEMENT_FLOAT_SIMPLE(classType, x) \
		IMPL_QUICK_SET_TYPE_IMP(classType, x, ::jgui::f32) \
		QUICK_MEMBER_IMPL_VALUE_SET(x)

#define QUICK_MEMBER_IMPLEMENT_INT32_SIMPLE(classType, x) \
		IMPL_QUICK_SET_TYPE_IMP(classType, x, ::jgui::i32) \
		QUICK_MEMBER_IMPL_VALUE_SET(x)

#define QUICK_MEMBER_IMPLEMENT_INT64_SIMPLE(classType, x) \
		IMPL_QUICK_SET_TYPE_IMP(classType, x, ::jgui::i64) \
		QUICK_MEMBER_IMPL_VALUE_SET(x)

#define QUICK_MEMBER_IMPLEMENT_COLOUR_SIMPLE(classType, x) \
		IMPL_QUICK_SET_TYPE_IMP(classType, x, ::jgui::Colour) \
		QUICK_MEMBER_IMPL_VALUE_SET(x)


	typedef void*(*FactoryFunction)(const char*);

	using ClassFactory = std::unordered_map<std::string, FactoryFunction>;

	jgui_export void* CreateClassByName(const std::string& name);

	extern jgui_export ClassFactory Factory;

#define DECLARE_CLASS(x, y) \
	public: \
	typedef y BaseClass;

#define DECLARE_DATA_DESC() \
public: \
	virtual ::jgui::DataDesc& GetDataDesc(); \
	static ::jgui::DataDesc& StaticGetDataDesc(); \

#define START_DATA_DESC_NO_DESC_PARENT(x) \
	void* CreateInstanceOf##x(const char*) \
	{ \
		return (void*) new x; \
	} \
	class x##DataDescCreator \
	{ \
		public: \
		x##DataDescCreator() \
		{ \
		::jgui::Factory[#x] = CreateInstanceOf##x; \
		desc.Parent = nullptr;  \
		desc.DataFuncs = new ::jgui::FunctionDesc(); 

#define START_DATA_DESC(x) \
	START_DATA_DESC_NO_DESC_PARENT(x) \
	desc.Parent = & x ::BaseClass::StaticGetDataDesc;

#define DATA_DESC_DATA_FUNC(x, y) \
		(*desc.DataFuncs)[#x] = (::jgui::DataDescFunc) &y ;

#define END_DATA_DESC(x) \
		} \
		::jgui::DataDesc desc; \
	}; \
	static x##DataDescCreator x##DataDescCreatorInstance; \
	::jgui::DataDesc& x ::StaticGetDataDesc() \
	{ \
		return x##DataDescCreatorInstance.desc; \
	} \
	::jgui::DataDesc& x::GetDataDesc() \
	{ \
		return StaticGetDataDesc(); \
	}
}

#endif