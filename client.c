#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define MAXCHAR 200 

// Fonction pour le chat
void chatFunction(int clientSocket) 
{ 
	char buff[MAXCHAR]; 
	int n; 

	for (;;) { 
		
		//vide le buffer
		memset(buff, 0, MAXCHAR);
     
	 	//On prepare un nouvel envoie
		printf("Votre message : "); 
		n = 0; 
		// Pendant que l'on tape, on ajoute les char dans le buffer

		while(1)
		{
			buff[n] = getchar();
			if(buff[n]=='\n')
			{
				break;
			}
			n++;
		}

		//Envoie un message au client
		write(clientSocket, buff, sizeof(buff)); 

		//Attend un message du client
		//Quand un message arrive il est mis dans le buffer
		read(clientSocket, buff, sizeof(buff)); 
		//Affichage du message
		printf("Message reçu : %s", buff);
	} 
} 

int main(int argc,char* argv[]) 
{ 
	//definition des variables
	int clientSocket, connfd; 
	struct sockaddr_in servaddr, cli; 

	//Création du socket
	clientSocket = socket(AF_INET, SOCK_STREAM, 0); 
	if (clientSocket == -1) { 
		printf("Erreur de la création du socket\n"); 
		exit(0); 
	}
		
	int port; 
	//Définition du port et de l'adresse du serveur via ligne de commande
	//    ./client ipDuServeur portDuServeur
	// Apres un redemarrage, l'ancien port peut ne pas etre dispo
	// Il faut penser a changer de port (Ou utiliser reuse_addr pour changer le timeout du port mais j'ai pas reussi)

	if(argc==3) 
    { 
        port = atoi(argv[2]);		//conversion du port en entier
		printf("Port : %d\n",port);

		//assignation du port et de l'ip
		servaddr.sin_family = AF_INET; 
		servaddr.sin_addr.s_addr = inet_addr(argv[1]); //IP
		servaddr.sin_port = htons(port); //PORT
    } 
	else
	{
		printf("Executez ./client ipHost portHost");		//Erreur dans la commande
	}
	

	//Connection au serveur
	if (connect(clientSocket, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) { 
		printf("La connection avec le serveur a échouée\n"); 
		exit(0); 
	} 
	else
		printf("Connecté\n"); 

	//Une fois connectée, on lance le chat. 
	chatFunction(clientSocket); 
} 
