#include "prototype.h"

static Player player;
static int maxCercle;

static Cercle *liste_cercle;
static int nbCercle;
static void launchExplosion(void);
static void cleanExplosion(void);

static void movePlayer(Input *input);
static GroupeGui notAlive;

void initPlayer(void)
{
	maxCercle = 0;

	player.maxSpeed = 5;
	player.cercle.vitesse.x = 0;
	player.cercle.vitesse.y = 0;

	player.cercle.couleur = getVert(255);
	player.cercle.rayon = 5;
	player.cercle.cx = 400 * getScaleX();
	player.cercle.cy = 590 * getScaleY();

	player.timer.ms = 30;
	player.timer.previousTime = SDL_GetTicks();
	player.timer.currentTime = 0;

	player.alive = SDL_TRUE;

	player.cercle.collider.x = player.cercle.cx - player.cercle.rayon;
	player.cercle.collider.y = player.cercle.cy - player.cercle.rayon;
	player.cercle.collider.w = player.cercle.rayon * 2;
	player.cercle.collider.h = player.cercle.rayon * 2;

	initGroupeGui(&notAlive, 0, 1, 0, 0);

	initFontHack();

	if(getLocalisation() == 0)
	{
		//int x, int y, int w, int h, char *texte, SDL_Color color, TTF_Font *font
		notAlive.liste_texte[0] = createTexte(200, 200, 400, 200, "APPUYER SUR ESPACE POUR REVIVRE", getBlanc(255), getFontHack());
	}
	else
	{
		//int x, int y, int w, int h, char *texte, SDL_Color color, TTF_Font *font
		notAlive.liste_texte[0] = createTexte(200, 200, 400, 200, "PRESS SPACE TO REVIVE", getBlanc(255), getFontHack());
	}

	cleanFontHack();
}	

void updatePlayer(Input *input)
{
	if(maxCercle > 90)
	{
		player.timer.currentTime = SDL_GetTicks();

		if(player.timer.currentTime >= player.timer.previousTime + player.timer.ms)
		{
			if(input->space && player.alive == SDL_FALSE)
			{
				resetPlayer();
				cleanExplosion();
			}

			movePlayer(input);

			player.cercle.cx += player.cercle.vitesse.x;
			player.cercle.cy += player.cercle.vitesse.y;
			player.cercle.collider.x += player.cercle.vitesse.x;
			player.cercle.collider.y += player.cercle.vitesse.y;

			if(player.cercle.cx - player.cercle.rayon < 0)
			{
				player.cercle.cx = player.cercle.rayon;
				player.cercle.collider.x = player.cercle.cx - player.cercle.rayon;
			}
			else if(player.cercle.cx + player.cercle.rayon > getWidth())
			{
				player.cercle.cx = getWidth() - player.cercle.rayon;
				player.cercle.collider.x = player.cercle.cx - player.cercle.rayon;
			}
			if(player.cercle.cy - player.cercle.rayon < 0)
			{
				player.cercle.cy = player.cercle.rayon;
				player.cercle.collider.y = player.cercle.cy - player.cercle.rayon;
			}
			else if(player.cercle.cy + player.cercle.rayon > getHeight())
			{
				player.cercle.cy = getHeight() - player.cercle.rayon;
				player.cercle.collider.y = player.cercle.cy - player.cercle.rayon;
			}

			if(player.alive)
			{
				if(collideWithHostile(player.cercle.collider))
				{
					playFx(0);
					launchExplosion();
				}
			}
			else
			{
				for(int i = 0; i < nbCercle; i++)
				{
					liste_cercle[i].cx += liste_cercle[i].vitesse.x;
					liste_cercle[i].cy += liste_cercle[i].vitesse.y;
				}
			}

			player.timer.previousTime = player.timer.currentTime;
		}
	}
}

void drawPlayer(void)
{
	if(player.alive)
	{
		if(maxCercle > 90)
		{
			for(int i = 0; i < 4; i++)
			{
				drawDisque(player.cercle);
			}
		}
		else if(maxCercle < 90 && maxCercle > 1)
		{
			drawCercle(player.cercle);
		}
	}
	else
	{
		for(int i = 0; i < nbCercle; i++)
		{
			drawDisque(liste_cercle[i]);
		}
		drawGroupeGui(&notAlive);
	}

}

