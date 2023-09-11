#include "draw.h"

using namespace std;

int draw::createWindow()
{

	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		GAME_STATE = MAIN;
		//Load media
		int velt_last;
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			Sans = TTF_OpenFont("font/Sans.ttf", 30);
			SansS = TTF_OpenFont("font/Sans.ttf", 15);
			SansB = TTF_OpenFont("font/Sans.ttf", 45);
			white = { 255, 255, 255 };
			red = { 255, 0, 0 };
			green = { 0, 255, 0 };
			link = new cardLinked();
			tags = new linkedMod();
			veld = -1;
			this->rands[0] = rand();
			this->rands[1] = rand();
			this->rands[2] = rand();
			cardTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
			cardPrevTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH/2, SCREEN_HEIGHT);
			values[0] = 0;
			values[1] = 0;
			values[2] = 0;
			cheats = false;
			offset=0;
			string* lastTextI=NULL;

			srand((unsigned)time(NULL));
			if (Sans == NULL)
			{
				printf("%s/n", TTF_GetError());
				return 1;
			}

			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while (!quit)
			{
				mousePres = false;
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}else if (e.type == SDL_MOUSEMOTION)
					{
						SDL_GetMouseState(&xMouse, &yMouse);
						//printf("x:%d\ny:%d\n\n", xMouse, yMouse);
					}else if (e.type == SDL_MOUSEBUTTONDOWN)
					{
						update=true;
						mousePres=true;
						//printf("x:%d\ny:%d\n\n", xMouse, yMouse);
					}
					else if (e.type == SDL_KEYDOWN )
					{
						update = true;
						//Handle backspace
						if (e.key.keysym.sym == SDLK_BACKSPACE && inputText->length() > 0 && offset>0)
						{
							//inputText->pop_back();
							inputText->erase(offset-1, 1);
							offset--;
						}
						if (e.key.keysym.sym == SDLK_DELETE && inputText->length() > 0 && offset < inputText->length())
						{
							//inputText->pop_back();
							inputText->erase(offset , 1);
						}
						if (e.key.keysym.sym == SDLK_LEFT && inputText->length() > 0)
						{
							//inputText->pop_back();
							offset--;
						}
						if (e.key.keysym.sym == SDLK_RIGHT && inputText->length() > offset)
						{
							//inputText->pop_back();
							offset++;
						}
					}else if (e.type == SDL_TEXTINPUT)
					{
						update = true;
						//Not copy or pasting
						if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
						{
							//Append character
							inputText->insert(offset, e.text.text);
							//*inputText += e.text.text;
							offset++;
						}
					}

				}
				//Clear screen
				//SDL_SetRenderDrawColor(gRenderer, 0xF7, 0xC0, 0xDC, 0xFF);
				SDL_SetRenderDrawColor(gRenderer, 0xF7, 0xC0, 0x00, 0xFF);
				SDL_RenderClear(gRenderer);
			
				velt_last = veld;

				switch (GAME_STATE)
				{
				case MAIN:
					id = 0;
					mainMenu();
					break;
				case EDDITOR:
					edditor();
					break;
				case EDDITORMENU:
					id = 0;
					edditorMenu();
					break;
				case EDDITORSAVE:
					id = 0;
					edditorSave();
					break;
				case LOAD:
					id = 0;
					load();
					break;
				case PLAY:
					play();
					break;
				case PLAYSELECT:
					playSelect();
					break;
				case SETTINGS:
					settings();
					break;
				case QUIT:
					quit = true;
					break;
				default:
					return 1;
				}
				if (velt_last != veld)
				{
					text.clear();
					//text = "0";
				}
				if (inputText != lastTextI)
				{
					if (inputText != NULL)
					{
						offset = inputText->length();
					}
					lastTextI = inputText;
				}
				for (int i = 0; i < 3; i++)
				{
					if (values[i] < 0)
					{
						values[i] = 0;
					}
				}

				if (offset < 0)
				{
					offset = 0;
				}
				else if (inputText != NULL&&offset > inputText->length())
				{
					offset = inputText->length();
				}

				//Update screen

				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free resources and close SDL
	close();
	return 0;

}

bool draw::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			std::cout << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
			success = false;
		}

		if (TTF_Init() < 0) {
			std::cout << "Couldn't initialize TTF lib: " << TTF_GetError() << std::endl;
			success = false;
		}
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("Roulet", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}


bool draw::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load PNG texture
	gTextureButton = loadTexture("textures/button.png");
	if (gTextureButton == NULL)
	{
		printf("Failed to load texture image1!\n");
		success = false;
	}

	gTextureButtonPres = loadTexture("textures/button_pressed.png");
	if (gTextureButtonPres == NULL)
	{
		printf("Failed to load texture image2!\n");
		success = false;
	}

	return success;
}

