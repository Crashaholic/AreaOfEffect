#include "gpch.h"
#include "SettingsHandler.h"

void GameUserSettings::LoadSettings(std::string location)
{
	std::fstream SettingsFile;
	SettingsFile.open("game.ini");
	std::string line;
	while (std::getline(SettingsFile, line))
	{
		if (line.find("windowwidth") != std::string::npos)
		{
			windowWidth = std::stoul(line.substr(line.find('=') + 1));
			continue;
		}
		if (line.find("windowheight") != std::string::npos)
		{
			windowHeight = std::stoul(line.substr(line.find('=') + 1));
			continue;
		}
		if (line.find("fullscreen") != std::string::npos)
		{
			windowFullscreen = std::stoi(line.substr(line.find('=') + 1));
			continue;
		}
		if (line.find("windowed") != std::string::npos)
		{
			windowedMode = std::stoi(line.substr(line.find('=') + 1));
			continue;
		}
		if (line.find("MOVE_FORWARD") != std::string::npos)
		{
			MOVE_FORWARD = std::stoul(line.substr(line.find('=') + 1));
			continue;
		}
		if (line.find("MOVE_BACKWARD") != std::string::npos)
		{
			MOVE_BACKWARD = std::stoul(line.substr(line.find('=') + 1));
			continue;
		}
		if (line.find("MOVE_LEFT") != std::string::npos)
		{
			MOVE_LEFT = std::stoul(line.substr(line.find('=') + 1));
			continue;
		}
		if (line.find("MOVE_RIGHT") != std::string::npos)
		{
			MOVE_RIGHT = std::stoul(line.substr(line.find('=') + 1));
			continue;
		}
		if (line.find("cursorSensX") != std::string::npos)
		{
			cursorSensX = std::stoul(line.substr(line.find('=') + 1));
			continue;
		}
		if (line.find("cursorSensY") != std::string::npos)
		{
			cursorSensY = std::stoul(line.substr(line.find('=') + 1));
			continue;
		}
		if (line.find("cursorInvX") != std::string::npos)
		{
			cursorInvX = std::stoul(line.substr(line.find('=') + 1));
			continue;
		}
		if (line.find("cursorInvY") != std::string::npos)
		{
			cursorInvY = std::stoul(line.substr(line.find('=') + 1));
			continue;
		}
	}

}

bool GameUserSettings::WriteToFile()
{
	return false;
}

