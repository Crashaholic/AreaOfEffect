#pragma once

#include "gpch.h"
#include "Spell.h"

struct Deck
{
	std::vector<Spell> CurrentDeck;

	Spell DrawRandom();
};
