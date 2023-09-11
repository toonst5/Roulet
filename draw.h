#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <cstdio>
#include <string>
#include<iostream>
#include "cardLinked.h"
#include "linkedMod.h"
#include "saveHandler.h"
#include <SDL_mixer.h>


using namespace std;

class draw
{
public:
	int createWindow();
	//Starts up SDL and creates window

private:
	bool init();

	//Loads media
	bool loadMedia();

	//Frees media and shuts down SDL
	void close();

	enum STATE {
		MAIN,
		EDDITOR,
		EDDITORMENU,
		EDDITORSAVE,
		PLAY,
		LOAD,
		PREVIEW,
		SETTINGS,
		QUIT,
		CONTINU,
		CARDSELECT
	}GAME_STATE;

	STATE next;

	//Loads individual image as texture
	SDL_Texture* loadTexture(std::string path);

	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;

	//Current displayed texture
	SDL_Texture* gTextureButton = NULL;
	SDL_Texture* gTextureButtonPres = NULL;
	SDL_Texture* cardTexture = NULL;
	SDL_Texture* buttTexture = NULL;
	SDL_Texture* penisTexture = NULL;
	SDL_Texture* selectTexture[10];
	SDL_Texture* imgMain;




	Mix_Music* gMusic = NULL;
	Mix_Chunk* gSlap = NULL;

	SDL_AudioSpec* wanted = NULL;


	//Main loop flag
	bool quit = false;

	bool mousePres = false;

	//Event handler
	SDL_Event e;

	int SCREEN_WIDTH = 1200;
	int	SCREEN_HEIGHT = 800;

	int mainMenu();
	int edditor();
	int edditorSave();
	int edditorMenu();
	int load();
	int play();
	int drawTextBlock(int, SDL_Rect, string, TTF_Font*, string*);
	int drawCard();
	int drawButton(SDL_Rect,string);
	int cardSelect();
	int cardPreview(int, SDL_Texture*);
	int settings();
	int continu();

	int values[3];
	int xMouse;
	int yMouse;
	int offset;

	bool update;
	bool updatePreview;
	bool showStats;
	string stringMem;
	TTF_Font* Sans;
	TTF_Font* SansS;
	TTF_Font* SansB;
	SDL_Color white;
	SDL_Color red;
	SDL_Color green;
	SDL_Color blau;
	SDL_Color purple;
	cardLinked* link;
	linkedMod* tags;
	int id;
	int veld;
	string text;
	string saveName;
	int startTime;
	saveHandler * saveHandl;
	bool cheats;

	string* inputText;

	int rands[3];
};

