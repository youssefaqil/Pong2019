#include "main.h"

void	lectureDonnees(t_data *data)
{
  char	buffer[1024];
  int	rread;

  while (1) {
    if ((rread = recv(data->socket, buffer, 1024, 0)) < 0) {
      fprintf(stderr, "Erreur dans la reception de donnees.\n");
      exit (1);
    }
    //buffer  est une zone de mémoire vive ou de disque utilisée pour enregistrer  les données temporairement, 
    buffer[rread] = '\0';

    if (strlen(buffer) <= 1024) {
      puts(buffer);
      data->game = buffer;
      verifCommande(buffer, data);
    } 
    memset(buffer, 0, 1024);
  }
}

void	*init_client(void *arg)
{
  int ret;
  t_data  *data;

  if ((data = malloc(sizeof(t_data))) == NULL) {
    fprintf(stderr, "Erreur dans l'allocation de donnees.\n");
    exit (1);
  }

  data = (t_data *)arg;
  data->socket = socket(AF_INET, SOCK_STREAM, 0);
  if (data->socket < 0){
    fprintf(stderr, "Erreur de connexion.\n");
		exit(1);
  } 
  printf("Client cree.\n");
//Implémentation Linux du protocole IPv4  
  data->sin.sin_family = AF_INET; //sin_family est toujours rempli avec AF_INET.
  data->sin.sin_port = htons(PORT);//sin_port contient le numéro de port, dans l'ordre des octets du réseau. Les numéros de ports inférieures à 1024 sont dits réservés
  data->sin.sin_addr.s_addr = inet_addr(data->IP);

/* Ouvre une connexion sur le socket FD pour homologue à ADDR (longueur en octets LEN).
    Pour les types de socket sans connexion, il suffit de définir l'adresse par défaut à laquelle envoyer
    et la seule adresse à partir de laquelle accepter les transmissions.
    Renvoie 0 en cas de succès, -1 en cas d'erreur.

    Cette fonction est un point d’annulation et n’est donc pas marquée avec
    __JETER. */
  ret = connect(data->socket, (const struct sockaddr *)&data->sin, sizeof(data->sin));
  if(ret < 0){
	  fprintf(stderr, "Erreur de connexion.\n");
    free(data);
	  exit(1);
  }

  printf("Connexion effectue.\n");

  lectureDonnees(data);
  close(data->socket);
  pthread_exit(NULL);
  return NULL;
}

int	lancementClient(char* ip, int scoreFinal)
{
  pthread_t	affichageThread;

  pthread_t	client;
  t_data  *data;

  if ((data = malloc(sizeof(t_data))) == NULL) {
    fprintf(stderr, "Erreur dans l'allocation de donnees.\n");
    exit (1);
  }

  data->type = CLIENT_TYPE;
  data->scorep1 = 0;
  data->scorep2 = 0;
  data->etatPartie = STATUT_STOP;
  data->IP = ip; 
  data->scoreFinal=scoreFinal; 

  pthread_create(&client, NULL, &init_client, data);
  pthread_create(&affichageThread, NULL, affichage, data);

  pthread_join(client, NULL);
  pthread_join(affichageThread, NULL);
  free(data); 

  return (0); 
}