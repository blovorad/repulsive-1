#include "prototype.h"

static Cercle objectif;
static Cercle objectifRemplir;
static Timer remplirTimer;
static SDL_bool isWin;
static void setObjectif(void);
static void checkVictory(void);
static void updateObjectif(void);

static Cercle *liste_hostile;
static int nbHostile;
static void initHostile(int level);
static void updateHostile(void);
static void drawHostile(void);

static GroupeGui menu;
static SDL_bool printMenu;
static void initMenu(void);

static GroupeGui tuto;
static Timer tutoTimer;
static void initTuto(void);
static SDL_bool activeTuto;

static GroupeGui winning;
static void initWinning(void);

static int currentLevel;

void initGame(void)
{
	SDL_Log("--------------INIT GAME EN COURS--------------\n");
	setPrintGame(SDL_TRUE);
	SDL_ShowCursor(SDL_DISABLE);

	currentLevel = 0;

	initPlayer();
	setObjectif();
	initMenu();
	initHostile(currentLevel);
	initTuto();
	initWinning();
	SDL_Log("--------------INIT GAME TERMINER--------------\n");
}

void updateGame(Input *input)
{
	if(!printMenu)
	{
		if(isWin == SDL_FALSE)
		{
			updatePlayer(input);
			updateObjectif();
			checkVictory();
		}
		else
		{
			updateGroupeGui(&winning);
		}
		updateHostile();
		if(activeTuto)
		{
			updateGroupeGui(&tuto);
			tutoTimer.currentTime = SDL_GetTicks();
			if(tutoTimer.currentTime >= tutoTimer.previousTime + tutoTimer.ms)
			{
				activeTuto = SDL_FALSE;
			}
			for(int i = 0; i < tuto.nbTexte; i++)
			{
				if(tuto.liste_texte[i].couleur.a < 255)
				{
					tuto.liste_texte[i].couleur.a += 2;
					if(tuto.liste_texte[i].couleur.a > 253)
					{
						tuto.liste_texte[i].couleur.a = 255;
					}
					SDL_SetTextureAlphaMod(tuto.liste_texte[i].sprite.texture, tuto.liste_texte[i].couleur.a);
				}
			}
		}
		else
		{
			if(tuto.liste_texte != NULL)
			{
				SDL_bool weClean = SDL_FALSE;
				for(int i = 0; i < tuto.nbTexte; i++)
				{
					if(tuto.liste_texte[i].couleur.a > 0)
					{
						tuto.liste_texte[i].couleur.a -= 2;
						if(tuto.liste_texte[i].couleur.a <= 3)
						{
							tuto.liste_texte[i].couleur.a = 0;
						}
						SDL_SetTextureAlphaMod(tuto.liste_texte[i].sprite.texture, tuto.liste_texte[i].couleur.a);
					}
					else
					{
						weClean = SDL_TRUE;
					}
				}
				if(weClean)
				{
					cleanGroupeGui(&tuto);
				}
			}
		}
	}
	else
	{
		updateGroupeGui(&menu);
	}
}

void drawGame(void)
{
	if(objectifRemplir.rayon > 0)
	{
		drawDisque(objectifRemplir);
	}
	drawCercle(objectif);
	drawHostile();
	drawPlayer();
	if(tuto.liste_texte != NULL)
	{
		drawGroupeGui(&tuto);
	}
	if(printMenu)
	{
		drawGroupeGui(&menu);
	}
	if(isWin)
	{
		drawGroupeGui(&winning);
	}
}

void cleanGame(void)
{
	SDL_Log("----------------CLEAN GAME EN COURS----------------\n");
	setPrintGame(SDL_FALSE);
	SDL_ShowCursor(SDL_ENABLE);

	cleanPlayer();
	cleanGroupeGui(&menu);
	cleanGroupeGui(&winning);
	if(tuto.liste_texte != NULL)
	{
		cleanGroupeGui(&tuto);
	}

	if(liste_hostile != NULL)
	{
		free(liste_hostile);
		liste_hostile = NULL;
		nbHostile = 0;
	}
	SDL_Log("----------------CLEAN GAME TERMINER----------------\n");
}

