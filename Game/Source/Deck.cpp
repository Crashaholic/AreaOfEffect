#include "gpch.h"
#include "Deck.h"

Spell Deck::DrawRandom()
{
	Math::RandIntMinMax(0, CurrentDeck.size() - 1);
	return Spell();
}
