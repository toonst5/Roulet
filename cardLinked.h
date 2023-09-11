#pragma once
#include "card.h"
class cardLinked
{
public:
	int max;
	int clear();
	cardLinked() { head = nullptr; max = 0; }
	void appandNode();
	int deleteNode(int);
	int insertNode(int);
	card* get(int);
private:
	card* head;
};

