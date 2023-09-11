#pragma once
#include "card.h"
class cardLinked
{
public:
	int clear();
	cardLinked() { head = nullptr; }
	void insertNode();
	int deleteNode(int);
	card* get(int);
private:
	card* head;
};

