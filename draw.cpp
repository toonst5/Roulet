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
			Sans = TTF_OpenFont("gameFiles/font/Sans.ttf", 30);
			SansS = TTF_OpenFont("gameFiles/font/Sans.ttf", 15);
			SansB = TTF_OpenFont("gameFiles/font/Sans.ttf", 45);
			white = { 255, 255, 255 };
			red = { 255, 0, 0 };
			green = { 0, 255, 0 };
			blau = { 0,191,255 };
			purple = { 138,43,226 };
			link = new cardLinked();
			tags = new linkedMod();
			saveHandl = new saveHandler(link);
			veld = -1;
			cardTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
			for (int i = 0; i < 10; i++)
			{
				selectTexture[i] = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH / 2, SCREEN_HEIGHT);
			}
			values[0] = 0;
			values[1] = 0;
			values[2] = 0;
			cheats = false;
			offset=0;
			string* lastTextI=NULL;

			//Mix_PlayMusic(gMusic, -1);

			srand((unsigned)time(NULL));
			this->rands[0] = rand();
			this->rands[1] = rand();
			this->rands[2] = rand();
			if (Sans == NULL)
			{
				printf("%s/n", TTF_GetError());
				return 1;
			}

			bool quit = false;

			//Event handler
			SDL_Event e;
			SDL_EventState(SDL_DROPFILE,SDL_ENABLE);
			//While application is running
			while (!quit)
			{
				mousePres = false;
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					switch (e.type)
					{
					case SDL_QUIT:
						quit = true;
						break;
					case SDL_MOUSEMOTION:
						SDL_GetMouseState(&xMouse, &yMouse);
						break;
					case SDL_MOUSEBUTTONDOWN:
						update = true;
						mousePres = true;
						break;
					case SDL_KEYDOWN:
						update = true;
						//Handle backspace
						if (e.key.keysym.sym == SDLK_BACKSPACE && inputText->length() > 0 && offset > 0)
						{
							//inputText->pop_back();
							inputText->erase(offset - 1, 1);
							offset--;
						}
						if (e.key.keysym.sym == SDLK_DELETE && inputText->length() > 0 && offset < inputText->length())
						{
							//inputText->pop_back();
							inputText->erase(offset, 1);
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
						break;
					case SDL_TEXTINPUT:
						update = true;
						//Not copy or pasting
						if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
						{
							//Append character
							inputText->insert(offset, e.text.text);
							//*inputText += e.text.text;
							offset++;
						}
						break;
					case SDL_DROPFILE:
						if (GAME_STATE == EDDITOR)
						{
							update = true;
							link->get(id)->img = saveHandl->fileImport(string(e.drop.file));
						}
						SDL_free(e.drop.file);
						break;
					default:
						break;
					}
				}
				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xF7, 0xC0, 0xDC, 0xFF);
				//SDL_SetRenderDrawColor(gRenderer, 0xF7, 0xC0, 0x00, 0xFF);
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
				case CARDSELECT:
					cardSelect();
					break;
				case SETTINGS:
					settings();
					break;
				case CONTINU:
					continu();
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
						offset = int(inputText->length());
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
					offset = int(inputText->length());
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


	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

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
	gTextureButton = loadTexture("gameFiles/textures/button.png");
	if (gTextureButton == NULL)
	{
		printf("Failed to load texture image1!\n");
		success = false;
	}

	gTextureButtonPres = loadTexture("gameFiles/textures/button_pressed.png");
	if (gTextureButtonPres == NULL)
	{
		printf("Failed to load texture image2!\n");
		success = false;
	}

	buttTexture = loadTexture("gameFiles/textures/butt.png");
	if (buttTexture == NULL)
	{
		printf("Failed to load texture image1!\n");
		success = false;
	}

	penisTexture = loadTexture("gameFiles/textures/penis.png");
	if (penisTexture == NULL)
	{
		printf("Failed to load texture image1!\n");
		success = false;
	}



	gSlap = Mix_LoadWAV("gameFiles/audio/wet_slap.wav");
	if (gSlap == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	gMusic = Mix_LoadMUS("gameFiles/audio/Forest.wav");
	if (gMusic == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
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
	SDL_DestroyTexture(buttTexture);
	SDL_DestroyTexture(penisTexture);
	SDL_DestroyTexture(imgMain);
	for (int i = 0; i < 10; i++)
	{
		SDL_DestroyTexture(selectTexture[i]);
		selectTexture[i] = NULL;
	}
	gTextureButton = NULL;
	gTextureButtonPres = NULL;
	cardTexture = NULL;
	imgMain = NULL;

	//Free the sound effects
    Mix_FreeChunk( gSlap );
    gSlap = NULL;
    
    //Free the music
    Mix_FreeMusic( gMusic );
    gMusic = NULL;


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
		values[0] = 0;
		values[1] = 0;
		values[2] = 0;
		tags->clear();
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
		link->appandNode();
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

	stretchRect.h = h;
	stretchRect.w = w;
	stretchRect.y = y;
	stretchRect.x = 1000;
	if (temp->next == nullptr)
	{
		if (drawButton(stretchRect, "new"))
		{
			id++;
			veld = -1;
			link->appandNode();
			temp = temp->next;
		}
	}
	else
	{
		if (drawButton(stretchRect, "next"))
		{
			id++;
			veld = -1;
			temp = temp->next;
		}
	}
	

	if (id != 0)
	{
		stretchRect.x = 10;
		if (drawButton(stretchRect, "Back"))
		{
			id--;
			veld = -1;
		}
	}

	stretchRect.x = 200;
	if (drawButton(stretchRect, "Delete"))
	{
		link->deleteNode(id);
		return 0;
	}

	stretchRect.x = 400;
	if (drawButton(stretchRect, "Save"))
	{
		saveHandl->save(saveName);
		//GAME_STATE = EDDITORSAVE;
	}



	stretchRect.x = 600;
	if (drawButton(stretchRect, "Select"))
	{
		updatePreview = true;
		next = EDDITOR;
		GAME_STATE = CARDSELECT;
	}

	stretchRect.x = 800;
	if (drawButton(stretchRect, "Insert"))
	{
		link->insertNode(id);
	}

	stretchRect.h = 50;
	stretchRect.w = 50;
	stretchRect.x = SCREEN_WIDTH - 60;
	stretchRect.y = 10;
	if (drawButton(stretchRect, "X"))
	{
		saveHandl->save(saveName);
		GAME_STATE = MAIN;
	}




	stretchRect.x = 10;
	stretchRect.y = 10;
	stretchRect.h = 50;
	stretchRect.w = 300;
	drawTextBlock(1, stretchRect, "title card", Sans, &temp->title);


	stretchRect.x = 10;
	stretchRect.y = 130;
	stretchRect.h = 500;
	stretchRect.w = 300;
	drawTextBlock(2, stretchRect, "text for the card with x y z as random stats", SansS, &temp->text);

	stretchRect.x = 10;
	stretchRect.y = 70;
	stretchRect.h = 50;
	stretchRect.w = 300;
	drawTextBlock(3, stretchRect, "image name", Sans, &temp->img);

	for (int i = 0; i < 10; i++)
	{
		stretchRect.x = 330 + i/5*60;
		stretchRect.y = 10 + i%5*60;
		stretchRect.h = 50;
		stretchRect.w = 50;
		drawTextBlock(4 + i, stretchRect, "id", Sans, &temp->conection[i]);
		
	}

	for (int i = 0; i < 6; i++)
	{
		stretchRect.x = 315 + i % 2 * 70;
		stretchRect.y = 320 + i / 2 * 60;
		stretchRect.h = 50;
		stretchRect.w = 60;
		if (i % 2)
		{
			drawTextBlock(14 + i, stretchRect, "max", Sans, &temp->ra[i]);
		}else
		{
			drawTextBlock(14 + i, stretchRect, "min",Sans, &temp->ra[i]);
		}
	}

	stretchRect.x = 330;
	stretchRect.y = 510;
	stretchRect.h = 50;
	stretchRect.w = 100;
	drawTextBlock(20, stretchRect, "bpm", Sans, &temp->bpm);

	stretchRect.x = 330;
	stretchRect.y = 570;
	stretchRect.h = 50;
	stretchRect.w = 100;
	drawTextBlock(21, stretchRect, "time", Sans, &temp->time);

	for (int i = 0; i < 6; i++)
	{
		stretchRect.x = SCREEN_WIDTH - 700 + i / 2 * 100;
		stretchRect.y = 70 + i % 2 * 60;
		stretchRect.h = 50;
		stretchRect.w = 80;
		if (i % 2)
		{
			drawTextBlock(22 + i, stretchRect, ("need "+to_string(i/2+1)).c_str(), Sans, &temp->valeus[i]);
		}
		else
		{
			drawTextBlock(22 + i, stretchRect, ("give " + to_string(i / 2+1)).c_str(), Sans, &temp->valeus[i]);
		}
	}

	for (int i = 0; i < 6; i++)
	{
		stretchRect.x = SCREEN_WIDTH - 390 + i/2*130;
		stretchRect.y = 70 + i%2 * 60;
		stretchRect.h = 50;
		stretchRect.w = 120;
		if (i % 2)
		{
			drawTextBlock(28 + i, stretchRect, "need tag", Sans, &temp->tags[i]);
		}
		else
		{
			drawTextBlock(28 + i, stretchRect, "give tag", Sans, &temp->tags[i]);
		}
	}

	stretchRect.x = 10;
	stretchRect.y = 640;
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
	stretchRect.y = 640;
	stretchRect.h = 50;
	stretchRect.w = 100;
	string type= temp->type;
	if (drawButton(stretchRect, "type:"+ type))
	{
		temp->type =to_string((stoi(temp->type)+1)%2);
	}

	stretchRect.y = 10;
	stretchRect.h = 50;
	stretchRect.w = 250;
	stretchRect.x = SCREEN_WIDTH - stretchRect.w - 70;
	drawTextBlock(34, stretchRect, "title", Sans, &saveName);

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
		saveHandl->save(text);
		veld = -1;
		text.clear();
			
		GAME_STATE = EDDITOR;
	}


	return 0;
}

