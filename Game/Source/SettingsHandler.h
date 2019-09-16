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
	unsigned int SELECT_CARD_LEFT;
	unsigned int SELECT_CARD_RIGHT;
	unsigned int RESTOCK_DECK;

	unsigned int cursorSensX;
	unsigned int cursorSensY;
	unsigned int cursorInvX;
	unsigned int cursorInvY;

	unsigned int lifebarDecoration;
	unsigned int lifebarValues;

	void LoadSettings(std::string location);

	template <typename NewValue, typename VariableName>
	void SaveSetting(VariableName, NewValue)
	{
		VariableName = NewValue;
	}

	bool WriteToFile();
};
