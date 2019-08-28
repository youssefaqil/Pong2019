#include "main.h"

void	verifCommande(char *buffer, t_data *data)
{
  char  **jeu;
  int i = 0;

  jeu = transfert(buffer, '\n');

  while (jeu[i] != NULL) {
    if (strncmp(jeu[i], "SCORE", 5) == 0) {
      scores(jeu[i], data);
    } else if (strncmp(jeu[i], "JOUEURS", 7) == 0) {
      joueurs(jeu[i], data);
    } else if (strncmp(jeu[i], "BALL", 4) == 0) {
      balle(jeu[i], data);
    } else if (strncmp(jeu[i], "STATUT", 6) == 0) {
      statuts(jeu[i], data);
    }
    i++;
  }
  
  free(jeu);
}

void	statuts(char *buffer, t_data *data)
{
  char	**tab;

  tab = transfert(buffer, ' ');
  data->etatPartie = atoi(tab[1]);

  free(tab[0]);
  free(tab[1]);
  free(tab);
  free(buffer);
}

void	scores(char *buffer, t_data *data)
{
  char	**tab;

  tab = transfert(buffer, ' ');

  data->scorep1 = atoi(tab[1]);
  data->scorep2 = atoi(tab[2]);

  free(tab[0]);
  free(tab[1]);
  free(tab[2]);
  free(tab);
  free(buffer);
}

void	joueurs(char *buffer, t_data *data)
{
  char	**tab;

  tab = transfert(buffer, ' ');

  data->xjoueur1 = atoi(tab[1]);
  data->yjoueur1 = atoi(tab[2]);

  data->xjoueur2 = atoi(tab[3]);
  data->yjoueur2 = atoi(tab[4]);

  int i;
  for(i = 0; i < 5; i++){
	  free(tab[i]);
  }

  free(tab);
  free(buffer);
}

void	balle(char *buffer, t_data *data)
{
  char	**tab;

  tab = transfert(buffer, ' ');
  data->ball_x = atoi(tab[1]);
  data->ball_y = atoi(tab[2]);

  free(tab[0]);
  free(tab[1]);
  free(tab[2]);
  free(tab);
  free(buffer);
}

char	**transfert(char *str, char sp)
{
  int	i;
  int	j;
  int	x;
  char	**tab;

  if ((tab = malloc((1 + strlen(str)) * sizeof(char *))) == NULL)
    return (NULL);

  i = 0;
  j = 0;
  while (str[i] != '\0') {
    x = 0;
    while (str[i] != '\0' && (str[i] == sp || str[i] == ' ' || str[i] == '\t'))
      i++;
    if (str[i] != '\0' && (tab[j] = malloc(strlen(str) + 1)) == NULL)
      return (NULL);
    if (str[i] == '\0')
      return (tab);
    while (str[i] != '\0' && str[i] != sp) {
     tab[j][x] = str[i];
     x++;
     i++;
   }
   while (str[i++] != '\0' && str[i] == sp);
   tab[j++][x] = 0;
   i--;
 }
 tab[j] = NULL;
 return (tab);
}

void *affichage(void *arg)
{
  t_data	*data;
  t_affichage	*affichage;
  
  //La structure FILE permet de stocker les informations relatives à la gestion d'un flux de données
//stderr : ce dernier flux est associé à la sortie standard d'erreur de l'application
  if ((data = malloc(sizeof(t_data))) == NULL || (affichage = malloc(sizeof(t_affichage))) == NULL)
  {
    fprintf(stderr, "Erreur\n");
    exit (1);
  }
  data = (t_data *)arg;

  if (init_sdl(affichage, data) == 1) {
    return NULL;
  } else {
    sdl_start(affichage, data);
  }

  pthread_exit(NULL);
  return NULL;
}
