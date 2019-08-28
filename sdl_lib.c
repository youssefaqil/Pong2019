#include "main.h"

//SDL: Simple DirectMediaLayer : est une bibliothéque multimédia à pour but de créer des interface graphiques.
//Init_SDL : Utilisez cette fonction pour initialiser la bibliothèque SDL. Ceci doit être appelé avant d'utiliser la plupart des autres fonctions SDL
int		init_sdl(t_affichage *affichage, t_data *data)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Can not initialize the SDL : %s\n", SDL_GetError());
		exit(1);
	}
	  //SDL_SetVideoMode : SDL_SetVideoMode(int width, int height, int bpp(Bytes Per Pixel|Nombre d'Octets par pixel), Uint32 flags)
	  // SDL_HWSURFACE | SDL_DOUBLEBUF : des flags supportent pour la SDL_surface 
	  // SDL_HWSURFACE met la surface dans la mémoire de la carte graphique
	  //affichage->screen : struct t_affichage affecte les paramètres à objet screen 
	affichage->screen = SDL_SetVideoMode(TAILLE_L,TAILLE_H,32,SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (affichage->screen == NULL) {
		SDL_Quit();
		fprintf(stderr, "Can not create SDL window : %s\n", SDL_GetError());
		exit(1);
	}
     
	 //SDL_WM_SetCaption : Définit le titre de la fenêtre et le nom de l'icône.
	SDL_WM_SetCaption("Pong", NULL);

//SDL_GetKeyState:  Obtenir un instantané de l'état actuel du clavier 
                  //: cette fonction vous donne l'état actuel une fois tous les événements traités. 
				   //Ainsi, si une touche ou un bouton a été enfoncé et relâché avant que vous ne traitiez les événements, 
				   //l'état enfoncé n'apparaîtra jamais dans les appels Getstate.
	affichage->keystates = SDL_GetKeyState(NULL);

	affichage->line = SDL_CreateRGBSurface(SDL_HWSURFACE,LINE_W,TAILLE_H,32,0,0,0,0);
	SDL_FillRect(affichage->line,NULL,SDL_MapRGB(affichage->line->format,100,100,100));

	affichage->rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE,BAR_W,BAR_H,32,0,0,0,0);
	SDL_FillRect(affichage->rectangle,NULL,SDL_MapRGB(affichage->rectangle->format,255,255,255));

	affichage->player1 = SDL_CreateRGBSurface(SDL_HWSURFACE,BAR_W,BAR_H,32,0,0,0,0);
	SDL_FillRect(affichage->player1,NULL,SDL_MapRGB(affichage->player1->format,255,255,255));

	affichage->player2 = SDL_CreateRGBSurface(SDL_HWSURFACE,BAR_W,BAR_H,32,0,0,0,0);
	SDL_FillRect(affichage->player2,NULL,SDL_MapRGB(affichage->player2->format,255,255,255));

	affichage->ball = SDL_CreateRGBSurface(SDL_HWSURFACE,BALL_TAILLE,BALL_TAILLE,32,0,0,0,0);
	SDL_FillRect(affichage->ball,NULL,SDL_MapRGB(affichage->ball->format,255,255,255));

	
	if (TTF_Init() < 0) {
		fprintf(stderr, "Impossible to initialize SDL_TTF : %s\n", TTF_GetError());
		return 1;
	}

	affichage->font = TTF_OpenFont("Loma-Bold.ttf", 30);
	affichage->font1 = TTF_OpenFont("Loma-Bold.ttf", 25);
	affichage->font2 = TTF_OpenFont("Loma-Bold.ttf", 20);

	affichage->couleur.r = 255;
	affichage->couleur.g = 255;
	affichage->couleur.b = 255;

	affichage->positionScore1.x = TAILLE_L/2 - 100 ;
	affichage->positionScore1.y = SCORE_D ;
	affichage->positionScore2.x = TAILLE_L/2 + 100 ;
	affichage->positionScore2.y = SCORE_D ;
	affichage->positionGameStart.x = TAILLE_L/2 - 45;
	affichage->positionGameStart.y = TAILLE_H/2 - 60;
	affichage->positionPName1.x = 10;
	affichage->positionPName1.y = 0;
	affichage->positionPName2.x = TAILLE_L - 110;
	affichage->positionPName2.y = 0;
	affichage->positionGameOver.x = TAILLE_L/2 - 85;
	affichage->positionGameOver.y = TAILLE_H/2 - 60;
	affichage->positionGameWin.x = TAILLE_L/2 - 80;
	affichage->positionGameWin.y = TAILLE_H/2 - 20;
	affichage->positionGameAgain.x = TAILLE_L/2 - 45;
	affichage->positionGameAgain.y = TAILLE_H/2 + 20;
	affichage->positionGameExit.x = TAILLE_L/2 - 40;
	affichage->positionGameExit.y = TAILLE_H/2 + 50;

	SDL_EnableKeyRepeat(10, 50);

	
	affichage->positionP1.x = 50;
	affichage->positionP1.y = TAILLE_H/2 - BAR_H/2;

	data->xjoueur1 = affichage->positionP1.x;
	data->yjoueur1 = affichage->positionP1.y;

	affichage->positionP2.x = TAILLE_L - 50 - BAR_W;
	affichage->positionP2.y = TAILLE_H/2 - BAR_H/2;

	data->xjoueur2 = affichage->positionP2.x;
	data->yjoueur2 = affichage->positionP2.y;

	affichage->vitesseX = 0;
	affichage->vitesseY = 0;

	reset_ball(&affichage->positionBall,&data->vitesseX, &data->vitesseY);

	data->ball_x = affichage->positionBall.x;
	data->ball_y = affichage->positionBall.y;

	affichage->positionLigne.x = TAILLE_L/2 - LINE_W/2;
	affichage->positionLigne.y = 0;

	return (0);
}

