#include "prototype.h"

static GroupeGui menu;

static Ligne liste_ligne[50];
static int nbLigne;

void initCredit(void)
{
	SDL_Log("--------------INIT CREDIT EN COURS--------------\n");
	setPrintCredit(SDL_TRUE);
	nbLigne = 50;
	initGroupeGui(&menu, 0, 6, 1, 0);	

	initFontHack();

	if(getLocalisation() == 0)
	{
		//int x, int y, int w, int h, int texture,SDL_Color panelColor, char *ptexte, SDL_Color texteColor, TTF_Font *font, SDL_bool pFill, int event
		menu.liste_bouton[0] = createBouton(0, 400, 250, 50, -1, getNoir(255), "RETOUR SUR LE PONT", getBlanc(255), getFontHack(), SDL_TRUE, 10);

		//int x, int y, int w, int h, char *texte, SDL_Color color, TTF_Font *font
		menu.liste_texte[0] = createTexte(200, 50, 300, 50, "JEU PROGRAMMER PAR BLOVORAD", getBlanc(255),getFontHack());
		menu.liste_texte[1] = createTexte(200, 100, 200, 50, "FEATURING TON CLAVIER", getBlanc(255),getFontHack());
		menu.liste_texte[2] = createTexte(200, 150, 200, 50, "ACCEPTATION DE TON ECRAN", getBlanc(255),getFontHack());
		menu.liste_texte[3] = createTexte(200, 200, 200, 50, "ACCORD DE TA SOURIS", getBlanc(255),getFontHack());
		menu.liste_texte[4] = createTexte(250, 250, 200, 50, "AUTORISATION DE TES ECOUTEURS", getBlanc(255),getFontHack());
		menu.liste_texte[5] = createTexte(300, 300, 200, 50, "ET SURTOUT OUVERT PAR TOI ! ", getBlanc(255),getFontHack());
	}
	else
	{
		//int x, int y, int w, int h, int texture,SDL_Color panelColor, char *ptexte, SDL_Color texteColor, TTF_Font *font, SDL_bool pFill, int event
		menu.liste_bouton[0] = createBouton(0, 400, 250, 50, -1, getNoir(255), "BACK TO CONTROL ROOM", getBlanc(255), getFontHack(), SDL_TRUE, 10);

		//int x, int y, int w, int h, char *texte, SDL_Color color, TTF_Font *font
		menu.liste_texte[0] = createTexte(200, 50, 300, 50, "GAME MADE BY BLOVORAD", getBlanc(255),getFontHack());
		menu.liste_texte[1] = createTexte(200, 100, 200, 50, "FEATURING YOUR KEYBOARD", getBlanc(255),getFontHack());
		menu.liste_texte[2] = createTexte(200, 150, 200, 50, "ACCEPT BY YOUR SCREEN", getBlanc(255),getFontHack());
		menu.liste_texte[3] = createTexte(200, 200, 200, 50, "AGREEMENT BY YOUR MOUSE", getBlanc(255),getFontHack());
		menu.liste_texte[4] = createTexte(250, 250, 200, 50, "PERMIT BY YOUR EARPHONE", getBlanc(255),getFontHack());
		menu.liste_texte[5] = createTexte(300, 300, 200, 50, "AND ESPECIALLY LAUNCH BY YOU ! ", getBlanc(255),getFontHack());
	}

	cleanFontHack();
	for(int i = 0; i < nbLigne; i++)
	{
		liste_ligne[i].x1 = rand() % getWidth();
		liste_ligne[i].x2 = liste_ligne[i].x1 + rand() % 20 + 5;

		liste_ligne[i].y1 = rand() % getHeight();
		liste_ligne[i].y2 = liste_ligne[i].y1;

		if(rand() % 2 == 1)
		{
			liste_ligne[i].vitesse.x = rand() % 2 + 2;
		}
		else
		{
			liste_ligne[i].vitesse.x = -(rand() % 2 + 2);
		}
		liste_ligne[i].vitesse.y = 0;

		liste_ligne[i].couleur = getBlanc(255);
	}
	SDL_Log("--------------INIT CREDIT TERMINER--------------\n");
}

void updateCredit(void)
{
	updateGroupeGui(&menu);

	for(int i = 0; i < nbLigne; i++)
	{
		liste_ligne[i].x1 += liste_ligne[i].vitesse.x;
		liste_ligne[i].x2 += liste_ligne[i].vitesse.x;
		liste_ligne[i].y1 += liste_ligne[i].vitesse.y;
		liste_ligne[i].y2 += liste_ligne[i].vitesse.y;

		if(liste_ligne[i].vitesse.x > 0)
		{
			if(liste_ligne[i].x1 > getWidth())
			{
				liste_ligne[i].x1 = -25;
				liste_ligne[i].x2 = liste_ligne[i].x1 + rand() % 20 + 5;
				liste_ligne[i].y1 = rand() % getHeight();
				liste_ligne[i].y2 = liste_ligne[i].y1;
			}
		}
		else
		{
			if(liste_ligne[i].x2 < 0)
			{
				liste_ligne[i].x1 = getWidth();
				liste_ligne[i].x2 = liste_ligne[i].x1 + rand() % 20 + 5;
				liste_ligne[i].y1 = rand() % getHeight();
				liste_ligne[i].y2 = liste_ligne[i].y1;
			}
		}
	}
}

void drawCredit(void)
{
	drawGroupeGui(&menu);

	for(int i = 0; i < nbLigne; i++)
	{
		drawLine(liste_ligne[i].x1, liste_ligne[i].y1, liste_ligne[i].x2, liste_ligne[i].y2, &liste_ligne[i].couleur);
	}
}

void cleanCredit(void)
{
	setPrintCredit(SDL_FALSE);

	cleanGroupeGui(&menu);
}