void draw::close()
{
	//Free loaded image
	SDL_DestroyTexture(gTextureButton);
	SDL_DestroyTexture(gTextureButtonPres);
	SDL_DestroyTexture(cardTexture);
	SDL_DestroyTexture(cardPrevTexture);
	gTextureButton = NULL;
	gTextureButtonPres = NULL;
	cardTexture = NULL;
	cardPrevTexture = NULL;

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* draw::loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		//printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			//printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

int draw::mainMenu()
{
	int y=0;
	int w = 300;
	int h = 90;
	SDL_Surface* surfaceText = TTF_RenderText_Solid(Sans, "Roulet", white);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
	SDL_FreeSurface(surfaceText);

	SDL_Rect stretchRect;
	stretchRect.x = SCREEN_WIDTH / 2 - 100;
	stretchRect.y = 10;
	stretchRect.w = 200;
	stretchRect.h = 50;
	SDL_RenderCopy(gRenderer, Message, NULL, &stretchRect);
	SDL_DestroyTexture(Message);

	y = 300;
	stretchRect.h = h;
	stretchRect.w = w;
	stretchRect.x = (SCREEN_WIDTH - stretchRect.w) / 2;
	stretchRect.y = y - stretchRect.h / 2;
	if (drawButton(stretchRect, "Play"))
	{
		next = PLAY;
		GAME_STATE = LOAD;
	}

	y = 400;
	stretchRect.h = h;
	stretchRect.w = w;
	stretchRect.x = (SCREEN_WIDTH - stretchRect.w) / 2;
	stretchRect.y = y - stretchRect.h / 2;
	if (drawButton(stretchRect,"Eddit"))
	{
		GAME_STATE = EDDITORMENU;
	}

	y = 500;
	stretchRect.h = h;
	stretchRect.w = w;
	stretchRect.x = (SCREEN_WIDTH - stretchRect.w) / 2;
	stretchRect.y = y - stretchRect.h / 2;
	if (drawButton(stretchRect, "Settings"))
	{
		GAME_STATE = SETTINGS;
	}

	y = 600;
	stretchRect.h = h;
	stretchRect.w = w;
	stretchRect.x = (SCREEN_WIDTH - stretchRect.w) / 2;
	stretchRect.y = y - stretchRect.h / 2;
	if (drawButton(stretchRect, "Quit"))
	{
		GAME_STATE = QUIT;
	}

	return 0;
}

int draw::edditor()
{
	card* temp;
	if (link->get(id) == nullptr)
	{
		link->insertNode();
		temp = link->get(id);
	}
	else
	{
		temp = link->get(id);
	}

	int x = 0;
	int w = 150;
	int h = 40;
	int y = SCREEN_HEIGHT - 50;
	string title = "Edditor card: " + to_string(temp->id);
	SDL_Surface* surfaceText = TTF_RenderText_Solid(Sans, title.c_str(), white);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
	SDL_FreeSurface(surfaceText);

	SDL_Rect stretchRect;
	stretchRect.x = SCREEN_WIDTH / 2 - 100;
	stretchRect.y = 10;
	stretchRect.w = 200;
	stretchRect.h = 50;
	SDL_RenderCopy(gRenderer, Message, NULL, &stretchRect);
	SDL_DestroyTexture(Message);


	x = 1000;
	stretchRect.h = h;
	stretchRect.w = w;
	stretchRect.x = x;
	stretchRect.y = y;
	if (drawButton(stretchRect, "Next"))
	{
		id++;
		veld = -1;
	}

	if (id != 0)
	{
		x = 10;
		stretchRect.h = h;
		stretchRect.w = w;
		stretchRect.x = x;
		stretchRect.y = y;
		if (drawButton(stretchRect, "Back"))
		{
			id--;
			veld = -1;
		}
	}

	x = 300;
	stretchRect.h = h;
	stretchRect.w = w;
	stretchRect.x = x;
	stretchRect.y = y;
	if (drawButton(stretchRect, "Save"))
	{
		GAME_STATE = EDDITORSAVE;
	}

	x = 700;
	stretchRect.h = h;
	stretchRect.w = w;
	stretchRect.x = x;
	stretchRect.y = y;
	if (drawButton(stretchRect, "Exit"))
	{
		GAME_STATE = MAIN;
	}




	stretchRect.x = 10;
	stretchRect.y = 10;
	stretchRect.h = 50;
	stretchRect.w = 250;
	if (temp->title.empty())
	{
		drawTextBlock(1, stretchRect, "title", Sans, &temp->title);
	}
	else
	{
		drawTextBlock(1, stretchRect, temp->title, Sans, &temp->title);
	}


	stretchRect.x = 10;
	stretchRect.y = 70;
	stretchRect.h = 250;
	stretchRect.w = 250;
	if (temp->text.empty())
	{
		drawTextBlock(2, stretchRect, "text for the card with x y z as random stats", SansS, &temp->text);
	}
	else
	{
		drawTextBlock(2, stretchRect, temp->text, SansS, &temp->text);
	}


	stretchRect.x = 10;
	stretchRect.y = 330;
	stretchRect.h = 50;
	stretchRect.w = 250;
	if (temp->img.empty())
	{
		drawTextBlock(3, stretchRect, "image name", Sans, &temp->img);
	}
	else
	{
		drawTextBlock(3, stretchRect, temp->img, Sans, &temp->img);
	}


	for (int i = 0; i < 10; i++)
	{
		stretchRect.x = 5 + i%5*60;
		stretchRect.y = 390 + i/5*60;
		stretchRect.h = 50;
		stretchRect.w = 50;
		if (temp->conection[i].empty())
		{
			drawTextBlock(4 + i, stretchRect, "id", Sans, &temp->conection[i]);
		}
		else
		{
			drawTextBlock(4 + i, stretchRect, temp->conection[i], Sans, &temp->conection[i]);
		}
		
	}

	for (int i = 0; i < 6; i++)
	{
		if (!text.empty() && veld == 14 + i)
		{
			if (!text.empty())
			{
				temp->ra[i] = stoi(text);
			}
			else
			{
				if (i % 2)
				{
					temp->ra[i] = "10";
				}
				else
				{
					temp->ra[i] = "0";
				}
			}
			
		}
		stretchRect.x = 10 + i / 2 * 60;
		stretchRect.y = 510 + i % 2 * 60;
		stretchRect.h = 50;
		stretchRect.w = 50;
		if (temp->ra[i] == "0"  || temp->ra[i] == "10")
		{
			if (i % 2)
			{
				drawTextBlock(14 + i, stretchRect, "max", Sans, &temp->ra[i]);
			}else
			{
				drawTextBlock(14 + i, stretchRect, "min",Sans, &temp->ra[i]);
			}
		}
		else
		{
			drawTextBlock(14 + i, stretchRect, temp->ra[i], Sans, &temp->ra[i]);
		}
	}

	stretchRect.x = 190;
	stretchRect.y = 510;
	stretchRect.h = 50;
	stretchRect.w = 60;
	if (temp->bpm.empty())
	{
		drawTextBlock(20, stretchRect, "bpm", Sans, &temp->bpm);
	}
	else
	{
		drawTextBlock(20, stretchRect, temp->bpm, Sans, &temp->bpm);
	}

	stretchRect.x = 190;
	stretchRect.y = 570;
	stretchRect.h = 50;
	stretchRect.w = 70;
	if (temp->time.empty())
	{
		drawTextBlock(21, stretchRect, "time", Sans, &temp->time);
	}
	else
	{
		drawTextBlock(21, stretchRect, temp->time, Sans, &temp->time);
	}

	for (int i = 0; i < 6; i++)
	{
		stretchRect.x = 270;
		stretchRect.y = 10 + i * 60;
		stretchRect.h = 50;
		stretchRect.w = 100;
		if (temp->valeus[i].empty())
		{
			if (i % 2)
			{
				drawTextBlock(22 + i, stretchRect, ("need "+to_string(i/2+1)).c_str(), Sans, &temp->valeus[i]);
			}
			else
			{
				drawTextBlock(22 + i, stretchRect, ("give " + to_string(i / 2+1)).c_str(), Sans, &temp->valeus[i]);
			}
		}
		else
		{
			drawTextBlock(22+i, stretchRect, temp->valeus[i], Sans, &temp->valeus[i]);
		}
	}

	for (int i = 0; i < 6; i++)
	{
		stretchRect.x = 380+i/2*130;
		stretchRect.y = 70 + i%2 * 60;
		stretchRect.h = 50;
		stretchRect.w = 120;
		if (temp->tags[i].empty())
		{
			if (i % 2)
			{
				drawTextBlock(28 + i, stretchRect, "need tag", Sans, &temp->tags[i]);
			}
			else
			{
				drawTextBlock(28 + i, stretchRect, "give tag", Sans, &temp->tags[i]);
			}
		}
		else
		{
			drawTextBlock(28 + i, stretchRect, temp->tags[i], Sans, &temp->tags[i]);
		}
	}

	stretchRect.x = 10;
	stretchRect.y = 630;
	stretchRect.h = 50;
	stretchRect.w = 100;
	if (temp->random)
	{
		if (drawButton(stretchRect, "random"))
		{
			temp->random = false;
		}
	}
	else
	{
		if (drawButton(stretchRect, "choice"))
		{
			temp->random = true;
		}
	}
	stretchRect.x = 120;
	stretchRect.y = 630;
	stretchRect.h = 50;
	stretchRect.w = 100;
	string type= temp->type;
	if (drawButton(stretchRect, "type:"+ type))
	{
		temp->type =to_string((stoi(temp->type)+1)%2);
	}

	stretchRect.x = SCREEN_WIDTH / 8 * 3;
	stretchRect.y = SCREEN_HEIGHT / 8 * 2;
	stretchRect.w = SCREEN_WIDTH / 8 * 5;
	stretchRect.h = SCREEN_HEIGHT / 8 * 5;

	if (update)
	{
		drawCard();
		update = false;
	}
	SDL_RenderCopy(gRenderer, cardTexture, NULL, &stretchRect);

	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderDrawRect(gRenderer, &stretchRect);

	if (veld < 0)
	{
		inputText = nullptr;
		SDL_StopTextInput();
	}
	return 0;
}

int draw::edditorSave()
{

	string title = "Name of roulet";
	SDL_Surface* surfaceText = TTF_RenderText_Solid(Sans, title.c_str(), white);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
	SDL_FreeSurface(surfaceText);

	SDL_Rect stretchRect;
	stretchRect.x = SCREEN_WIDTH / 2 - 100;
	stretchRect.y = 10;
	stretchRect.w = 200;
	stretchRect.h = 50;
	SDL_RenderCopy(gRenderer, Message, NULL, &stretchRect);
	SDL_DestroyTexture(Message);



	stretchRect.h = 50;
	stretchRect.w = 250;
	stretchRect.x = (SCREEN_WIDTH - stretchRect.w) / 2;
	stretchRect.y = 120;
	drawTextBlock(1, stretchRect, text, Sans, &text);

	stretchRect.h = 50;
	stretchRect.w = 200;
	stretchRect.x = (SCREEN_WIDTH - stretchRect.w) / 2;
	stretchRect.y = SCREEN_HEIGHT - stretchRect.h -10;
	if (drawButton(stretchRect,"Save"))
	{
		saveHandler save(link);
		save.save(text);
		veld = -1;
		text.clear();
			
		GAME_STATE = EDDITOR;
	}


	return 0;
}

int draw::edditorMenu()
{
	int y=0;
	int w = 300;
	int h = 90;
	SDL_Surface* surfaceText = TTF_RenderText_Solid(Sans, "Edditor", white);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
	SDL_FreeSurface(surfaceText);

	SDL_Rect stretchRect;
	stretchRect.x = SCREEN_WIDTH / 2 - 100;
	stretchRect.y = 10;
	stretchRect.w = 200;
	stretchRect.h = 50;
	SDL_RenderCopy(gRenderer, Message, NULL, &stretchRect);
	SDL_DestroyTexture(Message);

	y = 300;
	stretchRect.h = h;
	stretchRect.w = w;
	stretchRect.x = (SCREEN_WIDTH - stretchRect.w) / 2;
	stretchRect.y = y - stretchRect.h / 2;
	if (drawButton(stretchRect, "New"))
	{
		link->clear();
		GAME_STATE = EDDITOR;
	}

	y = 400;
	stretchRect.h = h;
	stretchRect.w = w;
	stretchRect.x = (SCREEN_WIDTH - stretchRect.w) / 2;
	stretchRect.y = y - stretchRect.h / 2;
	if (drawButton(stretchRect, "Load"))
	{
		next = EDDITOR;
		GAME_STATE = LOAD;
	}

	return 0;
}

int draw::load()
{
	link->clear();
	string title = "Name of roulet";
	SDL_Surface* surfaceText = TTF_RenderText_Solid(Sans, title.c_str(), white);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
	SDL_FreeSurface(surfaceText);

	SDL_Rect stretchRect;
	stretchRect.x = SCREEN_WIDTH / 2 - 100;
	stretchRect.y = 10;
	stretchRect.w = 200;
	stretchRect.h = 50;
	SDL_RenderCopy(gRenderer, Message, NULL, &stretchRect);
	SDL_DestroyTexture(Message);



	stretchRect.h = 50;
	stretchRect.w = 250;
	stretchRect.x = (SCREEN_WIDTH - stretchRect.w) / 2;
	stretchRect.y = 120;
	drawTextBlock(1, stretchRect, text, Sans, &text);


	stretchRect.h = 50;
	stretchRect.w = 200;
	stretchRect.x = (SCREEN_WIDTH - stretchRect.w) / 2;
	stretchRect.y = SCREEN_HEIGHT - stretchRect.h - 10;
	if (drawButton(stretchRect, "Load"))
	{
		saveHandler load(link);
		if (!load.load(text))
		{
			veld = -1;
			text.clear();
			GAME_STATE = next;
		}
	}

	return 0;
}

int draw::play()
{
	card* temp;

	temp = link->get(id);
	if (temp == nullptr)
	{
		return 1;
	}

	SDL_RenderCopy(gRenderer, cardTexture, NULL, NULL);

	int time;
	if (!temp->time.empty())
	{
		time = stoi(temp->time);
	}
	else
	{
		time = 0;
	}

	int bpm;
	if (!temp->bpm.empty())
	{
		bpm = stoi(temp->bpm);
	}
	else
	{
		bpm = 0;
	}

	SDL_Rect stretchRect;

	if (time==0 || cheats || (int(SDL_GetTicks64()) - startTime) > time * 60000 && startTime > 0)
	{
		card* temp2 = NULL;
		stretchRect.h = 70;
		stretchRect.w = 200;
		stretchRect.x = SCREEN_WIDTH - stretchRect.w - 10;
		stretchRect.y = SCREEN_HEIGHT - stretchRect.h - 10;
		if (drawButton(stretchRect, "Next"))
		{
			if(!temp->valeus[0].empty())values[0] = values[0] + stoi(temp->valeus[0]);
			if (!temp->valeus[2].empty())values[1] = values[1] + stoi(temp->valeus[2]);
			if (!temp->valeus[4].empty())values[2] = values[2] + stoi(temp->valeus[4]);
			if (!temp->tags[0].empty())
			{
				if (temp->tags[0].at(0) != *"-")
				{
					tags->insertNode(temp->tags[0]);
				}
				else
				{
					tags->deleteNode(temp->tags[0].substr(1,temp->tags[0].size()));
				}
			}
			if (!temp->tags[2].empty())
			{
				if (temp->tags[2].at(0) != *"-")
				{
					tags->insertNode(temp->tags[2]);
				}
				else
				{
					tags->deleteNode(temp->tags[2].substr(1, temp->tags[2].size()));
				}
			}
			if (temp->tags[4].length()>0)
			{
				if (temp->tags[4].at(0) != *"-")
				{
					tags->insertNode(temp->tags[2]);
				}
				else
				{
					tags->deleteNode(temp->tags[4].substr(1, temp->tags[4].size()));
				}
			}
			startTime = -1;
			this->rands[0] = rand();
			this->rands[1] = rand();
			this->rands[2] = rand();
			int conC = 0;
			int opt[10] = { 0,0,0,0,0,0,0,0,0,0 };
			int val0 = 0;
			int val1 = 0;
			int val2 = 0;
			for (int i = 0; i < 10; i++)
			{
				if (!temp->conection[i].empty())
				{
					temp2 = link->get(stoi(temp->conection[i]));
					val0 = 0;
					if (!temp2->valeus[1].empty()) { val0 = stoi(temp2->valeus[1]); }
					val1 = 0;
					if (!temp2->valeus[3].empty()) { val1 = stoi(temp2->valeus[3]); }
					val2 = 0;
					if (!temp2->valeus[5].empty()) { val2 = stoi(temp2->valeus[5]); }
					if (val0<=values[0] && val1 <= values[1] && val2 <= values[2] && tags->hasTag(temp2->tags[1]) && tags->hasTag(temp2->tags[3]) && tags->hasTag(temp2->tags[5]))
					{
						opt[conC] = stoi(temp->conection[i]);
						conC++;
					}
				}
				
			}
			if (conC != 0 && temp->random || conC == 1)
			{
				id = opt[rand() % conC];
			}
			else if(conC == 0)
			{
				id = temp->id + 1;
			}
			else
			{
				GAME_STATE = PLAYSELECT;
			}
		}
	}

	if (startTime <= 0 && time>0)
	{
		stretchRect.h = 70;
		stretchRect.w = 200;
		stretchRect.x = SCREEN_WIDTH/2 - stretchRect.w/2;
		stretchRect.y = SCREEN_HEIGHT - stretchRect.h - 10;
		if (drawButton(stretchRect, "Start"))
		{
			startTime = int(SDL_GetTicks64());
		}
	}

	if (!temp->bpm.empty() && ((int(SDL_GetTicks64()) - startTime) < time * 60000 && startTime > 0 || time==0))
	{
		int tick = int(SDL_GetTicks64());
		int p = 30000 / bpm;
		stretchRect.w = 50;
		stretchRect.h = 25;
		stretchRect.x = int(SCREEN_WIDTH - 100 - abs(float(tick % p )/ p - 0.5) * 50);
		stretchRect.y = SCREEN_HEIGHT - stretchRect.h / 2 - 100;
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(gRenderer, &stretchRect);

		stretchRect.w = 50;
		stretchRect.h = 50;
		stretchRect.x = SCREEN_WIDTH - 150;
		stretchRect.y = SCREEN_HEIGHT - stretchRect.w / 2 - 100;
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderFillRect(gRenderer, &stretchRect);
	}

	if (time>0&&(int(SDL_GetTicks64()) - startTime) < time * 60000 && startTime > 0)
	{
		stretchRect.w = SCREEN_WIDTH - 50;
		stretchRect.h = 50;
		stretchRect.x = 10;
		stretchRect.y = SCREEN_HEIGHT - 60;
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(gRenderer, &stretchRect);

		stretchRect.w = int(float(int(SDL_GetTicks64()) - startTime)/time*60000*stretchRect.w);
		stretchRect.h = 50;
		stretchRect.x = 10;
		stretchRect.y = SCREEN_HEIGHT - 60;
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(gRenderer, &stretchRect);
	}

	if (update)
	{
		drawCard();
		update = false;
	}

	return 0;
}

int draw::drawTextBlock(int veltI, SDL_Rect stretchRect, string defo, TTF_Font* font, string* pString)
{
	SDL_Surface* surfaceText;
	SDL_Texture* Message;
	int lineX = 0;
	int lineY = 0;
	if (veld == veltI)
	{
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_SetTextInputRect(&stretchRect);
		SDL_StartTextInput();
		inputText = pString;
		/*surfaceText = TTF_RenderText_Blended_Wrapped(font, defo.substr(0, offset).c_str(), white, stretchRect.w);
		Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
		SDL_FreeSurface(surfaceText);
		SDL_QueryTexture(Message, NULL, NULL, &lineX, &lineY);
		SDL_RenderDrawLine(gRenderer, lineX + stretchRect.x, lineY + stretchRect.y, lineX + stretchRect.x, lineY + stretchRect.y - 20);*/
	}
	else
	{
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
	}
	SDL_RenderDrawRect(gRenderer, &stretchRect);

	if (xMouse > (stretchRect.x) && xMouse < (stretchRect.x + stretchRect.w) && yMouse > stretchRect.y && yMouse < stretchRect.y + stretchRect.h && mousePres)
	{
		veld = veltI;
	}
	surfaceText = TTF_RenderText_Blended_Wrapped(font, defo.c_str(), white, stretchRect.w);
	Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
	SDL_FreeSurface(surfaceText);
	SDL_QueryTexture(Message, NULL, NULL, &stretchRect.w, &stretchRect.h);
	SDL_RenderCopy(gRenderer, Message, NULL, &stretchRect);
	SDL_DestroyTexture(Message);
	return 0;
}

int draw::drawCard()
{
	//The final texture
	card* temp = link->get(id);;
	int w=0;
	int h=0;
	int x=0;
	int y=0;
	int type = 0;
	if (temp == nullptr)
	{
		return 1;
	}
	type = stoi(temp->type);
	SDL_SetRenderTarget(gRenderer, cardTexture);

	SDL_SetRenderDrawColor(gRenderer, 0xF7, 0xC0, 0xDC, 0xFF);
	SDL_RenderClear(gRenderer);
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

	string title = temp->title;
	SDL_Surface* surfaceText = TTF_RenderText_Solid(SansB, title.c_str(), white);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
	SDL_FreeSurface(surfaceText);

	SDL_Rect stretchRect;
	SDL_Texture* png = NULL;
	string textR = temp->text;

	while (textR.find(" x ") != string::npos)
	{
		textR.replace(textR.find(" x "), 3, " " + to_string(rands[0] % (stoi(temp->ra[1]) - stoi(temp->ra[0])) + stoi(temp->ra[0])) + " ");
	}
	while (textR.find(" y ") != string::npos)
	{
		textR.replace(textR.find(" y "), 3, " " + to_string(rands[1] % (stoi(temp->ra[3]) - stoi(temp->ra[2])) + stoi(temp->ra[2])) + " ");
	}
	while (textR.find(" z ") != string::npos)
	{
		textR.replace(textR.find(" z "), 3, " " + to_string(rands[2] % (stoi(temp->ra[5]) - stoi(temp->ra[4])) + stoi(temp->ra[4])) + " ");
	}



	SDL_QueryTexture(Message, NULL, NULL, &stretchRect.w, &stretchRect.h);
	switch (type)
	{
	case 1:
		stretchRect.x = SCREEN_WIDTH / 8 * 5;
		stretchRect.y = 10;
		break;
	default:
		stretchRect.x = (SCREEN_WIDTH - stretchRect.w) / 2;
		stretchRect.y = 10;
		break;
	}
	SDL_RenderCopy(gRenderer, Message, NULL, &stretchRect);
	SDL_DestroyTexture(Message);


	png = loadTexture("textures/" + temp->img + ".png");
	if (png == NULL)
	{
		png = loadTexture("textures/" + temp->img + ".jpg");
	}
	if (png == NULL)
	{
		png = loadTexture("textures/" + temp->img + ".jpeg");
	}
	if (!png == NULL)
	{
		SDL_QueryTexture(png, NULL, NULL, &stretchRect.w, &stretchRect.h);
		switch (type)
		{
		case 1:
			w = SCREEN_WIDTH / 8 * 5-20;
			h = SCREEN_HEIGHT -20;
			break;
		default:
			w = SCREEN_WIDTH / 8 * 4;
			h = SCREEN_HEIGHT / 8 * 4;
			break;
		}
		if (stretchRect.w > w)
		{
			stretchRect.h = int(w / float(stretchRect.w) * stretchRect.h);
			stretchRect.w = w;
		}
		if (stretchRect.h > h)
		{
			stretchRect.w = int(h / float(stretchRect.h) * stretchRect.w);
			stretchRect.h =h;
		}

		switch (type)
		{
		case 1:
			x = 10;
			y = 10;
			break;
		default:
			x = (SCREEN_WIDTH - stretchRect.w) / 2;
			y = SCREEN_HEIGHT / 8;
			break;
		}
		stretchRect.x = x;
		stretchRect.y = y;
		SDL_RenderCopy(gRenderer, png, NULL, &stretchRect);
		SDL_DestroyTexture(png);
	}
	/*stretchRect.w = w;
	stretchRect.h = h;
	stretchRect.x = x;
	stretchRect.y = y;
	SDL_RenderDrawRect(gRenderer,&stretchRect);*/

	SDL_Color* collor=&white;
	switch (type)
	{
	case 1:
		x = SCREEN_WIDTH / 8 * 5;;
		y = 100;
		w = SCREEN_WIDTH / 8 * 3;
		//stretchRect.h = 0;
		break;
	default:
		x = 10;
		y = SCREEN_HEIGHT / 8 * 5;
		w = SCREEN_WIDTH - 50;
		//stretchRect.h = 0;
		break;
	}

	while(textR.find("\\n") != string::npos || textR.find("\\g") != string::npos || textR.find("\\w") != string::npos || textR.find("\\r") != string::npos)
	{
		if (textR.find("\\n") < textR.find("\\g") && textR.find("\\n") < textR.find("\\w") && textR.find("\\n") < textR.find("\\"))
		{
			surfaceText = TTF_RenderText_Blended_Wrapped(SansS, textR.substr(0, textR.find("\\n")).c_str(), *collor, w);
			Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
			SDL_FreeSurface(surfaceText);
			SDL_QueryTexture(Message, NULL, NULL, &stretchRect.w, &stretchRect.h);
			stretchRect.x = x;
			stretchRect.y = y;
			y = y + stretchRect.h;
			SDL_RenderCopy(gRenderer, Message, NULL, &stretchRect);
			SDL_DestroyTexture(Message);
			textR = textR.substr(textR.find("\\n") + 2, textR.length());
		}
		else
		{
			surfaceText = TTF_RenderText_Blended_Wrapped(SansS, textR.substr(0, textR.find("\\")).c_str(), *collor, w);
			Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
			SDL_FreeSurface(surfaceText);
			SDL_QueryTexture(Message, NULL, NULL, &stretchRect.w, &stretchRect.h);
			stretchRect.x = x ;
			stretchRect.y = y;
			y = y + stretchRect.h;
			SDL_RenderCopy(gRenderer, Message, NULL, &stretchRect);
			SDL_DestroyTexture(Message);
			//cout << textR.at(textR.find("\\") + 1);
			switch (textR.at(textR.find("\\")+1))
			{
			case *"g":
				collor = &green;
				break;
			case *"r":
				collor = &red;
				break;
			default:
				collor = &white;
				break;
			}
			textR = textR.substr(textR.find("\\")+2, textR.length());
		}
	}

	surfaceText = TTF_RenderText_Blended_Wrapped(SansS, textR.c_str(), *collor, w);
	Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
	SDL_FreeSurface(surfaceText);
	SDL_QueryTexture(Message, NULL, NULL, &stretchRect.w, &stretchRect.h);
	stretchRect.x = x;
	stretchRect.y = y;
	SDL_RenderCopy(gRenderer, Message, NULL, &stretchRect);
	SDL_DestroyTexture(Message);

	SDL_SetRenderTarget(gRenderer, nullptr);

	return 0;
}

int draw::drawButton(SDL_Rect stretchRect, string name)
{
	SDL_Surface* surfaceText = TTF_RenderText_Solid(Sans, name.c_str(), white);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
	SDL_FreeSurface(surfaceText);
	//SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	int h = stretchRect.h;
	int w = stretchRect.w;
	if (xMouse > (stretchRect.x) && xMouse < (stretchRect.x + stretchRect.w) && yMouse > stretchRect.y && yMouse < stretchRect.y + stretchRect.h)
	{
		if (mousePres)
		{
			return 1;
		}
		stretchRect.x = stretchRect.x + int(stretchRect.w * 0.1) / 2;
		stretchRect.y = stretchRect.y + int(stretchRect.h * 0.1) / 2;
		stretchRect.h = int(stretchRect.h * 0.9);
		stretchRect.w = int(stretchRect.w * 0.9);
		SDL_RenderCopy(gRenderer, gTextureButtonPres, NULL, &stretchRect);
		//SDL_RenderDrawRect(gRenderer, &stretchRect);
	}
	else
	{
		SDL_RenderCopy(gRenderer, gTextureButton, NULL, &stretchRect);
		//SDL_RenderDrawRect(gRenderer, &stretchRect);
	}
	SDL_QueryTexture(Message, NULL, NULL, &w, &h);
	stretchRect.x = stretchRect.x + stretchRect.w / 2;
	stretchRect.y = stretchRect.y + stretchRect.h / 2;
	stretchRect.w = w * stretchRect.h / h;
	stretchRect.x = stretchRect.x - int(stretchRect.w ) / 2;
	stretchRect.y = stretchRect.y - int(stretchRect.h ) / 2;
	SDL_RenderCopy(gRenderer, Message, NULL, &stretchRect);
	//SDL_RenderDrawRect(gRenderer, &stretchRect);
	SDL_DestroyTexture(Message);
	return 0;
}

int draw::playSelect()
{
	SDL_Rect stretchRect;
	int conC = 0;
	int opt[10] = { 0,0,0,0,0,0,0,0,0,0 };
	card* temp = link->get(id);
	card* temp2 = NULL;
	int val0 = 0;
	int val1 = 0;
	int val2 = 0;
	for (int i = 0; i < 10; i++)
	{
		if (!temp->conection[i].empty())
		{
			temp2 = link->get(stoi(temp->conection[i]));
			val0 = 0;
			if (!temp2->valeus[1].empty()) { val0 = stoi(temp2->valeus[1]); }
			val1 = 0;
			if (!temp2->valeus[3].empty()) { val1 = stoi(temp2->valeus[3]); }
			val2 = 0;
			if (!temp2->valeus[5].empty()) { val2 = stoi(temp2->valeus[5]); }
			if (val0 <= values[0] && val1 <= values[1] && val2 <= values[2] && tags->hasTag(temp2->tags[1]) && tags->hasTag(temp2->tags[3]) && tags->hasTag(temp2->tags[5]))
			{
				opt[conC] = stoi(temp->conection[i]);
				conC++;
			}
		}

	}
	if (conC == 2)
	{
		stretchRect.x = 10;
		stretchRect.y = 10;
		stretchRect.w = SCREEN_WIDTH/2-30;
		stretchRect.h = SCREEN_HEIGHT-20;
		cardPreview(opt[0]);
		SDL_RenderCopy(gRenderer, cardPrevTexture, NULL, &stretchRect);
		if (xMouse > (stretchRect.x) && xMouse < (stretchRect.x + stretchRect.w) && yMouse > stretchRect.y && yMouse < stretchRect.y + stretchRect.h && mousePres)
		{
			GAME_STATE = PLAY;
			id = opt[0];
		}
		stretchRect.x = SCREEN_WIDTH / 2 + 20;
		stretchRect.w = SCREEN_WIDTH / 2 - 30;
		cardPreview(opt[1]);
		SDL_RenderCopy(gRenderer, cardPrevTexture, NULL, &stretchRect);
		if (xMouse > (stretchRect.x) && xMouse < (stretchRect.x + stretchRect.w) && yMouse > stretchRect.y && yMouse < stretchRect.y + stretchRect.h && mousePres)
		{
			GAME_STATE = PLAY;
			id = opt[1];
		}
	}
	else if(conC<9)
	{
		for (int i = 0; i < (conC); i++)
		{
			stretchRect.w = SCREEN_WIDTH / 4 - 50;
			stretchRect.h = SCREEN_HEIGHT / 2 - 30;
			stretchRect.x = 10+(10 + stretchRect.w)*(i%4);
			stretchRect.y = 10+(10 + stretchRect.h)*(i/4);
			cardPreview(opt[i]);
			SDL_RenderCopy(gRenderer, cardPrevTexture, NULL, &stretchRect);
			if (xMouse > (stretchRect.x) && xMouse < (stretchRect.x + stretchRect.w) && yMouse > stretchRect.y && yMouse < stretchRect.y + stretchRect.h && mousePres)
			{
				GAME_STATE = PLAY;
				id = opt[i];
			}
		}
	}
	else
	{
		for (int i = 0; i < (conC); i++)
		{
			stretchRect.w = SCREEN_WIDTH / 5 - 60;
			stretchRect.h = SCREEN_HEIGHT / 5 - 40;
			stretchRect.x = 10 + (10 + stretchRect.w) * (i % 5);
			stretchRect.y = 10 + (10 + stretchRect.h) * (i / 5);
			cardPreview(opt[i]);
			SDL_RenderCopy(gRenderer, cardPrevTexture, NULL, &stretchRect);
			if (xMouse > (stretchRect.x) && xMouse < (stretchRect.x + stretchRect.w) && yMouse > stretchRect.y && yMouse < stretchRect.y + stretchRect.h && mousePres)
			{
				GAME_STATE = PLAY;
				id = opt[i];
			}
		}
	}
	return 0;
}

int draw::cardPreview(int prevId)
{
	//The final texture
	card* temp = link->get(prevId);;

	if (temp == nullptr)
	{
		return 1;
	}

	SDL_SetRenderTarget(gRenderer, cardPrevTexture);

	SDL_SetRenderDrawColor(gRenderer, 0xF7, 0xC0, 0xDC, 0xFF);
	SDL_RenderClear(gRenderer);
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

	string title = temp->title;
	SDL_Surface* surfaceText = TTF_RenderText_Solid(SansB, title.c_str(), white);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
	SDL_FreeSurface(surfaceText);

	SDL_Rect stretchRect;
	SDL_Texture* png = NULL;
	stretchRect.w = SCREEN_WIDTH / 2;
	stretchRect.h = SCREEN_HEIGHT;
	stretchRect.x = 0;
	stretchRect.y = 0;
	SDL_RenderDrawRect(gRenderer, &stretchRect);

	SDL_QueryTexture(Message, NULL, NULL, &stretchRect.w, &stretchRect.h);
	stretchRect.x = (SCREEN_WIDTH/2 - stretchRect.w) / 2;
	stretchRect.y = 10;
	SDL_RenderCopy(gRenderer, Message, NULL, &stretchRect);
	SDL_DestroyTexture(Message);




	png = loadTexture("textures/" + temp->img + ".png");
	if (png == NULL)
	{
		png = loadTexture("textures/" + temp->img + ".jpg");
	}
	if (png == NULL)
	{
		png = loadTexture("textures/" + temp->img + ".jpeg");
	}
	if (!png == NULL)
	{
		SDL_QueryTexture(png, NULL, NULL, &stretchRect.w, &stretchRect.h);
		if (stretchRect.w > SCREEN_WIDTH / 8 * 6)
		{
			stretchRect.h = int((SCREEN_WIDTH / 8 * 6) / float(stretchRect.w) * stretchRect.h);
			stretchRect.w = (SCREEN_WIDTH / 8 * 6);
		}
		if (stretchRect.h > SCREEN_HEIGHT / 8 * 6)
		{
			stretchRect.w = int((SCREEN_HEIGHT / 2) / float(stretchRect.h) * stretchRect.w);
			stretchRect.h = (SCREEN_HEIGHT / 2);
		}
		stretchRect.x = (SCREEN_WIDTH/2 - stretchRect.w) / 2;
		stretchRect.y = SCREEN_HEIGHT / 8;
		SDL_RenderCopy(gRenderer, png, NULL, &stretchRect);
		SDL_DestroyTexture(png);
	}

	SDL_SetRenderTarget(gRenderer, nullptr);

	return 0;
}

int draw::settings()
{
	int y = 0;
	int w = 500;
	int h = 90;

	SDL_Rect stretchRect;
	y = 300;
	stretchRect.h = h;
	stretchRect.w = w;
	stretchRect.x = (SCREEN_WIDTH - stretchRect.w) / 2;
	stretchRect.y = y - stretchRect.h / 2;
	string cheat = cheats ? "true" : "false";
	if (drawButton(stretchRect, ("cheat:"+ cheat)))
	{
		cheats = !cheats;
	}

	y = 400;
	stretchRect.h = h;
	stretchRect.w = w;
	stretchRect.x = (SCREEN_WIDTH - stretchRect.w) / 2;
	stretchRect.y = y - stretchRect.h / 2;
	if (drawButton(stretchRect, "..."))
	{
	}

	y = 500;
	stretchRect.h = h;
	stretchRect.w = w;
	stretchRect.x = (SCREEN_WIDTH - stretchRect.w) / 2;
	stretchRect.y = y - stretchRect.h / 2;
	if (drawButton(stretchRect, "..."))
	{
	}

	y = 600;
	stretchRect.h = h;
	stretchRect.w = w;
	stretchRect.x = (SCREEN_WIDTH - stretchRect.w) / 2;
	stretchRect.y = y - stretchRect.h / 2;
	if (drawButton(stretchRect, "Back"))
	{
		GAME_STATE = MAIN;
	}
	return 0;
}
