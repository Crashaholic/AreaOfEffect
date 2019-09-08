#pragma once

#include "gpch.h"
#include "Spell.h"

struct Deck
{
	///what's currently being played
	std::vector<Spell> CurrentDeck;
	///For knowing what to refill deck with
	std::vector<Spell> DeckMemory;
	///Draw a random card
	Spell DrawRandom();
};
