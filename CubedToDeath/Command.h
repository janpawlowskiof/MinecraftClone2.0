#pragma once
#include <string>
#include "Player.h"
#include <map>
#include <iostream>
#include <sstream>
#include <functional>

class Command
{
private:
	static void Teleport(std::vector<std::string> args)
	{
		Player::position.x = std::stof(args[0]);
		Player::position.y = std::stof(args[1]);
		Player::position.z = std::stof(args[2]);
	}
	static void SelectBlock(std::vector<std::string> args)
	{
		Player::selected_block_id = std::stoi(args[0]);
	}

	std::map<std::string, std::function<void(std::vector<std::string>)>> commands;
public:
	void Initialize()
	{
		commands.emplace("tp", Teleport);
		commands.emplace("select", SelectBlock);
	}

	void Execute(std::string line)
	{
		try
		{
			std::istringstream stream(line);
			std::string command;
			stream >> command;

			std::vector<std::string> arguments;
			while(stream)
			{
				std::string arg;
				stream >> arg;
				arguments.push_back(arg);
			}
			commands[command](arguments);
		}
		catch (std::exception e)
		{
			std::cout << "Error in command\n";
		}
	}
};

