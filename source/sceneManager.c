#include "prototype.h"

SDL_bool printMainMenu;
SDL_bool printGame;
SDL_bool printOption;
SDL_bool printCredit;

void initSceneManager(void)
{
	printMainMenu = SDL_TRUE;
	printGame = SDL_FALSE;
	printOption = SDL_FALSE;
	printCredit = SDL_FALSE;
}

SDL_bool isActiveOption(void)
{
	return printOption;
}

SDL_bool isActiveMainMenu(void)
{
	return printMainMenu;
}

SDL_bool isActiveGame(void)
{
	return printGame;
}

SDL_bool isActiveCredit(void)
{
	return printCredit;
}

void setPrintOption(SDL_bool active)
{
	printOption = active;
}

void setPrintMainMenu(SDL_bool active)
{
	printMainMenu = active;
}

void setPrintGame(SDL_bool active)
{
	printGame = active;
}

void setPrintCredit(SDL_bool active)
{
	printCredit = active;
}
