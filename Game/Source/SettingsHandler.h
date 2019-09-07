#pragma once
#include "gpch.h"

struct GameUserSettings
{
	unsigned int windowWidth;
	unsigned int windowHeight;
	unsigned int windowFullscreen;
	unsigned int windowedMode;

	unsigned int MOVE_FORWARD;
	unsigned int MOVE_BACKWARD;
	unsigned int MOVE_LEFT;
	unsigned int MOVE_RIGHT;

	void LoadSettings(std::string location);

	template <typename NewValue, typename VariableName>
	void SaveSetting(VariableName, NewValue)
	{
		VariableName = NewValue;
	}

	bool WriteToFile();
};
