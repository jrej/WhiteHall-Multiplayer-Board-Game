/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


struct _client{
	char adresseIp[40];
	int port;
	char nom[10];
} Clients[4];
int nbClients; //  compteur nombr de client
int ouvServer;
bool winJ = false ; // valeur a modifié en cas de victoire de jack
bool loseJ = false ; // valeur a modifié en cas de victoire des policier
int jack = 0 ; // compteur pour savoir si jack est connecter
int police = 0;// compteur pour savoir si les conde son co
int pos_police;
int caseJack[4];
int compteurJack;
int casePolice[3];
int compteurPolice;

void AfficherClients(){
	int i;
	for(i = 0; i<nbClients;i++){
		printf("%d : %s %d %s \n", i, Clients[i].nom,Clients[i].port,Clients[i].adresseIp);
	}
}

int rechercheClientParNom(char * nom){
	int i;
	for(i = 0; i<nbClients;i++){
		if(strcmp(Clients[i].nom,nom) == 0)
			return i;

	}
	return -1;
}



void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void sendMessageToClient(char *ip,char *mess, int port)
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    portno = port;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server = gethostbyname(ip);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        {
                printf("ERROR connecting\n");
                exit(1);
        }

        strcpy(buffer,mess);
        n = write(sockfd,buffer,strlen(buffer));

    close(sockfd);
}


void globalMessage(char * mess)
{
	int i;
	for(i=0;i < nbClients ; i++)
		sendMessageToClient(Clients[i].adresseIp,mess,Clients[i].port);

}
int main(int argc, char *argv[])
{

	int i;
	char messPos[100];
	char intC[1];
	int choixJack;
	char com;
	char adresseIpClient[40];
	char nomClient[20];
	int portClient;
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
	char ipaddress[200];
	int tour = 0 ;

     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     sockfd = socket(AF_INET, SOCK_STREAM, 0);

     if (sockfd < 0)
        error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while (1)
     {
     	newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr,
                 &clilen);
     	if (newsockfd < 0)
          	error("ERROR on accept");


     	bzero(buffer,256);
     	n = read(newsockfd,buffer,255);
     	if (n < 0)
		error("ERROR reading from socket");

    	//printf("Received packet from %s:%d\nData: [%s]\n\n",
        //        inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), buffer);
        if(tour >= nbClients)
            tour = 0 ;

        if(winJ)
            globalMessage("W Jack");
        if(loseJ)
            globalMessage(" L Jack ") ;

	switch(buffer[0]){
		case 'C' : sscanf(buffer,"%s %s %d %s", &com, adresseIpClient, &portClient,nomClient); // recopie selon protocole de connexion : C <adresseIp> <port> <nomJoueur>
		if(nbClients == 2)
			sendMessageToClient(adresseIpClient, "La partie est déja complète\n", portClient);
		if(ouvServer == 0){ //Le serveur accueille des clients

			if(rechercheClientParNom(nomClient) >= 0 ) //Vérifie si le nom est unique
				sendMessageToClient(adresseIpClient, "Nom déja utilisé\n", portClient);

			else {
				// On recopie les informations émises dans le tableaux de Clients
				strcpy(Clients[nbClients].adresseIp,adresseIpClient);

				Clients[nbClients].port = portClient;
				strcpy(Clients[nbClients].nom,nomClient);

				if(jack == 0){

				sendMessageToClient(adresseIpClient, " Jack \n" , portClient);
				jack = 1;

				}

				else if(jack == 1 && police < 3){//3 police max pur le moment

				sendMessageToClient(adresseIpClient , "Police\n", portClient);
				police++;

				}


				nbClients++; //Indique qu'on ajoute un joueur

				sendMessageToClient(adresseIpClient, "Connexion réussi\n", portClient); //Indique au joueur qu'il est connecté

				if(nbClients == 2){
					ouvServer=1; // Le serveur lance la partie
					globalMessage("La partie peut maintenant commencer\n Jack doit choisir 4 cases\n"); //Indique à tous que le début de la partie
					sendMessageToClient(Clients[0].adresseIp, "Jack, choisis 4 cases\n", Clients[0].port); //1ere règle: Jack(joueur 0) doit choisir les  4 cases

					for(i = 0 ; i < nbClients; i++){
						strcat(messPos,"T ");
						printf("Message cc %s\n",messPos);
						sprintf(intC,"%d ",i);
						strcat(messPos,intC);
						printf("message envoyé : %s \n",messPos);
						sendMessageToClient(Clients[i].adresseIp, messPos, Clients[i].port);
						strcpy(messPos,"");
					}


					AfficherClients();
				}
			}
		}
		break;
		case 'X' : sscanf(buffer,"%s %d" ,&com , &choixJack); // protocole d'envoi de cases par Jack : X <NumCase>
			if(ouvServer == 1) {
				if(compteurJack < 4){
					caseJack[compteurJack] = choixJack; //On remplit le tableau
					compteurJack++;

				}
				if(compteurJack == 4){ //Jack a choisi ses 4 cases
					globalMessage("Jack a choisit ses 4 cases \n Lancement du game\n");
					int i; //Affichage des 4 cases de Jack
					for( i=0;i<compteurJack;i++)
						printf("Case choisi : %d\n",caseJack[i]);
					ouvServer=2;
				}


			}
		break;

		case 'P' : sscanf(buffer, "%s %d", &com , &pos_police);
			if(ouvServer == 1){
				casePolice[compteurPolice] = pos_police;
				printf("Police case %d \n",casePolice[compteurPolice] );
				compteurPolice++;
				if(compteurPolice > 3)
					compteurPolice = 0 ;

        case 'J' ://protocole d'envoie de positon pour Jack



			id= findClientByName(clientName);

			if(tour == 0 ){


				//	globalMessage(" J %d \n", position_jack);
                sscanf(buffer, "%s %d %s", &com , &position_jack, clientName);
					tour++;

			}

			else{

				id=findClientByName(clientName);

				sendMessageToClient(tcpClients[id].ipAddress,

					tcpClients[id].port,

					"veuillez attendre votre tour\n");



			}



			break ;

		case 'P' ://protocole d'envoie position Police



			id=findClientByName(clientName);

			if(tour >= 1 ){// la position des flics envoyé a tout les client avec leur nom pour differencier chaque policier

                sscanf(buffer, "%s %d %s ", &com , &position_police[tour-1], clientName);
					tour++;

			}

			else{



				sendMessageToClient(tcpClients[id].ipAddress,

					tcpClients[id].port,

					"veuillez attendre votre tour\n");



			}



			break ;

			}

	}





     	close(newsockfd);
     }
     close(sockfd);
     return 0;
}
