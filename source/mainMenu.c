#include "prototype.h"

static SDL_bool anime;
static SDL_bool animeCorps;

static Cercle liste_cercle[100];
static int nbCercle;

static GroupeGui menu;

void initMainMenu(void)
{
	SDL_Log("--------------INIT MAIN MENU EN COURS--------------\n");
	nbCercle = 100;
	setPrintMainMenu(SDL_TRUE);
	initGroupeGui(&menu, 0, 5, 4, 2);

	initFontHack();
	anime = SDL_TRUE;
	animeCorps = SDL_FALSE;

	if(getLocalisation() == 0)
	{
		//int x, int y, int w, int h, int texture,SDL_Color panelColor, char *ptexte, SDL_Color texteColor, TTF_Font *font, SDL_bool pFill, int event
		menu.liste_bouton[0] = createBouton(0, 200, 250, 50, -1, getNoir(255), "EN AVANT", getBlanc(255), getFontHack(), SDL_TRUE, 1);
		menu.liste_bouton[1] = createBouton(0, 250, 250, 50, -1, getNoir(255), "SALLE DES MACHINES", getBlanc(255), getFontHack(), SDL_TRUE, 2);
		menu.liste_bouton[2] = createBouton(0, 300, 250, 50, -1, getNoir(255), "EQUIPAGE", getBlanc(255), getFontHack(), SDL_TRUE, 9);
		menu.liste_bouton[3] = createBouton(0, 350, 250, 50, -1, getNoir(255), "QUITTER", getBlanc(255), getFontHack(), SDL_TRUE, 0);

		//int x, int y, int w, int h, int texture, SDL_Color color, SDL_bool pFill
		menu.liste_panel[0] = createPanel(0, 199, 252, 202, -1, getBlanc(255), SDL_FALSE);
		menu.liste_panel[1] = createPanel(-200, 50, 200, 100, -1, getBlanc(255), SDL_TRUE);

		//int x, int y, int w, int h, char *texte, SDL_Color color, TTF_Font *font
		menu.liste_texte[0] = createTexte(0, 50, 200, 50, "BIENVENU", getNoir(255), getFontHack());
		menu.liste_texte[1] = createTexte(0, 75, 200, 50, "DANS", getNoir(255), getFontHack());
		menu.liste_texte[2] = createTexte(0, 100, 200, 50, "REPULSIVE", getNoir(255), getFontHack());
		menu.liste_texte[3] = createTexte(400, 175, 200, 50, "TROUVER UN MOYEN DE GUERIR LA GALAXIE", getBlanc(255), getFontHack());
		menu.liste_texte[4] = createTexte(400, 200, 200, 50, "TOUT LE MONDE DEPEND DE VOUS ET VOTRE EQUIPAGE", getBlanc(255), getFontHack());
	}
	else
	{
		//int x, int y, int w, int h, int texture,SDL_Color panelColor, char *ptexte, SDL_Color texteColor, TTF_Font *font, SDL_bool pFill, int event
		menu.liste_bouton[0] = createBouton(0, 200, 250, 50, -1, getNoir(255), "FORWARD", getBlanc(255), getFontHack(), SDL_TRUE, 1);
		menu.liste_bouton[1] = createBouton(0, 250, 250, 50, -1, getNoir(255), "MACHINE ROOM", getBlanc(255), getFontHack(), SDL_TRUE, 2);
		menu.liste_bouton[2] = createBouton(0, 300, 250, 50, -1, getNoir(255), "CREW", getBlanc(255), getFontHack(), SDL_TRUE, 9);
		menu.liste_bouton[3] = createBouton(0, 350, 250, 50, -1, getNoir(255), "QUIT", getBlanc(255), getFontHack(), SDL_TRUE, 0);

		//int x, int y, int w, int h, int texture, SDL_Color color, SDL_bool pFill
		menu.liste_panel[0] = createPanel(0, 199, 252, 202, -1, getBlanc(255), SDL_FALSE);
		menu.liste_panel[1] = createPanel(-200, 50, 200, 100, -1, getBlanc(255), SDL_TRUE);

		//int x, int y, int w, int h, char *texte, SDL_Color color, TTF_Font *font
		menu.liste_texte[0] = createTexte(0, 50, 200, 50, "WELCOME", getNoir(255), getFontHack());
		menu.liste_texte[1] = createTexte(0, 75, 200, 50, "IN", getNoir(255), getFontHack());
		menu.liste_texte[2] = createTexte(0, 100, 200, 50, "REPULSIVE", getNoir(255), getFontHack());
		menu.liste_texte[3] = createTexte(400, 175, 200, 50, "FIND A WAY TO CURE GALAXY", getBlanc(255), getFontHack());
		menu.liste_texte[4] = createTexte(400, 200, 200, 50, "EVERYBODY DEPEND OF YOU AND YOUR CREW", getBlanc(255), getFontHack());
	}

	cleanFontHack();

	for(int i = 0; i < nbCercle; i++)
	{
		liste_cercle[i].cx = (rand() % 800) * getScaleX();
		liste_cercle[i].cy = (rand() % 600) * getScaleY();
		liste_cercle[i].rayon = rand() % 5;
		liste_cercle[i].couleur = getBlanc(255);
		if(rand() % 2 == 1)
		{
			liste_cercle[i].vitesse.x = rand() % 5;
		}
		else
		{
			liste_cercle[i].vitesse.x = -(rand() % 5);
		}
		if(rand() % 2 == 1)
		{
			liste_cercle[i].vitesse.y = rand() % 5;
		}
		else
		{
			liste_cercle[i].vitesse.y = -(rand() % 5);
		}

		if(liste_cercle[i].vitesse.x == 0 && liste_cercle[i].vitesse.y == 0)
		{
			liste_cercle[i].vitesse.y = 2;
		}
	}
	SDL_Log("--------------INIT MAIN MENU TERMINER--------------\n");
}