static void initTuto(void)
{
	activeTuto = SDL_TRUE;
	initGroupeGui(&tuto, 0, 3, 0, 0);
	initFontHack();

	//int x, int y, int w, int h, char *texte, SDL_Color color, TTF_Font *font
	if(getLocalisation() == 0)
	{
		tuto.liste_texte[0] = createTexte(300, 100, 200, 50, "ZQSD POUR SE DEPLACER", getBlanc(0), getFontHack());
		tuto.liste_texte[1] = createTexte(300, 125, 200, 50, "RESTER DANS LE CERCLE JAUNE", getBlanc(0), getFontHack());
		tuto.liste_texte[2] = createTexte(300, 150, 200, 50, "EVITER LES CERCLES ROUGE", getBlanc(0), getFontHack());
	}
	else
	{
		tuto.liste_texte[0] = createTexte(300, 100, 200, 50, "WASD TO MOVE", getBlanc(0), getFontHack());
		tuto.liste_texte[1] = createTexte(300, 125, 200, 50, "STAY IN YELLOW CIRCLE", getBlanc(0), getFontHack());
		tuto.liste_texte[2] = createTexte(300, 150, 200, 50, "AVOID RED CIRCLE", getBlanc(0), getFontHack());
	}

	for(int i = 0; i < tuto.nbTexte; i++)
	{
		SDL_SetTextureAlphaMod(tuto.liste_texte[i].sprite.texture, tuto.liste_texte[i].couleur.a++);
	}

	tutoTimer.ms = 4000;
	tutoTimer.currentTime = 0;
	tutoTimer.previousTime = SDL_GetTicks();

	cleanFontHack();	
}

static void setObjectif(void)
{
	objectif.cx = 400 * getScaleX();
	objectif.cy = 50 * getScaleY();

	if(rand() % 2 == 1)
	{
		objectif.vitesse.x = rand() % 1 + 1;
	}
	else
	{
		objectif.vitesse.x = -(rand() % 1 + 1);
	}
	if(rand() % 2 == 1)
	{
		objectif.vitesse.y = rand() % 1 + 1;
	}
	else
	{
		objectif.vitesse.y = -(rand() % 1 + 1);
	}
	objectif.rayon = 20;

	objectif.couleur = getJaune(255);

	objectifRemplir.cx = 400 * getScaleX();
	objectifRemplir.cy = 50 * getScaleY();

	objectifRemplir.vitesse.x = 0;
	objectifRemplir.vitesse.y = 0;
	objectifRemplir.rayon = 0;

	objectifRemplir.couleur = getJaune(255);

	remplirTimer.ms = 200;
	remplirTimer.currentTime = 0;
	remplirTimer.previousTime = SDL_GetTicks();

	isWin = SDL_FALSE;
}

static void updateObjectif(void)
{
	objectif.cx += objectif.vitesse.x;
	objectif.cy += objectif.vitesse.y;
	objectifRemplir.cx += objectif.vitesse.x;
	objectifRemplir.cy += objectif.vitesse.y;

	if(objectif.cx - objectif.rayon < 0)
	{
		objectif.cx = 0 + objectif.rayon;
		objectifRemplir.cx = 0 + objectif.rayon;
		objectif.vitesse.x *= -1;
		if(rand() % 2 == 1)
		{
			objectif.vitesse.y = rand() % 1 + 1;
		}
		else
		{
			objectif.vitesse.y = -(rand() % 1 + 1);
		}
	}
	else if(objectif.cx + objectif.rayon > getWidth())
	{
		objectif.cx = getWidth() - objectif.rayon;
		objectifRemplir.cx = getWidth() - objectif.rayon;
		objectif.vitesse.x *= -1;
		if(rand() % 2 == 1)
		{
			objectif.vitesse.y = rand() % 1 + 1;
		}
		else
		{
			objectif.vitesse.y = -(rand() % 1 + 1);
		}
	}
	if(objectif.cy + objectif.rayon > getHeight())
	{
		objectif.cy = getHeight() - objectif.rayon;
		objectifRemplir.cy = getHeight() - objectif.rayon;
		if(rand() % 2 == 1)
		{
			objectif.vitesse.x = rand() % 1 + 1;
		}
		else
		{
			objectif.vitesse.x = -(rand() % 1 + 1);
		}
		objectif.vitesse.y *= -1;
	}
	else if(objectif.cy - objectif.rayon < 0)
	{
		objectif.cy = 0 + objectif.rayon;
		objectifRemplir.cy = 0 + objectif.rayon;
		if(rand() % 2 == 1)
		{
			objectif.vitesse.x = rand() % 1 + 1;
		}
		else
		{
			objectif.vitesse.x = -(rand() % 1 + 1);
		}
		objectif.vitesse.y *= -1;
	}
}

