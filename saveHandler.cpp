#include "saveHandler.h"

namespace fs = std::filesystem;

int saveHandler::save(std::string dir)
{
	int id = 0;
	std::string texture = "/textures";
	std::string data = "/data.txt";
	if (!fs::is_directory(dir) || !fs::exists(dir)) { // Check if src folder exists
		fs::create_directory(dir); // create src folder
	}
	if (!fs::is_directory(dir + texture) || !fs::exists(dir + texture)) { // Check if src folder exists
		fs::create_directory(dir + texture); // create src folder
	}

	std::ofstream MyFile((dir+data).c_str());
	card* temp = link->get(0);
	while (temp != nullptr)
	{
		MyFile << "\n\n@ id="<<temp->id<<"\n";
		MyFile << "type:" << temp->type << "\n";
		MyFile << "random:" << temp->random << "\n";
		MyFile << "title:" << temp->title << "\n";
		MyFile << "img:" << temp->img << "\n";
		MyFile << "text:" << temp->text << "\n";
		MyFile << "min random value 1:" << temp->ra[0] << "\n";
		MyFile << "max random value 1:" << temp->ra[1] << "\n";
		MyFile << "min random value 2:" << temp->ra[2] << "\n";
		MyFile << "max random value 2:" << temp->ra[3] << "\n";
		MyFile << "min random value 3:" << temp->ra[4] << "\n";
		MyFile << "max random value 3:" << temp->ra[5] << "\n";
		MyFile << "conectio0:" << temp->conection[0] << "\n";
		MyFile << "conectio1:" << temp->conection[1] << "\n";
		MyFile << "conectio2:" << temp->conection[2] << "\n";
		MyFile << "conectio3:" << temp->conection[3] << "\n";
		MyFile << "conectio4:" << temp->conection[4] << "\n";
		MyFile << "conectio5:" << temp->conection[5] << "\n";
		MyFile << "conectio6:" << temp->conection[6] << "\n";
		MyFile << "conectio7:" << temp->conection[7] << "\n";
		MyFile << "conectio8:" << temp->conection[8] << "\n";
		MyFile << "conectio9:" << temp->conection[9] << "\n";
		MyFile << "bpm:" << temp->bpm << "\n";
		MyFile << "time:" << temp->time << "\n";
		MyFile << "value1 given:" << temp->valeus[0] << "\n";
		MyFile << "value1 requert:" << temp->valeus[1] << "\n";
		MyFile << "value2 given:" << temp->valeus[2] << "\n";
		MyFile << "value2 requert:" << temp->valeus[3] << "\n";
		MyFile << "value3 given:" << temp->valeus[4] << "\n";
		MyFile << "value3 requert:" << temp->valeus[5] << "\n";
		MyFile << "tag1 give:" << temp->tags[0] << "\n";
		MyFile << "tag1 need:" << temp->tags[1] << "\n";
		MyFile << "tag2 give:" << temp->tags[2] << "\n";
		MyFile << "tag2 need:" << temp->tags[3] << "\n";
		MyFile << "tag3 give:" << temp->tags[4] << "\n";
		MyFile << "tag3 need:" << temp->tags[5] << "\n";


		std::string sourc = "textures/" + temp->img + ".png";
		std::string dest = dir + texture + "/" + temp->img + ".png";
		std::ifstream ifs(sourc, std::ios::binary);
		if (!ifs)
		{
			//std::cout << "Could not open png file.";
			sourc = "textures/" + temp->img + ".jpg";
			dest = dir + texture + "/" + temp->img + ".jpg";
			ifs.close();
			ifs.open(sourc, std::ios::binary);
		}
		if (!ifs)
		{
			//std::cout << "Could not open png file.";
			sourc = "textures/" + temp->img + ".jpeg";
			dest = dir + texture + "/" + temp->img + ".jpeg";
			ifs.close();
			ifs.open(sourc, std::ios::binary);
		}
		if (!ifs)
		{
			//std::cout << "Could not open jpg file.";
		}
		else
		{
			std::ofstream myOutpue;
			char buffer[1024];
			myOutpue.open(dest, std::ios::binary);
			while (ifs.read(buffer, sizeof(buffer)))
			{
				myOutpue.write(buffer, ifs.gcount());
			}
			myOutpue.write(buffer, ifs.gcount());
			myOutpue.close();
		}
		temp = temp->next;
	}
	MyFile << "#";
	MyFile.close();
	return 0;
}

