#include "main.h"

void verifClient(char *buffer, t_data *data)
{
  char	**jeu;
  int i;

  i = 0;
  jeu = transfert(buffer, '\n');

  while (jeu[i] != NULL) {
    if (strncmp(jeu[i], "start", 5) == 0) {
      data->etatPartie = STATUT_DEBUT;
    }
    if (strncmp(jeu[i], "again", 5) == 0) {
      data->scorep1 = 0;
      data->scorep2 = 0;
      data->etatPartie = STATUT_DEBUT;
    } else if (strncmp(jeu[i], "exit", 4) == 0) {
      SDL_Quit();
      exit(0);
    } else if (strncmp(jeu[i], "UP 1", 4) == 0) {
      if (data->yjoueur1 >= 5) {
        data->yjoueur1 -=5;
      }
    } else if (strncmp(jeu[i], "DOWN 1", 6) == 0 ) {
      if(data->yjoueur1 <= TAILLE_H - BAR_H - 5) {
        data->yjoueur1 += 5;
      }
    } else {
      memset(buffer, 0, 1024);
    }
    i++;
  }
  free(jeu);
}


void	demarrageServeur(t_server *server, t_data *data)
{
  struct  sockaddr_in csin;
  size_t  size;
  int cs;
  char  str[1024];
  int rread;
  char  sendc[1024];
  char  scoreP1_s[10];
  char  scoreP2_s[10];
  char  P1_x_s[10];
  char  P1_y_s[10];
  char  P2_x_s[10];
  char  P2_y_s[10];
  char  Ball_x_s[10];
  char	Ball_y_s[10];

  size = sizeof(csin);
  /*
atendre une connexion sur le socket FD.
    Quand une connexion arrive, ouvrez un nouveau socket pour communiquer avec elle,
    définissez * ADDR (qui est * ADDR_LEN octets de long) sur l'adresse de la connexion
    peer et * ADDR_LEN à la longueur réelle de l'adresse et renvoient le
    nouveau descripteur de socket, ou -1 pour les erreurs.
    Cette fonction est un point d’annulation et n’est donc pas marquée avec
    __JETER. * /
  */
  cs = accept(server->fd_socket, (struct sockaddr *)&csin, (socklen_t *)&size);

  if (cs < 0) {
    fprintf(stderr, "Erreur\n");
    exit (1);
  }

  while (1) {
    if ((rread = recv(cs, str, 1024, 0)) < 0) {
      fprintf(stderr, "Erreur de reception\n");
      exit(errno);
    } else {
      str[rread] = '\0';
    }

    verifClient(str, data);

    if(data->etatPartie == STATUT_STOP) {
    	continue;
    }

    if(data->etatPartie < STATUT_GAME_OVER) {

		  memset(sendc, 0, 1024);

      if(data->ball_y <= 0) {
        data->vitesseY = BALL_VITESSE;
      }
      if(data->ball_y >= TAILLE_H) {
        data->vitesseY = -BALL_VITESSE; 
      }

		  
      if(data->ball_x <=0) {
        balleInitialisation(&data->ball_x,&data->ball_y,&data->vitesseX, &data->vitesseY);
        data->scorep2++;
      }
      if(data->ball_x + BALL_TAILLE >TAILLE_L) {
        balleInitialisation(&data->ball_x,&data->ball_y,&data->vitesseX, &data->vitesseY);
        data->scorep1++;
      }

      
      if(data->ball_x <= data->xjoueur1+BAR_W && data->ball_x > data->xjoueur1) {
        if(data->ball_y >= data->yjoueur1 && data->ball_y <= data->yjoueur1 + BAR_H)
        data->vitesseX = BALL_VITESSE;
      }
      
      if(data->ball_x >= data->xjoueur2 &&  data->ball_x < data->xjoueur2 + BAR_W/2) {
        if(data->ball_y >= data->yjoueur2 && data->ball_y <= data->yjoueur2 + BAR_H)
        data->vitesseX = -BALL_VITESSE;
      }

      data->ball_x += data->vitesseX;
      data->ball_y += data->vitesseY;

      
      sprintf(scoreP1_s,"%d",data->scorep1);
      sprintf(scoreP2_s,"%d",data->scorep2);

      strcat(sendc, "SCORE ");
      strcat(sendc, scoreP1_s);
      strcat(sendc, " ");
      strcat(sendc, scoreP2_s);

      sprintf(Ball_x_s,"%d",data->ball_x);
      sprintf(Ball_y_s,"%d",data->ball_y);

      strcat(sendc, "\nBALL ");
      strcat(sendc, Ball_x_s);
      strcat(sendc, " ");
      strcat(sendc, Ball_y_s);

      sprintf(P1_x_s,"%d",data->xjoueur1);
      sprintf(P1_y_s,"%d",data->yjoueur1);
      sprintf(P2_x_s,"%d",data->xjoueur2);
      sprintf(P2_y_s,"%d",data->yjoueur2);

      strcat(sendc, "\nJOUEURS ");
      strcat(sendc, P1_x_s);
      strcat(sendc, " ");
      strcat(sendc, P1_y_s);
      strcat(sendc, " ");
      strcat(sendc, P2_x_s);
      strcat(sendc, " ");
      strcat(sendc, P2_y_s);

		  strcat(sendc, "\nSTATUT ");
		  if(data->etatPartie == STATUT_STOP) {
        strcat(sendc, "0");
      } else if(data->etatPartie == STATUT_DEBUT) {
        strcat(sendc, "1");
      } else if(data->etatPartie == STATUT_GAME_OVER) {
        strcat(sendc, "2");
      }

      if(send(cs, sendc, strlen(sendc), 0) < 0) {
        perror("Erreur lors de l'envoi ");
        exit(errno);
      }

      memset(str, 0, 1024);
      memset(sendc, 0, 1024);
    }
  }
}

