#ifndef JGUI_COMMAND_H
#define JGUI_COMMAND_H

#include <jgui/panel.h>

namespace jgui
{
	struct Command
	{
		Panel* Activator;
		std::string CommandString;
	};

	typedef void(*CommandHandlerFunc)(const Command&);

	void ExecuteCommand(const Command& command);

	void AddCommandHandler(const std::string& commandName, CommandHandlerFunc func);

#define COMMAND_HANDLER(x) \
	void TOKENPASTE(CommandHandler_Func_, __LINE__) (const ::jgui::Command& command); \
	class TOKENPASTE(CommandHandler_, __LINE__) \
	{ \
		TOKENPASTE(CommandHandler_, __LINE__) () \
		{ \
			::jgui::AddCommandHandler( #x, TOKENPASTE(CommandHandler_Func_, __LINE__) ); \
		} \
	}; \
	void TOKENPASTE(CommandHandler_Func_, __LINE__) (const ::jgui::Command& command)
}

#endif