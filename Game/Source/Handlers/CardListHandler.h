#pragma once
#include "../gpch.h"
#include "../Spell.h"

void GetCardList(std::vector<Spell>& spellVec, std::string locName)
{
	std::ifstream inputFile(locName.c_str(), std::ios::binary);
	unsigned int sizeofdata;
	inputFile.read((char*)&sizeofdata, sizeof(unsigned int));
	spellVec.resize(sizeofdata);
	for (size_t i = 0; i < sizeofdata; i++)
		inputFile.read((char*)&spellVec[i], sizeof(Spell));
	inputFile.close();
}