void cleanPlayer(void)
{
	SDL_Log("CLEAN PLAYER EN COURS\n");
	cleanGroupeGui(&notAlive);
	SDL_Log("CLEAN PLAYER TERMINER\n");
}

void addCercle(void)
{
	maxCercle++;
}

SDL_Rect getPlayerCollider(void)
{
	return player.cercle.collider;
}

SDL_bool isPlayerAlive(void)
{
	return player.alive;
}

int getMaxCercle(void)
{
	return maxCercle;
}

int getPlayerX(void)
{
	return player.cercle.cx;
}

int getPlayerY(void)
{
	return player.cercle.cy;
}

int getPlayerRayon(void)
{
	return player.cercle.rayon;
}

static void cleanExplosion(void)
{
	nbCercle = 0;
	if(liste_cercle != NULL)
	{
		free(liste_cercle);
		liste_cercle = NULL;
	}
}

static void launchExplosion(void)
{
	player.alive = SDL_FALSE;

	nbCercle = 50;
	liste_cercle = malloc(sizeof(Cercle) * nbCercle);
	if(liste_cercle == NULL)
	{
		fprintf(stderr, "malloc; %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < nbCercle; i++)
	{
		liste_cercle[i].cx = player.cercle.cx;
		liste_cercle[i].cy = player.cercle.cy;
		liste_cercle[i].rayon = 1;

		if(rand() % 2 == 1)
		{
			liste_cercle[i].vitesse.x = rand() % 10 + 5;
		}
		else
		{
			liste_cercle[i].vitesse.x = -(rand() % 10 + 5);
		}
		if(rand() % 2 == 1)
		{
			liste_cercle[i].vitesse.y = rand() % 10 + 5;
		}
		else
		{
			liste_cercle[i].vitesse.y = -(rand() % 10 + 5);
		}
		liste_cercle[i].couleur = getVert(255);
	}
}

void resetPlayer(void)
{
	playFx(1);
	player.alive = SDL_TRUE;
	player.cercle.cx = 400 * getScaleX();
	player.cercle.cy = 590 * getScaleY();
	player.cercle.collider.x = 400 * getScaleX() - player.cercle.rayon;
	player.cercle.collider.y = 590 * getScaleY() - player.cercle.rayon;
}

static void movePlayer(Input *input)
{
	if(input->q)
	{
		player.cercle.vitesse.x -= 1;
		if(player.cercle.vitesse.x < -player.maxSpeed)
		{
			player.cercle.vitesse.x = -player.maxSpeed;
		}
	}
	else if(input->d)
	{
		player.cercle.vitesse.x += 1;
		if(player.cercle.vitesse.x > player.maxSpeed)
		{
			player.cercle.vitesse.x = player.maxSpeed;
		}
	}
	else
	{
		if(player.cercle.vitesse.x > 0)
		{
			player.cercle.vitesse.x -= 1;
			if(player.cercle.vitesse.x < 0)
			{
				player.cercle.vitesse.x = 0;
			}
		}
		else if(player.cercle.vitesse.x < 0)
		{
			player.cercle.vitesse.x += 1;
			if(player.cercle.vitesse.x > 0)
			{
				player.cercle.vitesse.x = 0;
			}
		}
	}

	if(input->z)
	{
		player.cercle.vitesse.y -= 1;
		if(player.cercle.vitesse.y < -player.maxSpeed)
		{
			player.cercle.vitesse.y = -player.maxSpeed;
		}
	}
	else if(input->s)
	{
		player.cercle.vitesse.y += 1;
		if(player.cercle.vitesse.y > player.maxSpeed)
		{
			player.cercle.vitesse.y = player.maxSpeed;
		}
	}
	else
	{
		if(player.cercle.vitesse.y > 0)
		{
			player.cercle.vitesse.y -= 1;
			if(player.cercle.vitesse.y < 0)
			{
				player.cercle.vitesse.y = 0;
			}
		}
		else if(player.cercle.vitesse.y < 0)
		{
			player.cercle.vitesse.y += 1;
			if(player.cercle.vitesse.y > 0)
			{
				player.cercle.vitesse.y = 0;
			}
		}
	}
}