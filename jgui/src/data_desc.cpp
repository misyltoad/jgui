#include <jgui/data_desc.h>
#include <stdio.h>

namespace jgui
{
	void* MissingClass(const char* name)
	{
		printf("Attempted to instance a class I don't know! (%s)\n", name);
		return nullptr;
	}

	void* CreateClassByName(const std::string& name)
	{
		auto index = Factory.find(name);
		if (index == Factory.end())
		{
			MissingClass(name.c_str());
			return nullptr;
		}


		return Factory[name](name.c_str());
	}

	ClassFactory Factory;
}