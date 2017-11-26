#ifndef JGUI_SCHEME_H
#define JGUI_SCHEME_H

#include "jgui_common.h"

namespace jgui
{
	void jgui_export set_scheme_from_file(const char* name);
	void jgui_export set_scheme_from_memory(const char* data);
}

#endif