void updateMainMenu(void)
{
	if(anime != SDL_FALSE)
	{
		if(menu.liste_panel[1].dimension.x < 0)
		{
			menu.liste_panel[1].dimension.x += 5;
		}
		else
		{
			menu.liste_panel[1].dimension.x = 0;
			anime = SDL_FALSE;
		}
	}

	if(animeCorps == SDL_TRUE)
	{
		if(getMaxCercle() >= 99)
		{
			cleanMainMenu();
		}
	}
	for(int i = 0; i < nbCercle; i++)
	{
		liste_cercle[i].cx += liste_cercle[i].vitesse.x;
		liste_cercle[i].cy += liste_cercle[i].vitesse.y;

		if(animeCorps == SDL_FALSE)
		{
			if(liste_cercle[i].vitesse.x < 0 && liste_cercle[i].cx < 0)
			{
				liste_cercle[i].cx = getWidth() + liste_cercle[i].rayon * 2;
				liste_cercle[i].cy = rand() % getHeight();
			}
			else if(liste_cercle[i].vitesse.x > 0 && liste_cercle[i].cx > getWidth())
			{
				liste_cercle[i].cx = 0 - liste_cercle[i].rayon * 2;
				liste_cercle[i].cy = rand() % getHeight();
			}

			if(liste_cercle[i].vitesse.y < 0 && liste_cercle[i].cy < 0)
			{
				liste_cercle[i].cx = rand() % getWidth();
				liste_cercle[i].cy = getHeight() + liste_cercle[i].rayon * 2;
			}
			else if(liste_cercle[i].vitesse.y > 0 && liste_cercle[i].cy > getHeight())
			{
				liste_cercle[i].cx = rand() % getWidth();
				liste_cercle[i].cy = 0 - liste_cercle[i].rayon * 2;
			}
		}
		else
		{
			if(liste_cercle[i].cx + liste_cercle[i].rayon < 0 || liste_cercle[i].cx - liste_cercle[i].rayon  > getWidth() || 
				liste_cercle[i].cy + liste_cercle[i].rayon  < 0 || liste_cercle[i].cy - liste_cercle[i].rayon  > getHeight())
			{
				if(liste_cercle[i].vitesse.x != 0 || liste_cercle[i].vitesse.y != 0)
				{
					addCercle();
					liste_cercle[i].vitesse.x = 0;
					liste_cercle[i].vitesse.y = 0;
				}
			}
		}
	}
	if(animeCorps == SDL_FALSE)
	{
		updateGroupeGui(&menu);
	}
}

void drawMainMenu(void)
{
	if(animeCorps == SDL_FALSE)
	{
		drawGroupeGui(&menu);
	}
	for(int i = 0; i < nbCercle; i++)
	{
		drawCercle(liste_cercle[i]);
	}
}

void cleanMainMenu(void)
{
	SDL_Log("--------------CLEAN MAIN MENU EN COURS--------------\n");
	setPrintMainMenu(SDL_FALSE);
	cleanGroupeGui(&menu);
	SDL_Log("--------------CLEAN MAIN MENU TERMINER--------------\n");
}

//function math.angle(x1,y1, x2,y2) return math.atan2(y2-y1, x2-x1) end

void startAnimePlayerCorp(void)
{
	animeCorps = SDL_TRUE;
	int x = getWidth() / 2;
	int y = getHeight() / 2;
	for(int i = 0; i < nbCercle; i++)
	{
		double angle = atan2(liste_cercle[i].cy - y, liste_cercle[i].cx - x);

		liste_cercle[i].vitesse.x = 15 * cos(angle);
		liste_cercle[i].vitesse.y = 15 * sin(angle);
	}
}