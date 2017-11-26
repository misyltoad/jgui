#ifndef JGUI_DATA_DESC_H
#define JGUI_DATA_DESC_H

#include <unordered_map>
#include "jgui_common.h"

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
	typedef const DataDesc&(*ParentDesc)();

	using FunctionDesc = std::unordered_map<std::string, DataDescFunc>;

	struct DataDesc
	{
		ParentDesc Parent;
		FunctionDesc Functions;
	};

#define DESC_FUNC_BOOL_WRAPPER(x) \
	void x (const char* value) \
	{ \
		std::string data = std::string(value); \
		std::transform(data.begin(), data.end(), data.begin(), ::tolower); \
		if (data[0] == 'y' || data[0] == 't') \
			x ( true ); \
		else \
			x ( false ); \
	}

#define DESC_FUNC_INT_WRAPPER(x) \
	void x (const char* value) \
	{ \
		x(atoi(value)); \
	}

#define DESC_FUNC_FLOAT_WRAPPER(x) \
	void x (const char* value) \
	{ \
		x(atof(value)); \
	}

#define DESC_FUNC_COLOUR_WRAPPER(x) \
	void x (const char* value) \
	{ \
		x(jgui::Colour(value)); \
	}

	typedef void*(*FactoryFunction)(const char*);

	using ClassFactory = std::unordered_map<std::string, FactoryFunction>;

	jgui_export void* CreateClassByName(const std::string& name);

	extern ClassFactory Factory;

#define DECLARE_CLASS(x, y) \
	typedef y BaseClass;

#define DECLARE_DATA_DESC() \
	virtual const DataDesc& GetDataDesc(); \
	static const DataDesc& StaticGetDataDesc();

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
		::jgui::Factory[#x] = CreateInstanceOf##x ; \
		desc.Parent = nullptr;
	
#define START_DATA_DESC(x) \
	START_DATA_DESC_NO_DESC_PARENT(x) \
	desc.Parent = & x ::BaseClass::StaticGetDataDesc;

#define DATA_DESC_DATA_FUNC(x, y) \
		desc.Functions[#x] = (DataDescFunc) &y ;

#define END_DATA_DESC(x) \
		} \
		DataDesc desc; \
	}; \
	static x##DataDescCreator x##DataDescCreatorInstance; \
	const DataDesc& x ::StaticGetDataDesc() \
	{ \
		return x##DataDescCreatorInstance.desc; \
	} \
	const DataDesc& x::GetDataDesc() \
	{ \
		return StaticGetDataDesc(); \
	}
}

#endif