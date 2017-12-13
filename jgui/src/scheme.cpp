#include "jgui/jgui_common.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <jgui_private/string_helpers.h>
#include "jgui/scheme.h"
#include "jgui/data_desc.h"
#include "jgui/panel.h"

namespace jgui
{
	void set_scheme_from_file(const char* name)
	{
		FILE *f = nullptr;
		fopen_s(&f, name, "r");
		if (!f)
			return;

		fseek(f, 0, SEEK_END);

		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);

		char *string = (char*) malloc(fsize + 1);
		fread(string, fsize, 1, f);
		fclose(f);

		string[fsize] = 0;

		set_scheme_from_memory(string);
	}

	void set_scheme_from_memory(const char* data)
	{
		using ObjectData = std::unordered_map<std::string, std::string>;

		std::vector<std::string> lines = split_string(std::string(data), "\n");
		std::vector<ObjectData> objects;
		std::vector<size_t> objectStack;

		for (usize i = 0; i < lines.size(); i++)
		{
			auto& line = lines[i];
			line = trim(line);

			if (line == "{" || line.empty())
			{

			}
			else if (line == "}")
				objectStack.pop_back();
			else if (contains(line, ":"))
			{
				std::string key;
				std::string value;

				key = line;
				if (key.find(":") != key.npos)
					key = key.substr(0, key.find(":"));
				else
					continue;

				key = remove(key, '"');
				key = trim(key);

				value = remove(remove(line.substr(line.find(":") + 1), '"'), ':');

				if (value.find(":") != value.npos)
					value = value.substr(value.find(":") + 1);

				value = trim(value);
				value = remove(value, '"');
				objects[objectStack[objectStack.size() - 1]][key] = value;
			}
			else
			{
				ObjectData data;
				if (objectStack.size() > 0)
					data["Parent"] = objects[objectStack[objectStack.size() - 1]]["Name"];

				data["Name"] = remove(line, '"');
				objects.push_back(data);
				objectStack.push_back(objects.size() - 1);
			}
		}

		for (auto object : objects)
		{
			auto index = object.find("Type");
			if (object.end() == index)
				continue;

			jgui::Panel* panel = (jgui::Panel*) CreateClassByName(object["Type"].c_str());
			if (panel)
			{
				for (auto kv : object)
				{
					if (kv.first == "Type")
						continue;

					const DataDesc* dataDesc = &panel->GetDataDesc();
					while (dataDesc)
					{
						auto funcs = dataDesc->DataFuncs;
						auto index = funcs->find(kv.first);
						if (index == funcs->end())
						{
							if (dataDesc->Parent)
								dataDesc = & dataDesc->Parent();
							else
								dataDesc = nullptr;

							continue;
						}

						std::invoke((*funcs)[kv.first], panel, kv.second.c_str());
						break;
					}

					if (!dataDesc)
						printf("Attempted to set property but I don't know how! (%s)\n", kv.first.c_str());
				}
				panel->OnCreated();
			}
		}
	}
}