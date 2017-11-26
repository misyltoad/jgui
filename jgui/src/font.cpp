#include "jgui/font.h"
#include "jgui_private/globals.h"

#include <string>
#include <vector>

namespace jgui
{
	void register_font(const char* name, const u8* data, u32 size)
	{
		FontData localData;
		localData.Name = std::string(name);
		localData.Size = size;
		localData.Data = (u8*) malloc(size);
		memcpy(localData.Data, data, size);
		globals::fontData.push_back(localData);

		for (usize i = 0; i < globals::ActiveWindows.size(); i++)
			nvgCreateFontMem(globals::ActiveWindows[i].vg, localData.Name.c_str(), localData.Data, localData.Size, 0);
	}
}