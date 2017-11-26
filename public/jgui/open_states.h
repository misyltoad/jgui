#ifndef JGUI_OPEN_STATES_H
#define JGUI_OPEN_STATES_H

namespace jgui
{
	namespace OpenStates
	{
		enum Enum
		{
			Open,
			Minimised,
			Maximised,
			Fullscreen
		};
	}
	using OpenState = OpenStates::Enum;
}

#endif