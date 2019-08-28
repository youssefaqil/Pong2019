#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <pthread.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

//les prototypes des fonctions.
#define PORT 5555

#define SERVER_TYPE 0
#define CLIENT_TYPE 1

#define STATUT_STOP 0
#define STATUT_DEBUT 1
#define STATUT_GAME_OVER 2

#define	BUFFER_SIZE 50

#define TAILLE_L 800
#define TAILLE_H 480

#define BAR_W 15
#define BAR_H 120

#define LINE_W 2

#define BALL_X 100
#define BALL_Y 100
#define BALL_TAILLE 10
#define BALL_VITESSE 5

#define SCORE_D 30

//Une structure est un type de variable
//Une structure est composée de « sous-variables » qui sont en général des variables de type de base commeintetdouble, mais aussi des tableaux.
typedef struct	s_data
{
	struct sockaddr_in	sin;
	int     type;
	int     socket;
	char    *game;
	int		hauteur;
	int		larg;
	int		scorep1;
	int		scorep2;
	int 	ball_x;
	int 	ball_y;
	int 	vitesseX;
	int 	vitesseY;

	int	    xjoueur1;
	int		yjoueur1;
	int		xjoueur2;
	int		yjoueur2;

	int 	etatPartie;
	char* 	IP;
	int 	scoreFinal;
}		t_data;

typedef	struct  s_affichage
{
	SDL_Surface		*screen;
	SDL_Surface		*ball;
	SDL_Surface		*rectangle;    
	SDL_Surface		*player2;    		
	SDL_Surface		*player1;
	SDL_Surface		*line;

	Uint8	*keystates;

	SDL_Rect	positionP1;
	SDL_Rect	positionP2;
	SDL_Rect	positionBall;
	SDL_Rect	positionLigne;
	SDL_Rect	positionScore1;
	SDL_Rect	positionScore2;
	SDL_Rect	positionGameStart;
	SDL_Rect	positionPName1;
	SDL_Rect	positionPName2;
	SDL_Rect	positionGameOver;
	SDL_Rect	positionGameWin;
	SDL_Rect	positionGameAgain;
	SDL_Rect	positionGameExit;

	SDL_Event	event;

	int 		vitesseX;
	int 		vitesseY;

	TTF_Font 	*font;
	TTF_Font 	*font1;
	TTF_Font 	*font2;

	SDL_Color 	couleur;
	SDL_Surface *score1;
	SDL_Surface *score2;
	SDL_Surface *gameStart;
	SDL_Surface *gameOver;
	SDL_Surface *gameWin;
	SDL_Surface *gameAgain;
	SDL_Surface *gameExit;
	SDL_Surface *pName1;
	SDL_Surface *pName2;

	int	larg;
	int	hauteur;
} 	t_affichage;

typedef struct  s_server
{
	struct sockaddr_in	sin;
	int	fd_max;
	int	fd_socket;
	int	fd_op;
} 	t_server;


int   	main();
void  	*affichage(void *arg);
int   	init_sdl(t_affichage *affichage, t_data *data);
int   	sdl_start(t_affichage *affichage, t_data *data);
void	wait_event_client(t_affichage *affichage, t_data *data);
void  	draw_game(t_affichage *affichage, t_data *data);
void	wait_event_server(t_affichage *affichage, t_data *data);
void	reset_ball(SDL_Rect *position,int *x,int *y);
void	scores(char *buffer, t_data *data);
void	joueurs(char *buffer, t_data *data);
void	balle(char *buffer, t_data *data);
void    statuts(char *buffer, t_data *data);
char	**transfert(char *str, char sp);
void    print_ball(t_data *data, t_affichage *affichage);
void    print_joueurs(t_data *data, t_affichage *affichage);
int   	lancementClient(char* ip,int scoreFinal);
void	*init_client(void *arg);
void	lectureDonnees(t_data *data);
void	verifCommande(char *buffer, t_data *data);
int   	lancementServeur(char* ip, int scoreFinal);
void	*init_server(void *arg);
void	demarrageServeur(t_server *server, t_data *data);
void	verifClient(char *buffer, t_data *data);
void	balleInitialisation(int *pos_x,int *pos_y,int *x,int *y);

#endif 