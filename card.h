#pragma once

#include <string>

class card
{
public:
	card()
	{
		this->next = NULL;
		this->img = "";
		this->random = false;
		this->ra[0] = "";
		this->ra[1] = "";
		this->ra[2] = "";
		this->ra[3] = "";
		this->ra[4] = "";
		this->ra[5] = "";
		this->text = "";
		this->type = "0";
		this->layer = 0;
		this->title = "";
		this->id = 0;
		this->conection[0] = "";
		this->conection[1] = "";
		this->conection[2] = "";
		this->conection[3] = "";
		this->conection[4] = "";
		this->conection[5] = "";
		this->conection[6] = "";
		this->conection[7] = "";
		this->conection[8] = "";
		this->conection[9] = "";
		this->bpm = "";
		this->time = "";
		this->valeus[0] = "";
		this->valeus[1] = "";
		this->valeus[2] = "";
		this->valeus[3] = "";
		this->valeus[4] = "";
		this->valeus[5] = "";
		this->tags[0] = "";
		this->tags[1] = "";
		this->tags[2] = "";
		this->tags[3] = "";
		this->tags[4] = "";
		this->tags[5] = "";
	}
	card* next;
	std::string type;
	int layer;
	bool random;
	std::string ra[6];
	int id;
	std::string bpm;
	std::string conection[10];
	std::string time;
	std::string valeus[6];
	std::string tags[6];

	std::string text;
	std::string img;
	std::string title;


};

