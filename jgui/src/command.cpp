#include <jgui/command.h>

namespace jgui
{
	std::unordered_multimap<std::string, CommandHandlerFunc> CommandHandlers;

	void ExecuteCommand(const Command& command)
	{
		if (CommandHandlers.find(command.CommandString) == CommandHandlers.end())
		{
			printf("Cannot find a command handler for command: %s.\n", command.CommandString.c_str());
			return;
		}

		auto its = CommandHandlers.equal_range(command.CommandString);
		for (auto it = its.first; it != its.second; ++it)
			it->second(command);
	}

	void AddCommandHandler(const std::string& command, CommandHandlerFunc func)
	{
		CommandHandlers.insert({ command, func });
	}
}