int draw::edditorMenu()
{
	int y=0;
	const int w = 300;
	const int h = 90;
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
		saveName = "new";
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
		if (!saveHandl->load(text))
		{
			veld = -1;
			saveName = text;
			if (filesystem::exists(text+"/save.txt") && next == PLAY)
			{
				GAME_STATE = CONTINU;
			}
			else
			{
				GAME_STATE = next;
			}
			text.clear();
		}
	}

	stretchRect.h = 50;
	stretchRect.w = 50;
	stretchRect.x = SCREEN_WIDTH - 60;
	stretchRect.y = 10;
	if (drawButton(stretchRect, "X"))
	{
		GAME_STATE = MAIN;
	}


	return 0;
}

int draw::play()
{
	SDL_Surface* surfaceText = NULL;
	SDL_Texture* Message = NULL;
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

	stretchRect.h = 50;
	stretchRect.w = 50;
	stretchRect.x = SCREEN_WIDTH - 60;
	stretchRect.y = 10;
	if (drawButton(stretchRect, "|||"))
	{
		showStats = !showStats;
	}
	if (showStats)
	{
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
		stretchRect.h = SCREEN_HEIGHT-200;
		stretchRect.w = 200;
		stretchRect.x = SCREEN_WIDTH - stretchRect.w - 10;
		stretchRect.y = 70;
		SDL_RenderFillRect(gRenderer,&stretchRect);
		stretchRect.y = 70;
		for (int i = 0; i < 3; i++)
		{
			surfaceText = TTF_RenderText_Solid(SansS, to_string(values[i]).c_str(), white);
			Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
			SDL_FreeSurface(surfaceText);
			SDL_QueryTexture(Message, NULL, NULL, &stretchRect.w, &stretchRect.h);
			stretchRect.x = SCREEN_WIDTH- stretchRect.w-10;
			stretchRect.y += stretchRect.h+10;
			SDL_RenderCopy(gRenderer, Message, NULL, &stretchRect);
			SDL_DestroyTexture(Message);
		}
		for (int i = 0; tags->get(i)!=nullptr; i++)
		{
			surfaceText = TTF_RenderText_Solid(SansS, tags->get(i)->data.c_str(), white);
			Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
			SDL_FreeSurface(surfaceText);
			SDL_QueryTexture(Message, NULL, NULL, &stretchRect.w, &stretchRect.h);
			stretchRect.x = SCREEN_WIDTH - stretchRect.w - 10;
			stretchRect.y += stretchRect.h + 10;
			SDL_RenderCopy(gRenderer, Message, NULL, &stretchRect);
			SDL_DestroyTexture(Message);
		}
		stretchRect.h = 50;
		stretchRect.w = 50;
		stretchRect.x = SCREEN_WIDTH - 60;
		stretchRect.y += stretchRect.h + 10;
		if (drawButton(stretchRect, "X"))
		{
			GAME_STATE = MAIN;
		}
	}

	if (time==0 || cheats || (int(SDL_GetTicks64()) - startTime) > time * 60000 && startTime > 0)
	{
		card* temp2 = NULL;
		stretchRect.h = 70;
		stretchRect.w = 200;
		stretchRect.x = SCREEN_WIDTH - stretchRect.w - 10;
		stretchRect.y = SCREEN_HEIGHT - stretchRect.h - 10;
		if (drawButton(stretchRect, "Next"))
		{
			saveHandl->stateSave(id,values,tags,saveName);
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
				updatePreview = true;
				next = PLAY;
				GAME_STATE = CARDSELECT;
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
		int p = 60000 / bpm;
		SDL_QueryTexture(penisTexture, NULL, NULL, &stretchRect.w, &stretchRect.h);
		stretchRect.h = int(stretchRect.h * 0.4);
		stretchRect.w = int(stretchRect.w * 0.4);
		stretchRect.x = int(SCREEN_WIDTH - 80- abs(float(tick % p )/ p - 0.5) * 330);
		if (tick % p >= 0 && tick % p <= 5)
		{
			Mix_PlayChannel(-1, gSlap, 0);
		}
		stretchRect.y = SCREEN_HEIGHT - stretchRect.h / 2 - 150;
		SDL_RenderCopy(gRenderer, penisTexture, NULL,&stretchRect);

		SDL_QueryTexture(buttTexture, NULL, NULL, &stretchRect.w, &stretchRect.h);
		stretchRect.h = int(stretchRect.h * 0.6);
		stretchRect.w = int(stretchRect.w * 0.6);
		stretchRect.x = SCREEN_WIDTH - 250;
		stretchRect.y = SCREEN_HEIGHT - stretchRect.w / 2 - 150;
		SDL_RenderCopy(gRenderer, buttTexture, NULL,&stretchRect);
	}

	if (time>0&&(int(SDL_GetTicks64()) - startTime) < time * 60000 && startTime > 0)
	{
		stretchRect.w = SCREEN_WIDTH - 50;
		stretchRect.h = 20;
		stretchRect.x = 10;
		stretchRect.y = SCREEN_HEIGHT - 60;
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(gRenderer, &stretchRect);

		cout <<(int(SDL_GetTicks64()) - startTime) * stretchRect.w / (time * 60000)<<"\n";
		stretchRect.w = (int(SDL_GetTicks64()) - startTime)*stretchRect.w/(time*60000);
		stretchRect.h = 20;
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
	int lineX = stretchRect.x;
	int lineY = stretchRect.y;
	int offsetC = 0;
	SDL_Rect rect;
	if (veld == veltI)
	{
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_SetTextInputRect(&stretchRect);
		SDL_StartTextInput();
		inputText = pString;
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
	stretchRect.x = stretchRect.x + 2;
	stretchRect.y = stretchRect.y + 2;
	stretchRect.w = stretchRect.w - 4;
	stretchRect.h = stretchRect.h - 4;
	if (!pString->empty())
	{
		defo = *pString;
	}
	if(veld == veltI && !defo.empty())
	{
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
		while (defo.find(" ", 1) != string::npos)
		{
			surfaceText = TTF_RenderText_Solid(font, defo.substr(0, defo.find(" ", 1)).c_str(), white);
			Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
			SDL_FreeSurface(surfaceText);
			SDL_QueryTexture(Message, NULL, NULL, &rect.w, &rect.h);
			if (lineX  + rect.w >= stretchRect.x + stretchRect.w)
			{
				lineX = stretchRect.x;
				lineY += rect.h;
				if (defo[0] == *" ")
				{
					offsetC++;
					defo = defo.substr(1, defo.size());
					surfaceText = TTF_RenderText_Solid(font, defo.substr(0, defo.find(" ", 1)).c_str(), white);
					SDL_DestroyTexture(Message);
					Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
					SDL_FreeSurface(surfaceText);
					SDL_QueryTexture(Message, NULL, NULL, &rect.w, &rect.h);
				}
			}
			rect.x = lineX;
			rect.y = lineY;
			lineX += rect.w;
			SDL_RenderCopy(gRenderer, Message, NULL, &rect);
			SDL_DestroyTexture(Message);
			if (offsetC + defo.find(" ", 1) >= offset && offsetC <= offset && SDL_GetTicks64() / 500 % 2)
			{
				if (offsetC == offset)
				{
					SDL_RenderDrawLine(gRenderer, rect.x, rect.y, rect.x, rect.h + rect.y);
				}
				else
				{
					surfaceText = TTF_RenderText_Solid(font, defo.substr(0, offset - offsetC).c_str(), white);
					SDL_DestroyTexture(Message);
					Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
					SDL_FreeSurface(surfaceText);
					SDL_QueryTexture(Message, NULL, NULL, &rect.w, &rect.h);
					SDL_RenderDrawLine(gRenderer, rect.w + rect.x, rect.y, rect.w + rect.x, rect.h + rect.y);
				}
				SDL_DestroyTexture(Message);
			}

			offsetC += int(defo.find(" ", 1));
			defo = defo.substr(defo.find(" ",1), defo.length());
			
		}
		surfaceText = TTF_RenderText_Solid(font, defo.c_str(), white);
		Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
		SDL_FreeSurface(surfaceText);
		SDL_QueryTexture(Message, NULL, NULL, &rect.w, &rect.h);
		if (lineX  + rect.w >= stretchRect.x + stretchRect.w)
		{
			lineX = stretchRect.x;
			lineY += rect.h;
			if (defo[0] == *" ")
			{
				offsetC++;
				defo = defo.substr(1, defo.size());
				surfaceText = TTF_RenderText_Solid(font, defo.substr(0, defo.find(" ", 1)).c_str(), white);
				SDL_DestroyTexture(Message);
				Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
				SDL_FreeSurface(surfaceText);
				SDL_QueryTexture(Message, NULL, NULL, &rect.w, &rect.h);
			}
		}
		rect.x = lineX;
		rect.y = lineY;
		lineX += rect.w;
		SDL_RenderCopy(gRenderer, Message, NULL, &rect);
		SDL_DestroyTexture(Message);
		if (offsetC + defo.size() >= offset && offsetC <= offset && SDL_GetTicks64() / 500 % 2)
		{
			if (offsetC == offset)
			{
				SDL_RenderDrawLine(gRenderer, rect.x, rect.y, rect.x, rect.h + rect.y);
			}
			else
			{
				surfaceText = TTF_RenderText_Solid(font, defo.substr(0, offset - offsetC).c_str(), white);
				SDL_DestroyTexture(Message);
				Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
				SDL_FreeSurface(surfaceText);
				SDL_QueryTexture(Message, NULL, NULL, &rect.w, &rect.h);
				SDL_RenderDrawLine(gRenderer, rect.w + rect.x, rect.y, rect.w + rect.x, rect.h + rect.y);
			}
			SDL_DestroyTexture(Message);
		}
	}
	else
	{
		/*surfaceText = TTF_RenderText_Blended_Wrapped(font, defo.c_str(), white, stretchRect.w);
		Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
		SDL_FreeSurface(surfaceText);
		SDL_QueryTexture(Message, NULL, NULL, &stretchRect.w, &stretchRect.h);
		SDL_RenderCopy(gRenderer, Message, NULL, &stretchRect);
		SDL_DestroyTexture(Message);*/

		while (defo.find(" ", 1) != string::npos)
		{
			surfaceText = TTF_RenderText_Solid(font, defo.substr(0, defo.find(" ", 1)).c_str(), white);
			Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
			SDL_FreeSurface(surfaceText);
			SDL_QueryTexture(Message, NULL, NULL, &rect.w, &rect.h);
			if (lineX + rect.w >= stretchRect.x + stretchRect.w)
			{
				lineX = stretchRect.x;
				lineY += rect.h;
				if (defo[0] == *" ")
				{
					defo = defo.substr(1, defo.size());
					surfaceText = TTF_RenderText_Solid(font, defo.substr(0, defo.find(" ", 1)).c_str(), white);
					SDL_DestroyTexture(Message);
					Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
					SDL_FreeSurface(surfaceText);
					SDL_QueryTexture(Message, NULL, NULL, &rect.w, &rect.h);
				}
			}
			rect.x = lineX;
			rect.y = lineY;
			lineX += rect.w;
			SDL_RenderCopy(gRenderer, Message, NULL, &rect);
			SDL_DestroyTexture(Message);
			defo = defo.substr(defo.find(" ", 1), defo.length());

		}
		surfaceText = TTF_RenderText_Solid(font, defo.c_str(), white);
		Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
		SDL_FreeSurface(surfaceText);
		SDL_QueryTexture(Message, NULL, NULL, &rect.w, &rect.h);
		if (lineX + rect.w >= stretchRect.x + stretchRect.w)
		{
			lineX = stretchRect.x;
			lineY += rect.h;
			if (defo[0] == *" ")
			{
				defo = defo.substr(1, defo.size());
				surfaceText = TTF_RenderText_Solid(font, defo.substr(0, defo.find(" ", 1)).c_str(), white);
				SDL_DestroyTexture(Message);
				Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
				SDL_FreeSurface(surfaceText);
				SDL_QueryTexture(Message, NULL, NULL, &rect.w, &rect.h);
			}
		}
		rect.x = lineX;
		rect.y = lineY;
		lineX += rect.w;
		SDL_RenderCopy(gRenderer, Message, NULL, &rect);
		SDL_DestroyTexture(Message);
	}

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
	string textR = temp->text;

	int raV[6];
	for (int i = 0; i < 6; i++)
	{
		if (temp->ra[i].empty())
		{
			if (i % 2)
			{
				raV[i] = 10;
			}
			else
			{
				raV[i] = 0;
			}
		}
		else
		{
			raV[i] = stoi(temp->ra[i]);
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (raV[i*2]>= raV[i*2+1])
		{
			raV[i * 2 + 1] = raV[i * 2] + 1;
		}
	}
	while (textR.find(" x ") != string::npos)
	{
		textR.replace(textR.find(" x "), 3, " " + to_string(rands[0] % (raV[1] - raV[0]) + raV[0]) + " ");
	}
	while (textR.find(" y ") != string::npos)
	{
		textR.replace(textR.find(" y "), 3, " " + to_string(rands[1] % (raV[3] - raV[2]) + raV[2]) + " ");
	}
	while (textR.find(" z ") != string::npos)
	{
		textR.replace(textR.find(" z "), 3, " " + to_string(rands[2] % (raV[5] - raV[4]) + raV[4]) + " ");
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

	if (temp->img!=stringMem)
	{
		SDL_DestroyTexture(imgMain);
		imgMain = loadTexture("textures/" + temp->img + ".png");
		if (imgMain == NULL)
		{
			imgMain = loadTexture("textures/" + temp->img + ".jpg");
		}
		if (imgMain == NULL)
		{
			imgMain = loadTexture("textures/" + temp->img + ".jpeg");
		}
	}
	stringMem = temp->img;
	if (!imgMain == NULL)
	{
		SDL_QueryTexture(imgMain, NULL, NULL, &stretchRect.w, &stretchRect.h);
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
		SDL_RenderCopy(gRenderer, imgMain, NULL, &stretchRect);
		//SDL_DestroyTexture(imgMain);
	}

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
	SDL_Rect rect;
	int lineX = x;
	int lineY = y;
	bool newL = false;
	// "\rtest\wtest"
	// " test\wtest"
	// " test test"
	// " test test  "
	// "test\rtest"
	// "test\wtest"
	// "\rtest\wtest"
	// " test test  "

	textR += ' ';
	while (textR.find(" ", 1) != string::npos || textR.find("\\") != string::npos)
	{
		while ( textR.find(" ", 1) == string::npos  || (textR.find(" ", 1) > textR.find("\\") && textR.find(" ", 1) != string::npos))
		{

			switch (textR.at(textR.find("\\") + 1))
			{
			case 'r':
				collor = &red;
				break;
			case 'g':
				collor = &green;
				break;
			case 'b':
				collor = &blau;
				break;
			case 'p':
				collor = &purple;
				break;
			case 'n':
				newL = true;
				break;
			default:
				collor = &white;
				break;
			}
			if (textR.size() > textR.find("\\") + 2)
			{
				textR.replace(textR.find("\\"), 2, "");
			}
			else
			{
				textR.replace(textR.find("\\"), 1, " ");
			}
		}
		if(textR.find(" ", 1) != string::npos)
		{
			surfaceText = TTF_RenderText_Solid(SansS, textR.substr(0, textR.find(" ", 1)).c_str(), *collor);
			Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
			SDL_FreeSurface(surfaceText);
			SDL_QueryTexture(Message, NULL, NULL, &rect.w, &rect.h);
			if (lineX + rect.w >= x + w || newL)
			{
				newL = false;
				lineX = x;
				lineY += rect.h;
				if (textR[0] == *" ")
				{
					textR = textR.substr(1, textR.size());
					surfaceText = TTF_RenderText_Solid(SansS, textR.substr(0, textR.find(" ",1)).c_str(), *collor);
					SDL_DestroyTexture(Message);
					Message = SDL_CreateTextureFromSurface(gRenderer, surfaceText);
					SDL_FreeSurface(surfaceText);
					SDL_QueryTexture(Message, NULL, NULL, &rect.w, &rect.h);
				}
			}
			rect.x = lineX;
			rect.y = lineY;
			lineX += rect.w;
			SDL_RenderCopy(gRenderer, Message, NULL, &rect);
			SDL_DestroyTexture(Message);
			if (textR.size() > 1)
			{
				textR = textR.substr(textR.find(" ", 1), textR.length());
			}
		}
	}

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

int draw::cardSelect()
{
	SDL_Rect stretchRect;
	int conC = 0;
	int opt[10] = { 0,0,0,0,0,0,0,0,0,0 };
	card* temp = link->get(id);
	card* temp2 = NULL;
	int val0 = 0;
	int val1 = 0;
	int val2 = 0;
	if (next == PLAY)
	{
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
	}
	if (next == EDDITOR)
	{
		opt[0] = (id / 10)*10;
		if (opt[0]+10 < link->max)
		{
			conC = 10;
		}
		else
		{
			conC = link->max - opt[0];
		}
		for (int i = 1; i < conC; i++)
		{
			opt[i] = opt[i - 1] + 1;
		}
	}
	if (conC == 2)
	{
		stretchRect.x = 10;
		stretchRect.y = 10;
		stretchRect.w = SCREEN_WIDTH/2-30;
		stretchRect.h = SCREEN_HEIGHT-20;
		if (updatePreview)
		{
			cardPreview(opt[0], selectTexture[0]);
		}
		SDL_RenderCopy(gRenderer, selectTexture[0], NULL, &stretchRect);
		if (xMouse > (stretchRect.x) && xMouse < (stretchRect.x + stretchRect.w) && yMouse > stretchRect.y && yMouse < stretchRect.y + stretchRect.h && mousePres)
		{
			GAME_STATE = next;
			id = opt[0];
		}
		stretchRect.x = SCREEN_WIDTH / 2 + 20;
		stretchRect.w = SCREEN_WIDTH / 2 - 30;
		if (updatePreview)
		{
			cardPreview(opt[1], selectTexture[1]);
		}
		SDL_RenderCopy(gRenderer, selectTexture[1], NULL, &stretchRect);
		if (xMouse > (stretchRect.x) && xMouse < (stretchRect.x + stretchRect.w) && yMouse > stretchRect.y && yMouse < stretchRect.y + stretchRect.h && mousePres)
		{
			GAME_STATE = next;
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
			if (updatePreview)
			{
				cardPreview(opt[i], selectTexture[i]);
			}
			SDL_RenderCopy(gRenderer, selectTexture[i], NULL, &stretchRect);
			if (xMouse > (stretchRect.x) && xMouse < (stretchRect.x + stretchRect.w) && yMouse > stretchRect.y && yMouse < stretchRect.y + stretchRect.h && mousePres)
			{
				GAME_STATE = next;
				id = opt[i];
			}
		}
	}
	else
	{
		for (int i = 0; i < conC; i++)
		{
			stretchRect.w = SCREEN_WIDTH / 5;
			stretchRect.h = int(SCREEN_HEIGHT / 2.5);
			stretchRect.x = (stretchRect.w) * (i % 5);
			stretchRect.y = (stretchRect.h) * (i / 5);
			if (updatePreview)
			{
				cardPreview(opt[i], selectTexture[i]);
			}
			SDL_RenderCopy(gRenderer, selectTexture[i], NULL, &stretchRect);
			if (xMouse > (stretchRect.x) && xMouse < (stretchRect.x + stretchRect.w) && yMouse > stretchRect.y && yMouse < stretchRect.y + stretchRect.h && mousePres)
			{
				GAME_STATE = next;
				id = opt[i];
			}
		}
	}
	updatePreview = false;
	stretchRect.h = 40;
	stretchRect.w = 150;
	stretchRect.y = SCREEN_HEIGHT - 50;
	stretchRect.x = 1000;
	if (opt[0]+10 < link->max && next == EDDITOR && drawButton(stretchRect, "Next"))
	{
		updatePreview = true;
		id += 10;
	}
	if (opt[0] > 0)
	{
		stretchRect.x = 10;
		if (next == EDDITOR && drawButton(stretchRect, "Back"))
		{
			updatePreview = true;
			id -= 10;
		}
	}
	return 0;
}

int draw::cardPreview(int prevId,SDL_Texture* texture)
{
	//The final texture
	card* temp = link->get(prevId);;

	if (temp == nullptr)
	{
		return 1;
	}

	SDL_SetRenderTarget(gRenderer, texture);

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

int draw::continu()
{
	SDL_Rect stretchRect;
	int y = 0;
	const int w = 300;
	const int h = 90;

	y = 300;
	stretchRect.h = h;
	stretchRect.w = w;
	stretchRect.x = (SCREEN_WIDTH - stretchRect.w) / 2;
	stretchRect.y = y - stretchRect.h / 2;
	if (drawButton(stretchRect, "New"))
	{
		GAME_STATE = next;
	}

	y = 400;
	stretchRect.h = h;
	stretchRect.w = w;
	stretchRect.x = (SCREEN_WIDTH - stretchRect.w) / 2;
	stretchRect.y = y - stretchRect.h / 2;
	if (drawButton(stretchRect, "Continu"))
	{
		saveHandl->stateLoad(&id,values,tags,saveName);
		GAME_STATE = next;
	}
	return 0;
}