//La communication entre client server 
int		sdl_start(t_affichage *affichage, t_data *data)
{
	while (1) {
		if (data->type == c) {
			wait_event_client(affichage, data);
			draw_game(affichage, data); 
		}
		if (data->type == SERVER_TYPE) {
			wait_event_server(affichage, data);
			draw_game(affichage, data); 
		}
	}

	SDL_Quit();
	return (0);
}

/*to affichage the game*/
void 	draw_game(t_affichage *affichage, t_data *data)
{
	char		scoreP1_s[10];
	char		scoreP2_s[10];

	sprintf(scoreP1_s,"%d",data->scorep1);
	sprintf(scoreP2_s,"%d",data->scorep2);

	
	affichage->score1 = TTF_RenderText_Solid(affichage->font, scoreP1_s, affichage->couleur);
	affichage->score2 = TTF_RenderText_Solid(affichage->font, scoreP2_s, affichage->couleur);
	affichage->gameStart = TTF_RenderText_Solid(affichage->font, "Start (S)", affichage->couleur);  
	affichage->pName1 = TTF_RenderText_Solid(affichage->font, "Player1", affichage->couleur);
	affichage->pName2 = TTF_RenderText_Solid(affichage->font, "Player2", affichage->couleur);

	SDL_FillRect(affichage->screen,NULL,SDL_MapRGB(affichage->screen->format,0,0,0));
	SDL_BlitSurface(affichage->line, NULL,affichage->screen,&affichage->positionLigne);

	print_ball(data, affichage);
	SDL_BlitSurface(affichage->score1, NULL, affichage->screen,&affichage->positionScore1);
	SDL_BlitSurface(affichage->score2, NULL, affichage->screen,&affichage->positionScore2);

	SDL_BlitSurface(affichage->pName1, NULL, affichage->screen,&affichage->positionPName1);
	SDL_BlitSurface(affichage->pName2, NULL, affichage->screen,&affichage->positionPName2);

	
	if(data->etatPartie == STATUT_STOP){
			SDL_BlitSurface(affichage->gameStart, NULL, affichage->screen,&affichage->positionGameStart); 
	}

	
	if(data->scorep1 == data->scoreFinal || data->scorep2 == data->scoreFinal) {
		affichage->gameOver = TTF_RenderText_Solid(affichage->font, "Game Over!", affichage->couleur);  
		SDL_BlitSurface(affichage->gameOver, NULL, affichage->screen, &affichage->positionGameOver); 

		if (data->scorep1 == data->scoreFinal) {
			affichage->gameWin = TTF_RenderText_Solid(affichage->font1, "Player1 WIN", affichage->couleur);  
		} else {
			affichage->gameWin = TTF_RenderText_Solid(affichage->font1, "Player2 WIN", affichage->couleur);  
		}
		SDL_BlitSurface(affichage->gameWin, NULL, affichage->screen, &affichage->positionGameWin); 

		affichage->gameAgain = TTF_RenderText_Solid(affichage->font2, "encore (A)", affichage->couleur);
		SDL_BlitSurface(affichage->gameAgain, NULL, affichage->screen, &affichage->positionGameAgain);

		affichage->gameExit = TTF_RenderText_Solid(affichage->font2, "Exit (E)", affichage->couleur);
		SDL_BlitSurface(affichage->gameExit, NULL, affichage->screen, &affichage->positionGameExit);

		data->etatPartie = STATUT_GAME_OVER; 
	}

	print_joueurs(data, affichage);
	SDL_Flip(affichage->screen);
	SDL_Delay(10);
}

