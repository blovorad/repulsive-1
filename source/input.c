#include "prototype.h"

//CATALOGUE DES INPUTS ASSEZ CLAIR A LA LECTURE
void getInput(Input *input)
{
	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT :
				exit(EXIT_SUCCESS);
				break;
				
			case SDL_MOUSEBUTTONDOWN :
				if(event.button.button == SDL_BUTTON_LEFT)
				{
					actualiseLeftButton(1);
				}
				if(event.button.button == SDL_BUTTON_RIGHT)
				{
					actualiseRightButton(1);
				}
				break;

			case SDL_MOUSEBUTTONUP :
				if(event.button.button == SDL_BUTTON_LEFT)
				{
					actualiseLeftButton(2);
				}
				if(event.button.button == SDL_BUTTON_RIGHT)
				{
					actualiseRightButton(2);
				}
				break;
			case SDL_KEYDOWN :
				if(event.key.keysym.sym == SDLK_ESCAPE)
				{
					if(isActiveGame())
					{
						if(isPrintMenuGame())
						{
							SDL_ShowCursor(SDL_DISABLE);
							setPrintMenuGame(SDL_FALSE);
						}
						else
						{
							SDL_ShowCursor(SDL_ENABLE);
							setPrintMenuGame(SDL_TRUE);
						}
					}
				}
				if(getLocalisation() == 0)
				{
					if(event.key.keysym.sym == SDLK_z)
					{
						input->z = SDL_TRUE;
					}
					if(event.key.keysym.sym == SDLK_s)
					{
						input->s = SDL_TRUE;
					}
					if(event.key.keysym.sym == SDLK_q)
					{
						input->q = SDL_TRUE;
					}
					if(event.key.keysym.sym == SDLK_d)
					{
						input->d = SDL_TRUE;
					}
					if(event.key.keysym.sym == SDLK_SPACE)
					{
						input->space = SDL_TRUE;
					}
				}
				else
				{
					if(event.key.keysym.sym == SDLK_w)
					{
						input->z = SDL_TRUE;
					}
					if(event.key.keysym.sym == SDLK_s)
					{
						input->s = SDL_TRUE;
					}
					if(event.key.keysym.sym == SDLK_a)
					{
						input->q = SDL_TRUE;
					}
					if(event.key.keysym.sym == SDLK_d)
					{
						input->d = SDL_TRUE;
					}
					if(event.key.keysym.sym == SDLK_SPACE)
					{
						input->space = SDL_TRUE;
					}
				}
				break;

			case SDL_KEYUP :
				if(getLocalisation() == 0)
				{		
					if(event.key.keysym.sym == SDLK_z)
					{
						input->z = SDL_FALSE;
					}
					if(event.key.keysym.sym == SDLK_s)
					{
						input->s = SDL_FALSE;
					}
					if(event.key.keysym.sym == SDLK_q)
					{
						input->q = SDL_FALSE;
					}
					if(event.key.keysym.sym == SDLK_d)
					{
						input->d = SDL_FALSE;
					}
					if(event.key.keysym.sym == SDLK_SPACE)
					{
						input->space = SDL_FALSE;
					}
				}
				else
				{
					if(event.key.keysym.sym == SDLK_w)
					{
						input->z = SDL_FALSE;
					}
					if(event.key.keysym.sym == SDLK_s)
					{
						input->s = SDL_FALSE;
					}
					if(event.key.keysym.sym == SDLK_a)
					{
						input->q = SDL_FALSE;
					}
					if(event.key.keysym.sym == SDLK_d)
					{
						input->d = SDL_FALSE;
					}
					if(event.key.keysym.sym == SDLK_SPACE)
					{
						input->space = SDL_FALSE;
					}
				}
				break;

			default :
				break;
		}
	}
}