void	*init_server(void *arg)
{
  t_server	*server;
  t_data  *data;
  int ret;

  if ((data = malloc(sizeof(t_data))) == NULL) {
    fprintf(stderr, "Erreur de memoire.\n");
    exit(1);
  }

  data = (t_data *)arg;

  if ((server = malloc(sizeof(t_server))) == NULL) {
    fprintf(stderr, "Erreur du serveur.\n");
    exit(1);
  }
/* Stream socket c'est un prise de flux est un type de socket de communication de réseau qui fournit un flux de données
 entre deux systemes */
/* Utilisent le protocole de contrôle de transmission (TCP), le protocole de transmission de contrôle de flux (SCTP) 
ou le protocole de contrôle de congestion de datagramme (DCCP).*/
  server->fd_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server->fd_socket < 0) {
    fprintf(stderr, "Erreur de connexion.\n");
		exit(1);
  }
  printf("[+]Serveur cree avec succes.\n");
// on crée des sockte servreur
// AF_INET spécifie que l'on utilise le protocole IP
//AF_INET est la famille d'adresses pour IPv4
  server->sin.sin_family = AF_INET;
  server->sin.sin_port = htons(PORT);
  server->sin.sin_addr.s_addr = inet_addr(data->IP);
// relier le serveur avec les clients 
// un socket c'est flux pour comminiquer entre le serveur et le client

  ret = bind(server->fd_socket, (const struct sockaddr *)&server->sin, sizeof(server->sin));
  if(ret < 0) {
    fprintf(stderr, "[-]Erreur.\n");

    exit(1);
  }

  if(listen(server->fd_socket, 4) == 0) {
    printf("En attente...\n");
  } else {
    fprintf(stderr, "Erreur.\n");
    close(server->fd_socket);
  }

  demarrageServeur(server, data);
  close(server->fd_socket);
  return NULL;
}

int lancementServeur(char* ip,  int scoreFinal)
{
  pthread_t	print;
  pthread_t	server;
  t_data  *data;

  if ((data = malloc(sizeof(t_data))) == NULL) {
    fprintf(stderr, "Erreur de donnees.\n");
    exit (1);
  }

  data->type = SERVER_TYPE;
  data->scorep1 = 0;
  data->scorep2 = 0;
  data->etatPartie = STATUT_STOP;
  data->IP = ip;
  data->scoreFinal = scoreFinal;

  pthread_create(&server, NULL, &init_server, data);
  pthread_create(&print, NULL, affichage, data);

  pthread_join(server, NULL);
  pthread_join(print, NULL);
  free(data);

  return (0);
}