static void checkVictory(void)
{
	int x = getPlayerX();
	int y = getPlayerY();

	if(absolue(objectif.cx, x) < objectif.rayon && absolue(objectif.cy, y) < objectif.rayon)
	{
		remplirTimer.currentTime = SDL_GetTicks();
		if(remplirTimer.currentTime >= remplirTimer.previousTime + remplirTimer.ms)
		{
			if(isPlayerAlive())
			{
				objectifRemplir.rayon++;
				if(objectifRemplir.rayon >= objectif.rayon)
				{
					objectifRemplir.rayon = objectif.rayon;
				}
			}
			remplirTimer.previousTime = remplirTimer.currentTime;
		}
	}
	else
	{
		remplirTimer.currentTime = SDL_GetTicks();
		if(remplirTimer.currentTime >= remplirTimer.previousTime + remplirTimer.ms)
		{
			objectifRemplir.rayon--;
			if(objectifRemplir.rayon < 0)
			{
				objectifRemplir.rayon = 0;
			}
			remplirTimer.previousTime = remplirTimer.currentTime;
		}
	}
	if(isWin == SDL_FALSE)
	{
		if(objectifRemplir.rayon == objectif.rayon)
		{
			isWin = SDL_TRUE;
			for(int i = 0; i < nbHostile; i++)
			{
				liste_hostile[i].couleur = getJaune(255);
			}
			SDL_ShowCursor(SDL_ENABLE);
			if(currentLevel == 2)
			{
				SDL_DestroyTexture(winning.liste_bouton[0].texte.sprite.texture);
				winning.liste_bouton[0].texte.sprite.texture = NULL;
				SDL_DestroyTexture(winning.liste_bouton[1].texte.sprite.texture);
				winning.liste_bouton[1].texte.sprite.texture = NULL;
				free(winning.liste_texte);
				winning.liste_texte = NULL;

				winning.nbTexte = 1;
				winning.liste_texte = malloc(sizeof(Texte) * winning.nbTexte);
				if(winning.liste_texte == NULL)
				{
					fprintf(stderr, "malloc; %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}

				winning.liste_bouton[0] = createBouton(300, 275, 200, 50, -1, getNoir(255), "MAIN MENU", getBlanc(255), getFontHack(), SDL_TRUE, 4);
			}
		}
	}
}

static void initWinning(void)
{
	initGroupeGui(&winning, 0, 1, 2, 1);
	initFontHack();

	if(getLocalisation() == 0)
	{
		//int x, int y, int w, int h, int texture,SDL_Color panelColor, char *ptexte, SDL_Color texteColor, TTF_Font *font, SDL_bool pFill, int event
		winning.liste_bouton[0] = createBouton(300, 275, 200, 50, -1, getNoir(255), "MENU PRINCIPAL", getBlanc(255), getFontHack(), SDL_TRUE, 4);
		winning.liste_bouton[1] = createBouton(300, 325, 200, 50, -1, getNoir(255), "SYSTEM SUIVANT", getBlanc(255), getFontHack(), SDL_TRUE, 12);

		//int x, int y, int w, int h, int texture, SDL_Color color, SDL_bool pFill
		winning.liste_panel[0] = createPanel(290, 270, 220, 110, -1, getBleu(255), SDL_TRUE);

		//int x, int y, int w, int h, char *texte, SDL_Color color, TTF_Font *font
		winning.liste_texte[0] = createTexte(300, 225, 200, 50, "FELICITATION VOUS AVEZ PURIFIER LE SYSTEME", getBlanc(255), getFontHack());
	}
	else
	{
		//int x, int y, int w, int h, int texture,SDL_Color panelColor, char *ptexte, SDL_Color texteColor, TTF_Font *font, SDL_bool pFill, int event
		winning.liste_bouton[0] = createBouton(300, 275, 200, 50, -1, getNoir(255), "MAIN MENU", getBlanc(255), getFontHack(), SDL_TRUE, 4);
		winning.liste_bouton[1] = createBouton(300, 325, 200, 50, -1, getNoir(255), "NEXT SYSTEM", getBlanc(255), getFontHack(), SDL_TRUE, 12);

		//int x, int y, int w, int h, int texture, SDL_Color color, SDL_bool pFill
		winning.liste_panel[0] = createPanel(290, 270, 220, 110, -1, getBleu(255), SDL_TRUE);

		//int x, int y, int w, int h, char *texte, SDL_Color color, TTF_Font *font
		winning.liste_texte[0] = createTexte(300, 225, 200, 50, "CONGRATULATION YOU HAVE CURE THE SYSTEM", getBlanc(255), getFontHack());
	}

	cleanFontHack();	
}

static void initHostile(int level)
{
	if(getCurrentResolution() == 0)
	{
		if(level == 0)
		{
			nbHostile = 75;
		}
		else if(level == 1)
		{
			nbHostile = 95;
		}
		else if(level == 2)
		{
			nbHostile = 115;
		}
	}
	else if(getCurrentResolution() == 1)
	{
		if(level == 0)
		{
			nbHostile = 95;
		}
		else if(level == 1)
		{
			nbHostile = 115;
		}
		else if(level == 2)
		{
			nbHostile = 135;
		}
	}
	else if(getCurrentResolution() == 2)
	{
		if(level == 0)
		{
			nbHostile = 115;
		}
		else if(level == 1)
		{
			nbHostile = 135;
		}
		else if(level == 2)
		{
			nbHostile = 155;
		}
	}
	else if(getCurrentResolution() == 3)
	{
		if(level == 0)
		{
			nbHostile = 135;
		}
		else if(level == 1)
		{
			nbHostile = 155;
		}
		else if(level == 2)
		{
			nbHostile = 175;
		}
	}
	else
	{
		if(level == 0)
		{
			nbHostile = 200;
		}
		else if(level == 1)
		{	
			nbHostile = 220;
		}
		else if(level == 2)
		{
			nbHostile = 250;
		}
	}

	liste_hostile = malloc(sizeof(Cercle) * nbHostile);
	if(liste_hostile == NULL && nbHostile > 0)
	{
		fprintf(stderr, "malloc; %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < nbHostile; i++)
	{
		liste_hostile[i].rayon = rand() % 5 + 5;

		if(level == 0)
		{
			liste_hostile[i].vitesse.x = rand() % 10 + 1;
			liste_hostile[i].cx = 0 - liste_hostile[i].rayon - (rand() % 50);
			liste_hostile[i].vitesse.y = 0;
			liste_hostile[i].cy = rand() % getHeight() - 50;
			if(liste_hostile[i].cy < liste_hostile[i].rayon)
			{
				liste_hostile[i].cy = liste_hostile[i].rayon + rand() % 10;
			}
		}
		else if(level == 1)
		{
			if(i < nbHostile / 2)
			{
				liste_hostile[i].vitesse.x = rand() % 10 + 1;
				liste_hostile[i].cx = 0 - liste_hostile[i].rayon - (rand() % 50);
				liste_hostile[i].vitesse.y = 0;
				liste_hostile[i].cy = rand() % getHeight() - 50;
				if(liste_hostile[i].cy < liste_hostile[i].rayon)
				{
					liste_hostile[i].cy = liste_hostile[i].rayon + rand() % 10;
				}
			}
			else
			{
				liste_hostile[i].vitesse.x = -(rand() % 10 + 1);
				liste_hostile[i].cx = getWidth() + liste_hostile[i].rayon + (rand() % 50);
				liste_hostile[i].vitesse.y = 0;
				liste_hostile[i].cy = rand() % getHeight() - 50;
				if(liste_hostile[i].cy < liste_hostile[i].rayon)
				{
					liste_hostile[i].cy = liste_hostile[i].rayon + rand() % 10;
				}
			}
		}
		else if(level == 2)
		{
			if(i < nbHostile / 3)
			{
				liste_hostile[i].vitesse.x = rand() % 10 + 1;
				liste_hostile[i].cx = 0 - liste_hostile[i].rayon - (rand() % 50);
				liste_hostile[i].vitesse.y = 0;
				liste_hostile[i].cy = rand() % getHeight() - 50;
				if(liste_hostile[i].cy < liste_hostile[i].rayon)
				{
					liste_hostile[i].cy = liste_hostile[i].rayon + rand() % 10;
				}
			}
			else if(i < nbHostile / 2)
			{
				liste_hostile[i].vitesse.x = -(rand() % 10 + 1);
				liste_hostile[i].cx = getWidth() + liste_hostile[i].rayon + (rand() % 50);
				liste_hostile[i].vitesse.y = 0;
				liste_hostile[i].cy = rand() % getHeight() - 50;
				if(liste_hostile[i].cy < liste_hostile[i].rayon)
				{
					liste_hostile[i].cy = liste_hostile[i].rayon + rand() % 10;
				}
			}
			else
			{
				liste_hostile[i].vitesse.x = 0;
				liste_hostile[i].vitesse.y = rand() % 10 + 2;
				liste_hostile[i].cx = rand() % getWidth() ;
				if(liste_hostile[i].cx < liste_hostile[i].rayon)
				{
					liste_hostile[i].cx = liste_hostile[i].rayon * getScaleX();
				}
				else if(liste_hostile[i].cx > getWidth() - liste_hostile[i].rayon)
				{
					liste_hostile[i].cx = getWidth() - liste_hostile[i].rayon;
				}
				liste_hostile[i].cy = -20 * getScaleY();
			}
		}

		liste_hostile[i].couleur = getRouge(255);

		liste_hostile[i].collider.x = liste_hostile[i].cx - liste_hostile[i].rayon;
		liste_hostile[i].collider.y = liste_hostile[i].cy - liste_hostile[i].rayon;
		liste_hostile[i].collider.w = liste_hostile[i].rayon * 2;
		liste_hostile[i].collider.h = liste_hostile[i].rayon * 2;
	}
}

static void updateHostile(void)
{
	for(int i = 0; i < nbHostile; i++)
	{
		liste_hostile[i].cx += liste_hostile[i].vitesse.x;
		liste_hostile[i].cy += liste_hostile[i].vitesse.y;
		liste_hostile[i].collider.x += liste_hostile[i].vitesse.x;
		liste_hostile[i].collider.y += liste_hostile[i].vitesse.y;

		if(liste_hostile[i].vitesse.x < 0 && liste_hostile[i].cx + liste_hostile[i].rayon < 0)
		{
			liste_hostile[i].vitesse.x = -(rand() % 10 + 1);
			liste_hostile[i].rayon = rand() % 5 + 5;
			liste_hostile[i].cx = getWidth() + liste_hostile[i].rayon + (rand() % 50);
			liste_hostile[i].cy = rand() % getHeight() - 50;
			if(liste_hostile[i].cy < liste_hostile[i].rayon)
			{
				liste_hostile[i].cy = liste_hostile[i].rayon + rand() % 10;
			}
			liste_hostile[i].collider.x = liste_hostile[i].cx - liste_hostile[i].rayon;
			liste_hostile[i].collider.y = liste_hostile[i].cy - liste_hostile[i].rayon;
			liste_hostile[i].collider.w = liste_hostile[i].rayon * 2;
			liste_hostile[i].collider.h = liste_hostile[i].rayon * 2;
		}
		else if(liste_hostile[i].vitesse.x > 0 && liste_hostile[i].cx - liste_hostile[i].rayon > getWidth())
		{
			liste_hostile[i].vitesse.x = rand() % 10 + 1;
			liste_hostile[i].rayon = rand() % 5 + 5;
			liste_hostile[i].cx = 0 - liste_hostile[i].rayon - (rand() % 50);
			liste_hostile[i].cy = rand() % getHeight() - 50;
			if(liste_hostile[i].cy < liste_hostile[i].rayon)
			{
				liste_hostile[i].cy = liste_hostile[i].rayon + rand() % 10;
			}
			liste_hostile[i].collider.x = liste_hostile[i].cx - liste_hostile[i].rayon;
			liste_hostile[i].collider.y = liste_hostile[i].cy - liste_hostile[i].rayon;
			liste_hostile[i].collider.w = liste_hostile[i].rayon * 2;
			liste_hostile[i].collider.h = liste_hostile[i].rayon * 2;
		}
		if(liste_hostile[i].vitesse.y != 0 && liste_hostile[i].cy > getHeight() + liste_hostile[i].rayon)
		{
			liste_hostile[i].vitesse.x = 0;
			liste_hostile[i].vitesse.y = rand() % 10 + 2;
			liste_hostile[i].cx = rand() % getWidth();
			if(liste_hostile[i].cx < liste_hostile[i].rayon)
			{
				liste_hostile[i].cx = liste_hostile[i].rayon;
			}
			else if(liste_hostile[i].cx > getWidth() - liste_hostile[i].rayon)
			{
				liste_hostile[i].cx = getWidth() - liste_hostile[i].rayon;
			}
			liste_hostile[i].cy = -20 * getScaleY();
			liste_hostile[i].collider.x = liste_hostile[i].cx - liste_hostile[i].rayon;
			liste_hostile[i].collider.y = liste_hostile[i].cy - liste_hostile[i].rayon;
			liste_hostile[i].collider.w = liste_hostile[i].rayon * 2;
			liste_hostile[i].collider.h = liste_hostile[i].rayon * 2;
		}
	}
}

static void drawHostile(void)
{
	for(int i = 0; i < nbHostile; i++)
	{
		drawCercle(liste_hostile[i]);
	}
}

static void initMenu(void)
{
	printMenu = SDL_FALSE;
	initGroupeGui(&menu, 0, 1, 2, 1);

	initFontHack();

	if(getLocalisation() == 0)
	{
		//int x, int y, int w, int h, int texture,SDL_Color panelColor, char *ptexte, SDL_Color texteColor, TTF_Font *font, SDL_bool pFill, int event
		menu.liste_bouton[0] = createBouton(10, 300, 200, 50, -1, getNoir(255), "MENU PRINCIPAL", getBlanc(255), getFontHack(), SDL_TRUE, 4);
		menu.liste_bouton[1] = createBouton(10, 350, 200, 50, -1, getNoir(255), "RETOUR", getBlanc(255), getFontHack(), SDL_TRUE, 11);

		//int x, int y, int w, int h, int texture, SDL_Color color, SDL_bool pFill
		menu.liste_panel[0] = createPanel(0, 290, 220, 120, -1, getBleu(255), SDL_TRUE);

		//int x, int y, int w, int h, char *texte, SDL_Color color, TTF_Font *font
		menu.liste_texte[0] = createTexte(300, 50, 200, 50, "PAUSE ACTIVE", getBlanc(255), getFontHack());
	}
	else
	{
		//int x, int y, int w, int h, int texture,SDL_Color panelColor, char *ptexte, SDL_Color texteColor, TTF_Font *font, SDL_bool pFill, int event
		menu.liste_bouton[0] = createBouton(10, 300, 200, 50, -1, getNoir(255), "MAIN MENU", getBlanc(255), getFontHack(), SDL_TRUE, 4);
		menu.liste_bouton[1] = createBouton(10, 350, 200, 50, -1, getNoir(255), "RETURN", getBlanc(255), getFontHack(), SDL_TRUE, 11);

		//int x, int y, int w, int h, int texture, SDL_Color color, SDL_bool pFill
		menu.liste_panel[0] = createPanel(0, 290, 220, 120, -1, getBleu(255), SDL_TRUE);

		//int x, int y, int w, int h, char *texte, SDL_Color color, TTF_Font *font
		menu.liste_texte[0] = createTexte(300, 50, 200, 50, "PAUSE ON", getBlanc(255), getFontHack());
	}

	cleanFontHack();
}

void setPrintMenuGame(SDL_bool active)
{
	if(isWin == SDL_FALSE)
	{
		printMenu = active;
	}
}

SDL_bool isPrintMenuGame(void)
{
	return printMenu;
}

SDL_bool collideWithHostile(SDL_Rect r2)
{
	//absolue(objectif.cx, x) < objectif.rayon && absolue(objectif.cy, y) < objectif.rayon
	SDL_bool collider = SDL_FALSE;
	int  i = 0;

	while(collider == SDL_FALSE && i < nbHostile)
	{		
		collider = collide(liste_hostile[i].collider, r2);
		i++;
	}

	return collider;
}

SDL_bool isGameWin(void)
{
	return isWin;
}

void nextLevel(void)
{
	playFx(4);
	currentLevel++;

	initHostile(currentLevel);
	setObjectif();
	SDL_ShowCursor(SDL_DISABLE);
	resetPlayer();
}