int saveHandler::load(std::string dir)
{
	card* temp = nullptr;
	int id = 0;
	std::string texture = "/textures";
	std::string data = "/data.txt";
	if (!fs::is_directory(dir) || !fs::exists(dir)) 
	{
		return 1;
	}

	if (!fs::is_directory(dir + texture) || !fs::exists(dir + texture)) { // Check if src folder exists
		return 1;
	}

	std::ifstream MyFile((dir + data).c_str(), std::ifstream::in);
	char c = MyFile.get();

	std::cout << c;
	c = MyFile.get();

	while(1)
	{
		if (link->get(id) == nullptr)
		{
			link->appandNode();
			temp = link->get(id);
		}
		else
		{
			temp = link->get(id);
		}

		while (c != ':')
		{
			c = MyFile.get();
		}
		c = MyFile.get();
		temp->type = "";
		while (c != '\n')
		{

			temp->type = temp->type + c ;
			c = MyFile.get();
		}

		while (c != ':')
		{
			c = MyFile.get();
		}
		c = MyFile.get();
		if (c != '0')
		{

			temp->random = true;
		}
		else
		{
			temp->random = false;
		}

		while (c != ':')
		{
			c = MyFile.get();
		}
		c = MyFile.get();
		while (c != '\n')
		{

			temp->title = temp->title + c;
			c = MyFile.get();
		}

		while (c != ':')
		{
			c = MyFile.get();
		}
		c = MyFile.get();
		while (c != '\n')
		{

			temp->img = temp->img + c;
			c = MyFile.get();
		}

		std::string dest = "textures/" + temp->img + ".png";
		std::string sourc = dir + texture + "/" + temp->img + ".png";
		std::ifstream ifs(sourc, std::ios::binary);
		if (!ifs)
		{
			//std::cout << "Could not open png file.";
			sourc = dir + texture + "/" + temp->img + ".jpg";
			dest = "textures/" + temp->img + ".jpg";
			ifs.close();
			ifs.open(sourc, std::ios::binary);
		}
		if (!ifs)
		{
			//std::cout << "Could not open png file.";
			sourc = dir + texture + "/" + temp->img + ".jpeg";
			dest = "textures/" + temp->img + ".jpeg";
			ifs.close();
			ifs.open(sourc, std::ios::binary);
		}
		if (!ifs)
		{
			//std::cout << "Could not open jpg file.";
		}
		else
		{
			std::ofstream myOutpue;
			char buffer[1024];
			myOutpue.open(dest, std::ios::binary);
			while (ifs.read(buffer, sizeof(buffer)))
			{
				myOutpue.write(buffer, ifs.gcount());
			}
			myOutpue.write(buffer, ifs.gcount());
			myOutpue.close();
		}

		while (c != ':')
		{
			c = MyFile.get();
		}
		c = MyFile.get();
		while (c != '\n')
		{

			temp->text = temp->text + c;
			c = MyFile.get();
		}

		for (int i = 0; i < 6; i++)
		{
			while (c != ':')
			{
				c = MyFile.get();
			}
			c = MyFile.get();
			while (c != '\n')
			{
				temp->ra[i] = temp->ra[i] + c;
				c = MyFile.get();
			}
		}


		for (int i = 0; i < 10; i++)
		{
			while (c != ':')
			{
				c = MyFile.get();
			}
			c = MyFile.get();
			while (c != '\n')
			{
				temp->conection[i] = temp->conection[i] + c;
				c = MyFile.get();
			}
		}

		while (c != ':')
		{
			c = MyFile.get();
		}
		c = MyFile.get();
		while (c != '\n')
		{
			temp->bpm = temp->bpm + c;
			c = MyFile.get();
		}

		while (c != ':')
		{
			c = MyFile.get();
		}
		c = MyFile.get();
		while (c != '\n')
		{
			temp->time = temp->time + c;
			c = MyFile.get();
		}

		for (int i = 0; i < 6; i++)
		{
			while (c != ':')
			{
				c = MyFile.get();
			}
			c = MyFile.get();
			while (c != '\n')
			{
				temp->valeus[i] = temp->valeus[i] + c;
				c = MyFile.get();
			}
		}

		for (int i = 0; i < 6; i++)
		{
			while (c != ':')
			{
				c = MyFile.get();
			}
			c = MyFile.get();
			while (c != '\n')
			{
				temp->tags[i] = temp->tags[i] + c;
				c = MyFile.get();
			}
		}

		while (c != '@' && c != '#')
		{
			c = MyFile.get();
		}
		if (c == '#')
		{
			break;
		}
		id++;
	}
	MyFile.close();
	return 0;
}

