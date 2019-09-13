#pragma once

#include "gpch.h"
#include "Spell.h"

struct Deck
{
	///what's currently being played
	std::vector<Spell> deck;
	///Draw a random card
	inline Spell DrawRandom()
	{
		auto index = Math::RandIntMinMax(0, deck.size() - 1);
		auto toReturn = deck[index];
		deck.erase(deck.begin() + index);
		return toReturn;
	}
};