void wait_event_client(t_affichage *affichage, t_data *data)
{
	SDL_PollEvent(&affichage->event);
	if (affichage->event.type == SDL_QUIT) {
		SDL_Quit();
		exit(0);
	} else if (affichage->event.type == SDL_KEYDOWN && affichage->event.key.keysym.sym == SDLK_ESCAPE) {
		SDL_Quit();
		exit(0);
	} else if (affichage->event.type == SDL_KEYDOWN && affichage->event.key.keysym.sym == SDLK_s) {
		if(data->etatPartie == STATUT_STOP){
			dprintf(data->socket, "start\n");
			data->etatPartie = STATUT_DEBUT;
		}
	} else if(affichage->event.type == SDL_KEYDOWN && affichage->event.key.keysym.sym == SDLK_e) {
		if(data->etatPartie == STATUT_GAME_OVER){
			dprintf(data->socket, "exit\n");
			sleep(1);
			SDL_Quit();
			exit(0);
		}
	} else if (affichage->event.type == SDL_KEYDOWN && affichage->event.key.keysym.sym == SDLK_a) {
		if(data->etatPartie == STATUT_GAME_OVER){
			dprintf(data->socket, "encore\n");
			data->scorep1 = 0; 
			data->scorep2 = 0; 
			data->etatPartie = STATUT_DEBUT;
		}
	} else if (affichage->event.type == SDL_KEYDOWN && affichage->event.key.keysym.sym == SDLK_UP && data->etatPartie != STATUT_STOP) {
		dprintf(data->socket, "UP 1\n");
	} else if (affichage->event.type == SDL_KEYDOWN && affichage->event.key.keysym.sym == SDLK_DOWN && data->etatPartie != STATUT_STOP) {
		dprintf(data->socket, "DOWN 1\n");
	} else {
		dprintf(data->socket, "PLOP");
	}

	SDL_Delay(20);
}

void	wait_event_server(t_affichage *affichage, t_data *data)
{
	SDL_PollEvent(&affichage->event);
	if (affichage->event.type == SDL_QUIT) {
		SDL_Quit();
		exit(0);
	} else if (affichage->event.type == SDL_KEYDOWN && affichage->event.key.keysym.sym == SDLK_ESCAPE) {
		SDL_Quit();
		exit(0);
	} else if (affichage->event.type == SDL_KEYDOWN && affichage->event.key.keysym.sym == SDLK_s) {
		if(data->etatPartie == STATUT_STOP){
			data->etatPartie = STATUT_DEBUT;
		}
	} else if (affichage->event.type == SDL_KEYDOWN && affichage->event.key.keysym.sym == SDLK_e) {
		if(data->etatPartie == STATUT_GAME_OVER){
			SDL_Quit();
			exit(0);
		}
	} else if (affichage->event.type == SDL_KEYDOWN && affichage->event.key.keysym.sym == SDLK_a) {
		if(data->etatPartie == STATUT_GAME_OVER){
			data->scorep1 = 0;
			data->scorep2 = 0;
			data->etatPartie = STATUT_DEBUT;
		}
	}	else if (affichage->event.type == SDL_KEYDOWN && affichage->event.key.keysym.sym == SDLK_UP && data->etatPartie != STATUT_STOP) {
		if(data->yjoueur2 >= 5) {
			data->yjoueur2 -= 5;
		}
	} else if (affichage->event.type == SDL_KEYDOWN && affichage->event.key.keysym.sym == SDLK_DOWN && data->etatPartie != STATUT_STOP) {
		if(data->yjoueur2 <= TAILLE_H -BAR_H -5) {
			data->yjoueur2 += 5;
		}
	}
	SDL_Delay(20);
}

  
void	reset_ball(SDL_Rect *position, int *x, int *y)
{
	position->x = BALL_X;
	position->y = BALL_Y;
	*x = BALL_VITESSE;
	*y = BALL_VITESSE;
}

void	balleInitialisation(int *pos_x,int *pos_y,int *x,int *y)
{
	*pos_x = BALL_X;
	*pos_y = BALL_Y;
	*x = BALL_VITESSE;
	*y = BALL_VITESSE;
}

void    print_ball(t_data *data, t_affichage *affichage)
{
	affichage->positionBall.x = data->ball_x;
	affichage->positionBall.y = data->ball_y;
	SDL_BlitSurface(affichage->ball,NULL,affichage->screen,&affichage->positionBall);
}

void    print_joueurs(t_data *data, t_affichage *affichage)
{
	affichage->positionP1.x = data->xjoueur1;
	affichage->positionP1.y = data->yjoueur1;
	SDL_FillRect(affichage->player1, NULL, SDL_MapRGB(affichage->screen->format, 240, 5, 0));
	SDL_BlitSurface(affichage->player1, NULL, affichage->screen, &affichage->positionP1);

	affichage->positionP2.x = data->xjoueur2;
	affichage->positionP2.y = data->yjoueur2;
	SDL_FillRect(affichage->player2, NULL, SDL_MapRGB(affichage->screen->format, 240, 5, 0));
	SDL_BlitSurface(affichage->player2, NULL, affichage->screen, &affichage->positionP2);
}