int saveHandler::stateSave(int id,int* values, linkedMod* tags, std::string dir)
{
	std::string save = "/save.txt";

	std::ofstream MyFile((dir + save).c_str());
	card* temp = link->get(0);
	MyFile << "cardId" << ":" << id << "\n";
	for (int i = 0; i < 3; i++)
	{
		MyFile << "V"<< i << ":" << values[i] << "\n";
	}
	for (int i = 0; tags->get(i) != nullptr; i++)
	{
		MyFile << "tag" << i << ":" << tags->get(i)->data << "\n";
	}
	MyFile << "#";
	return 0;
}

int saveHandler::stateLoad(int* id,int* values, linkedMod* tags, std::string dir)
{
	std::string save = "/save.txt";
	if (!fs::is_directory(dir) || !fs::exists(dir)) 
	{
		return 1;
	}

	std::ifstream MyFile((dir + save).c_str(), std::ifstream::in);
	char c = MyFile.get();

	while (c != ':')
	{
		c = MyFile.get();
	}
	c = MyFile.get();
	*id = 0;
	while (c != '\n')
	{
		*id = *id * 10 + int(c - '0');
		c = MyFile.get();
	}

	c = MyFile.get();
	for (int i = 0; i < 3; i++)
	{
		while (c != ':')
		{
			c = MyFile.get();
		}
		c = MyFile.get();
		while (c != '\n')
		{
			values[i] = values[i] * 10 + int(c-'0');
			c = MyFile.get();
		}
	}
	int y = 0;
	while (1)
	{
		while (c != ':')
		{
			if (c == '#')
			{
				return 0;
			}
			c = MyFile.get();
		}
		c = MyFile.get();
		while (c != '\n')
		{
			if (tags->get(y) == NULL)
			{
				tags->insertNode("");
			}
			tags->get(y)->data += c;
			c = MyFile.get();
		}
		y++;
	}
		
	return 0;
}

std::string saveHandler::fileImport(std::string dirO)
{
	int slasP = -1;
	int dotP = 0;
	while (dirO.find("\\", slasP + 1) != std::string::npos)
	{
		slasP=int(dirO.find("\\", slasP + 1));
	}
	dotP = int(dirO.find("."));

	std::string dest = "textures\\" + dirO.substr(slasP + 1, dirO.size()- slasP - 1);
	if (!std::filesystem::exists(dest))
	{
		std::ifstream ifs(dirO, std::ios::binary);
		std::ofstream myOutpue;
		char buffer[1024];
		myOutpue.open(dest, std::ios::binary);
		while (ifs.read(buffer, sizeof(buffer)))
		{
			myOutpue.write(buffer, ifs.gcount());
		}
		myOutpue.write(buffer, ifs.gcount());
		myOutpue.close();

	}

	return  dirO.substr(slasP + 1, dotP - slasP - 1);
}

