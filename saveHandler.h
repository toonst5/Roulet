#pragma once
#include "cardLinked.h"
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
	std::string broad();
};

