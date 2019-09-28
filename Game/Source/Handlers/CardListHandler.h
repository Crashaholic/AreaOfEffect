#pragma once
#include "../gpch.h"
#include "../Spell.h"
#include "SpellFileVersion.h"

void GetCardList(std::vector<Spell>& spellVec, std::string locName)
{
	SpellFromFile beforeSpell;
	std::ifstream inputFile(locName.c_str(), std::ios::binary);
	unsigned int sizeofdata;
	inputFile.read((char*)&sizeofdata, sizeof(unsigned int));
	spellVec.resize(sizeofdata);
	for (size_t i = 0; i < sizeofdata; i++)
	{
		inputFile.read((char*)&beforeSpell, sizeof(SpellFromFile));
		spellVec[i] = Spell();
		spellVec[i].radius = beforeSpell.radius;
		spellVec[i].delay = beforeSpell.delay;
		spellVec[i].duration = beforeSpell.duration;
		spellVec[i].dmg = beforeSpell.dmg;
	}
	inputFile.close();
}