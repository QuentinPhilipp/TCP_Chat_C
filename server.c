#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define MAXCHAR 200 
#define SA struct sockaddr 

// Fonction pour le chat
void chatFunction(int chatSocket) 
{ 
	char buff[MAXCHAR]; 
	int n; 

	for (;;) { 

		//vide le buffer
		memset(buff, 0, MAXCHAR);

		// Le message du client est mis dans le buffer
		read(chatSocket, buff, sizeof(buff)); 

		// On affiche le buffer
		printf("Message reçu: %s", buff); 

		//On prepare un nouvel envoie
		printf("Votre message: ");
		//reset buffer
		memset(buff, 0, MAXCHAR); 
		n = 0; 

		// Pendant que l'on tape, on ajoute les char dans le buffer
		while(1)
		{
			buff[n] = getchar();

			//fin de la boucle au moment ou l'on appuie sur Entree
			if(buff[n]=='\n')		
			{
				break;
			}
			n++;
		} 

		// Envoie du buffer au client
		write(chatSocket, buff, sizeof(buff)); 
	} 
} 

// Main function
int main(int argc,char* argv[]) 
{ 
	//definition des variables
	int serverSocket, chatSocket, len; 
	struct sockaddr_in servaddr, cli; 

	// Création du socket serveur
	serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
	if (serverSocket == -1) { 
		printf("Socket fail\n"); 
	} 
	
    int port; 

	if(argc==2) 
    { 
        port = atoi(argv[1]);
		printf("%d\n",port);

		//assignation du port et de l'ip a l'objet
		servaddr.sin_family = AF_INET; 
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
		servaddr.sin_port = htons(port); 
    } 
	else 
	{
		printf("Executez ./server port\n");		//Erreur dans la commande
		exit(0);
	}

	// Bind le socket a l'adresse IP donnée
	if ((bind(serverSocket, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("Erreur de binding du socket\n"); //Verification
		exit(0); 
	} 
	
	// Serveur prêt à l'ecoute
	if ((listen(serverSocket, 1)) != 0) { 
		printf("Erreur ecoute\n"); 
		exit(0); 
	} 
	else
		printf("Le serveur est prêt a recevoir des messages\n"); 

	len = sizeof(cli);

	// Acceptation du client et creation du socket de chat
	chatSocket = accept(serverSocket, (struct sockaddr *)&cli, &len);
	if (chatSocket < 0) { 
		printf("Erreur acceptation du client\n"); 
		exit(0); 
	} 
	else
		printf("Nouveau client connecté\n"); 

	// Demarrage du chat
	chatFunction(chatSocket); 
} 
