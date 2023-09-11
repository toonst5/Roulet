#pragma once
#include "cardLinked.h"
#include "linkedMod.h"
#include <windows.h>
#include <filesystem>
#include <fstream>
#include <iostream>


class saveHandler
{
private:
	cardLinked* link;
public:
	saveHandler(cardLinked* linkT) { this->link = linkT; }
	int save(std::string);
	int load(std::string);
	int stateSave(int, int*,linkedMod*, std::string);
	int stateLoad(int*, int*, linkedMod*, std::string);
	std::string broad();
	std::string fileImport(std::string);
};

