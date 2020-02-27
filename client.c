#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define MAX 80 
#define SA struct sockaddr 


void chatFunction(int sockfd) 
{ 
	char buff[MAX]; 
	int n; 
	for (;;) { 
		bzero(buff, sizeof(buff)); 
		printf("Enter the string : "); 
		n = 0; 
		while ((buff[n++] = getchar()) != '\n') 
			; 
		write(sockfd, buff, sizeof(buff)); 
		bzero(buff, sizeof(buff)); 
		read(sockfd, buff, sizeof(buff)); 
		printf("From Server : %s", buff); 
		if ((strncmp(buff, "exit", 4)) == 0) { 
			printf("Client Exit...\n"); 
			break; 
		} 
	} 
} 



int main(int argc,char* argv[]) 
{ 
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 

	//Création du socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
	{
		printf("Socket successfully created..\n");
	}
		
	int port; 
	//Définition du port et de l'adresse du serveur via ligne de commande
	//    ./client ipDuServeur portDuServeur
	if(argc==3) 
    { 
        port = atoi(argv[2]);		//conversion du port en entier
		printf("Port : %d\n",port);

		//assignation du port et de l'ip a l'objet
		servaddr.sin_family = AF_INET; 
		servaddr.sin_addr.s_addr = inet_addr(argv[1]); //IP
		servaddr.sin_port = htons(port); //PORT
    } 
	else
	{
		printf("Use ./client ipHost portHost");		//Erreur dans la commande
	}
	

	//Connection au serveur
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		exit(0); 
	} 
	else
		printf("connected to the server..\n"); 

	//Une fois connectée, on lance le chat. 
	chatFunction(sockfd); 

	// close the socket 
	close(sockfd); 
} 
