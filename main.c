#include "main.h"

int	main()
{
	char ipAddress[10] = "127.0.0.1";
	int scoreFinal = 3;
	printf("Bienvenu chez Pong!");
	printf("Pour vous deplacer, utilisez les fleches directionnelles.\n");
	printf("Pour gagner, marquez %d points.\n\n", scoreFinal);

	while (1) {
		printf("Faites votre choix: \n");
		printf("1- Creer une partie\n");
		printf("2- Rejoindre une partie\n");
		printf("3- Quitter\n");
		
 
		int choix = 0;
		scanf("%d", &choix);

		if(choix == 1){	
			//pour inistiasier le srveur , mettre en place le serveur/ 
			lancementServeur(ipAddress, scoreFinal);
			break;
		} else if(choix == 2){
			lancementClient(ipAddress,scoreFinal);
			break;
		} else if(choix == 3){
			exit(1);
		} else{
			printf("Erreur, veillez entrer un choix valide\n\n");
			while (getchar() != '\n');
		}
	}

	return (0);
}
