#include "prototype.h"

//CATALOGUE DES FONT
TTF_Font *fontHack;

void initFontHack(void)
{
	fontHack = TTF_OpenFont("source/fonts/hack.ttf", 15);

	if(fontHack == NULL)
	{
		SDL_Log("IMPOSSIBLE DE CHARGER LA FONT :%s",TTF_GetError());
		exit(EXIT_FAILURE);
	}
}

TTF_Font *getFontHack(void)
{
	return fontHack;
}

void cleanFontHack(void)
{
	SDL_Log("CLEAN FONTHack EN COURS\n");
	if(fontHack != NULL)
	{
		TTF_CloseFont(fontHack);
		fontHack = NULL;
	}	
	SDL_Log("CLEAN FONTHack TERMINER\n");
}