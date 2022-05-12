#include <SDL.h>
#include <SDL/SDL_image.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
//#include "TAB_POINT.h"

struct monPoint{
	int x;
	int y;
};
struct monPoint tabPoint[200];
int cpt_point;
int port; // port du serveur
int jack = 0; //=1 si l'on est jack
int police = 0; // =1 si on est un conde
int connecte = 0; //permet de dire si l'on est connecté au serveur
int numJoueur =-1;
char numJoueurC[1];
char com;
volatile int synchro;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void sendMessageToServer(char *mess,char * port , char * serveur)
{
    char buffer[256];
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
  // printf("%s\n",mess);

    portno = atoi(port); //Serveur global du server_white
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
        	printf("ERROR socket\n");
		exit(1);

}
    server = gethostbyname(serveur);// a remplacer par la vrai IP du serveur
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

void *server_function(void *argn)
{
     	int sockfd, newsockfd, portno;
     	socklen_t clilen;
     	char buffer[256];
     	struct sockaddr_in serv_addr, cli_addr;
     	int n;

     	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd<0)
	{
		printf("sockfd error\n");
		exit(1);
	}

     	bzero((char *) &serv_addr, sizeof(serv_addr));
     	portno = port;
     	serv_addr.sin_family = AF_INET;
     	serv_addr.sin_addr.s_addr = INADDR_ANY;
     	serv_addr.sin_port = htons(portno);
     	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("bind error\n");
		exit(1);
	}

	listen(sockfd,5);
     	clilen = sizeof(cli_addr);
     	while (1)
     	{
        	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        	if (newsockfd < 0)
		{
			printf("accept error\n");
			exit(1);
		}

        	bzero(buffer,256);
        	n = read(newsockfd,buffer,255);
        	if (n < 0)
		{
			printf("read error\n");
			exit(1);
		}


		if(buffer[0] == 'T'){
			sscanf(buffer,"%s %d", &com, &numJoueur);
			//printf("%d\n",numJoueur);
			}
		//else if(strcmp(buffer,"Jack, choisis 4 cases\n") == 0 )
			//jack = 1;


       		printf("%s\n",buffer);

		if(strcmp(buffer, "Jack") == 0)
			jack = 1;
		else if(strcmp(buffer, "Police") == 0 )
			police =1 ;

        if(strcmp(buffer, "W Jack")){
            printf("Partie terminé , Victoire de Jack\n");
            quit = true ;

        }
        else if(strcmp(buffer, "L Jack")){
            printf("Partie terminer , Victoire des Policiers");
            quit = true ;
        }

		connecte = 1;//Indique que l'on est connecté
   		pthread_mutex_lock( &mutex ); // on protege la variable synchro
		synchro=1;// on souhaite section critique le plus court possible
   		pthread_mutex_unlock( &mutex );

		while (synchro);//tant que ca vaut 1, le consommateur n'as pas consomme le msg

        	close(newsockfd);
     }
		//usleep(100000);
}


void init_tab(void){
tabPoint[0].x=93 ; tabPoint[0].y=73;
tabPoint[1].x=281 ; tabPoint[1].y=56;
tabPoint[2].x=384 ; tabPoint[2].y=62;
tabPoint[3].x=536 ; tabPoint[3].y=63;
tabPoint[4].x=612 ; tabPoint[4].y=59;
tabPoint[5].x=719 ; tabPoint[5].y=61;
tabPoint[6].x=881 ; tabPoint[6].y=86;
tabPoint[7].x=55 ; tabPoint[7].y=127;
tabPoint[8].x=120 ; tabPoint[8].y=90;
tabPoint[9].x=137 ; tabPoint[9].y=135;
tabPoint[10].x=165 ; tabPoint[10].y=101;
tabPoint[11].x=197 ; tabPoint[11].y=97;
tabPoint[12].x=227 ; tabPoint[12].y=156;
tabPoint[13].x=258 ; tabPoint[13].y=99;
tabPoint[14].x=288 ; tabPoint[14].y=159;
tabPoint[15].x=303 ; tabPoint[15].y=116;
tabPoint[16].x=343 ; tabPoint[16].y=118;
tabPoint[17].x=383 ; tabPoint[17].y=124;
tabPoint[18].x=445 ; tabPoint[18].y=141;
tabPoint[19].x=540 ; tabPoint[19].y=95;
tabPoint[20].x=558 ; tabPoint[20].y=158;
tabPoint[21].x=599 ; tabPoint[21].y=111;
tabPoint[22].x=656 ; tabPoint[22].y=95;
tabPoint[23].x=717 ; tabPoint[23].y=153;
tabPoint[24].x=794 ; tabPoint[24].y=99;
tabPoint[25].x=897 ; tabPoint[25].y=165;
tabPoint[26].x=960 ; tabPoint[26].y=114;
tabPoint[27].x=75 ; tabPoint[27].y=201;
tabPoint[28].x=129 ; tabPoint[28].y=177;
tabPoint[29].x=134 ; tabPoint[29].y=229;
tabPoint[30].x=181 ; tabPoint[30].y=236;
tabPoint[31].x=219 ; tabPoint[31].y=203;
tabPoint[32].x=261 ; tabPoint[32].y=214;
tabPoint[33].x=315 ; tabPoint[33].y=219;
tabPoint[34].x=364 ; tabPoint[34].y=217;
tabPoint[35].x=397 ; tabPoint[35].y=251;
tabPoint[36].x=426 ; tabPoint[36].y=190;
tabPoint[37].x=476 ; tabPoint[37].y=177;
tabPoint[38].x=477 ; tabPoint[38].y=237;
tabPoint[39].x=521 ; tabPoint[39].y=223;
tabPoint[40].x=563 ; tabPoint[40].y=203;
tabPoint[41].x=623 ; tabPoint[41].y=195;
tabPoint[42].x=679 ; tabPoint[42].y=250;
tabPoint[43].x=691 ; tabPoint[43].y=196;
tabPoint[44].x=791 ; tabPoint[44].y=176;
tabPoint[45].x=794 ; tabPoint[45].y=238;
tabPoint[46].x=950 ; tabPoint[46].y=201;
tabPoint[47].x=84 ; tabPoint[47].y=260;
tabPoint[48].x=106 ; tabPoint[48].y=288;
tabPoint[49].x=109 ; tabPoint[49].y=326;
tabPoint[50].x=181 ; tabPoint[50].y=309;
tabPoint[51].x=246 ; tabPoint[51].y=341;
tabPoint[52].x=262 ; tabPoint[52].y=303;
tabPoint[53].x=287 ; tabPoint[53].y=272;
tabPoint[54].x=316 ; tabPoint[54].y=337;
tabPoint[55].x=426 ; tabPoint[55].y=274;
tabPoint[56].x=479 ; tabPoint[56].y=311;
tabPoint[57].x=550 ; tabPoint[57].y=334;
tabPoint[58].x=583 ; tabPoint[58].y=275;
tabPoint[59].x=671 ; tabPoint[59].y=322;
tabPoint[60].x=730 ; tabPoint[60].y=325;
tabPoint[61].x=739 ; tabPoint[61].y=273;
tabPoint[62].x=802 ; tabPoint[62].y=316;
tabPoint[63].x=880 ; tabPoint[63].y=322;
tabPoint[64].x=903 ; tabPoint[64].y=280;
tabPoint[65].x=934 ; tabPoint[65].y=338;
tabPoint[66].x=950 ; tabPoint[66].y=286;
tabPoint[67].x=95 ; tabPoint[67].y=385;
tabPoint[68].x=149 ; tabPoint[68].y=403;
tabPoint[69].x=209 ; tabPoint[69].y=421;
tabPoint[70].x=277 ; tabPoint[70].y=392;
tabPoint[71].x=360 ; tabPoint[71].y=357;
tabPoint[72].x=434 ; tabPoint[72].y=351;
tabPoint[73].x=484 ; tabPoint[73].y=388;
tabPoint[74].x=531 ; tabPoint[74].y=406;
tabPoint[75].x=605 ; tabPoint[75].y=370;
tabPoint[76].x=625 ; tabPoint[76].y=408;
tabPoint[77].x=692 ; tabPoint[77].y=404;
tabPoint[78].x=769 ; tabPoint[78].y=398;
tabPoint[79].x=827 ; tabPoint[79].y=399;
tabPoint[80].x=67 ; tabPoint[80].y=451;
tabPoint[81].x=156 ; tabPoint[81].y=465;
tabPoint[82].x=246 ; tabPoint[82].y=488;
tabPoint[83].x=260 ; tabPoint[83].y=440;
tabPoint[84].x=289 ; tabPoint[84].y=492;
tabPoint[85].x=357 ; tabPoint[85].y=490;
tabPoint[86].x=361 ; tabPoint[86].y=446;
tabPoint[87].x=401 ; tabPoint[87].y=466;
tabPoint[88].x=472 ; tabPoint[88].y=495;
tabPoint[89].x=513 ; tabPoint[89].y=454;
tabPoint[90].x=550 ; tabPoint[90].y=507;
tabPoint[91].x=556 ; tabPoint[91].y=458;
tabPoint[92].x=641 ; tabPoint[92].y=473;
tabPoint[93].x=719 ; tabPoint[93].y=464;
tabPoint[94].x=778 ; tabPoint[94].y=451;
tabPoint[95].x=887 ; tabPoint[95].y=511;
tabPoint[96].x=898 ; tabPoint[96].y=429;
tabPoint[97].x=967 ; tabPoint[97].y=470;
tabPoint[98].x=113 ; tabPoint[98].y=526;
tabPoint[99].x=179 ; tabPoint[99].y=565;
tabPoint[100].x=187 ; tabPoint[100].y=521;
tabPoint[101].x=254 ; tabPoint[101].y=590;
tabPoint[102].x=277 ; tabPoint[102].y=538;
tabPoint[103].x=341 ; tabPoint[103].y=594;
tabPoint[104].x=398 ; tabPoint[104].y=537;
tabPoint[105].x=480 ; tabPoint[105].y=559;
tabPoint[106].x=523 ; tabPoint[106].y=568;
tabPoint[107].x=629 ; tabPoint[107].y=561;
tabPoint[108].x=632 ; tabPoint[108].y=522;
tabPoint[109].x=705 ; tabPoint[109].y=593;
tabPoint[110].x=738 ; tabPoint[110].y=515;
tabPoint[111].x=768 ; tabPoint[111].y=582;
tabPoint[112].x=847 ; tabPoint[112].y=548;
tabPoint[113].x=859 ; tabPoint[113].y=598;
tabPoint[114].x=900 ; tabPoint[114].y=587;
tabPoint[115].x=968 ; tabPoint[115].y=557;
tabPoint[116].x=89 ; tabPoint[116].y=663;
tabPoint[117].x=109 ; tabPoint[117].y=603;
tabPoint[118].x=176 ; tabPoint[118].y=649;
tabPoint[119].x=256 ; tabPoint[119].y=675;
tabPoint[120].x=271 ; tabPoint[120].y=641;
tabPoint[121].x=362 ; tabPoint[121].y=653;
tabPoint[122].x=380 ; tabPoint[122].y=610;
tabPoint[123].x=420 ; tabPoint[123].y=648;
tabPoint[124].x=457 ; tabPoint[124].y=678;
tabPoint[125].x=506 ; tabPoint[125].y=666;
tabPoint[126].x=514 ; tabPoint[126].y=610;
tabPoint[127].x=590 ; tabPoint[127].y=610;
tabPoint[128].x=616 ; tabPoint[128].y=648;
tabPoint[129].x=654 ; tabPoint[129].y=613;
tabPoint[130].x=798 ; tabPoint[130].y=650;
tabPoint[131].x=876 ; tabPoint[131].y=638;
tabPoint[132].x=919 ; tabPoint[132].y=629;
tabPoint[133].x=133 ; tabPoint[133].y=727;
tabPoint[134].x=194 ; tabPoint[134].y=752;
tabPoint[135].x=236 ; tabPoint[135].y=764;
tabPoint[136].x=292 ; tabPoint[136].y=726;
tabPoint[137].x=326 ; tabPoint[137].y=738;
tabPoint[138].x=361 ; tabPoint[138].y=704;
tabPoint[139].x=429 ; tabPoint[139].y=713;
tabPoint[140].x=518 ; tabPoint[140].y=740;
tabPoint[141].x=636 ; tabPoint[141].y=727;
tabPoint[142].x=682 ; tabPoint[142].y=688;
tabPoint[143].x=778 ; tabPoint[143].y=748;
tabPoint[144].x=816 ; tabPoint[144].y=720;
tabPoint[145].x=844 ; tabPoint[145].y=762;
tabPoint[146].x=854 ; tabPoint[146].y=700;
tabPoint[147].x=887 ; tabPoint[147].y=739;
tabPoint[148].x=900 ; tabPoint[148].y=686;
tabPoint[149].x=916 ; tabPoint[149].y=722;
tabPoint[150].x=952 ; tabPoint[150].y=702;

}

int main(int argc, char ** argv)
{
	if(argc != 6 ){
	printf("Error,<fonction> <AdresseServeur> <PortServeur> <VotreAdresse> <VotrePort> <NomJoueur>\n");
	exit(1);
	}
	char messageServer[100]; ///Variable contenant le message à envoyer au serveur
	char adresseIp[40]; // Contient l'adresse Ip du client
	int copie=0;
	char nom[20];//Contient le nom du client passé en argument

	strcpy(adresseIp,argv[3]);
	port = atoi(argv[4]);
	strcpy(nom,argv[5]);

	init_tab(); //Initialise le tableau des cases numérotés

	int position,cpt; //position indique le numéro de la case sélectionné , cpt parcourt le tableau pour chercher la case
	char positionC[1]; // convertit la position en caractere
	char couleur[20];
	char pion[20]; // Indique quel pion on va choisir, selon l'ordre de connexion
	int x=414,y=334;
    int quit = 0;
    int cmp = 0 ;
    SDL_Event event;
    pthread_t thread1;
	int mx,my; // indique les position x et y de l'endroit ou l'on clique

    SDL_Init(SDL_INIT_VIDEO); // Initialisation du système graphique

    SDL_Window * window = SDL_CreateWindow("SDL2 Displaying Image",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024+128, 1024, 0); // Crée la fenetre de taille 1024+128 * 1024

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0); // On s'en bat les couilles
    //SDL_Surface * image = SDL_LoadBMP("whitehall_1024x1024.bmp");
    SDL_Surface * image = SDL_LoadBMP("whitehall_1024x1024.bmp"); // Charge l'image du plateau
printf("image=%p\n",image);

    //SDL_Surface * pawn = SDL_LoadBMP("pawn.bmp");
    SDL_Surface * rond = SDL_LoadBMP("rond.bmp"); //charge le rond
    SDL_SetColorKey( rond, SDL_TRUE, SDL_MapRGB( rond->format, 0, 0, 0 ) ); //la mm
    //SDL_Surface * rond = SDL_LoadBMP("rond.bmp");
    SDL_Surface * rond_jaune = SDL_LoadBMP("rond_jaune.bmp"); //charge le rond
    SDL_SetColorKey( rond_jaune, SDL_TRUE, SDL_MapRGB( rond_jaune->format, 0, 0, 0 ) ); //la mm
    SDL_Surface * connexion = SDL_LoadBMP("connexion.bmp"); //charge le rond
    //SDL_SetColorKey( connexion, SDL_TRUE, SDL_MapRGB( connexion->format, 0, 0, 0 ) ); //la mm


    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image); // crée la texture de la carte

    SDL_Texture * texture_rond = SDL_CreateTextureFromSurface(renderer, rond);// pour le rond
    SDL_Texture * texture_rond_jaune = SDL_CreateTextureFromSurface(renderer, rond_jaune);// pour le rond
    SDL_Texture * texture_connexion = SDL_CreateTextureFromSurface(renderer, connexion);// pour le rond
	SDL_SetTextureAlphaMod(texture_rond,128); // regle la transparence
	SDL_SetTextureAlphaMod(texture_rond_jaune,168); // regle la transparence
    synchro=0;

    pthread_create( &thread1, NULL, server_function, NULL);

	// sendMessageToServer("X 0 23\n");

    while (!quit)
    {
        //SDL_WaitEvent(&event);

	if (SDL_PollEvent(&event)) // voit si y a un evenement disponible
	{
		//printf("un event\n");
        	switch (event.type)
        	{
            		case SDL_QUIT:// si je quitte l'applcation
                		quit = 1;
    				pthread_cancel(thread1);
                		break;
			case  SDL_MOUSEBUTTONDOWN: // si je clique sur une case
				SDL_GetMouseState( &mx, &my ); //récupere les coordonnées
				position=0,cpt=0; //initialise les valeurs à 0

				if(mx>1024){	// si on clique sur la case connexion
					printf("%d",connecte);
					if(connecte == 0){	//si l'on n'est pas encore connecté
						printf("Connexion au réseau\n");

						// On inscrit le message dans messageServer selon le protocole utilisé : C <adresseIp> <port> <nomJoueur>
						strcat(messageServer,"C ");
						strcat(messageServer,argv[3]);//IP
						strcat(messageServer," ");
						strcat(messageServer,argv[4]);//port
						strcat(messageServer," ");
						strcat(messageServer,argv[5]);//nomJoueur

						sendMessageToServer(messageServer,argv[2],argv[1]); // on envoie le msg au serveur
						strcpy(messageServer, "");

					}
				}
				else{
				while(position ==0 && cpt < 150){
					if( 	( (tabPoint[cpt].x + 15 > mx ) && ( tabPoint[cpt].x - 15 < mx) ) //On vérifie ici si l'on a cliqué sur une case(dim 30*30)
					      &&( (tabPoint[cpt].y + 15 > my ) && ( tabPoint[cpt].y - 15 < my) ) ) {
							position = cpt+1;
							printf("Le condé est sur la case %d\n",position); // affiche la case selectionné
							if(jack == 1){
                                if (cmp < 3 ){ // choix des quatre case en debut de partie
								strcat(messageServer,"X ");// On inscrit le message selon le protocole : X <Case>
								sprintf(positionC, "%d", position);
								strcat(messageServer,positionC);

								sendMessageToServer(messageServer,argv[2],argv[1]); // on envoie le msg au serveur
								strcpy(messageServer, "");
								cmp ++ ;
                                }
                                else{

                                strcat(messageServer,"J ");// On inscrit le message selon le protocole : J <Case> <name> (pour envoyer la position de Jack)
								sprintf(positionC, "%d ", position);
								strcat(messageServer,positionC);
								sprintf(positionC, "%d ", position);
								strcat(messageServer,positionC);

								strcat(messageServer,argv[5]);
								sendMessageToServer(messageServer,argv[2],argv[1]); // on envoie le msg au serveur
								strcpy(messageServer, "");

                                }
							}
							if(police == 1){
							strcat(messageServer,"P ");// On inscrit le message selon le protocole : P <Case> <name> (pour envoyer la position de policier)
								sprintf(positionC, "%d ", position);
								strcat(messageServer,positionC);
                                sprintf(positionC, "%d ", position);
								strcat(messageServer,positionC);

								strcat(messageServer,argv[5]);
								sendMessageToServer(messageServer,argv[2],argv[1]); // on envoie le msg au serveur
								strcpy(messageServer, "");
							}


					      }

                    }

				}



				cpt++; //parcourt le tableau de point

				if(cpt==150)
					printf("tu es nul tu n'as pas reussi a cliquer sur une case !\n");
				//tabPoint[cpt_point].x = mx;
				//tabPoint[cpt_point].y = my;
				 }
			}
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_LEFT: {
						int i;
						//for(i = 0 ; i < cpt_point ; i++)
							//printf("tabPoint[%d].x=%d ; tabPoint[%d].y=%d;\n", i,tabPoint[i].x,i,tabPoint[i].y);

				}
					break;
        	}

        }

	if (synchro==1)
	{
			//sscanf(buffer,"%s %d %d",couleur, &x,&y);
			//printf("%s bouge son pion en %d, %d\n", couleur, x,y);
   		pthread_mutex_lock( &mutex );

			synchro=0;
   		pthread_mutex_unlock( &mutex );
	}
		//recopie dans l'ordre de superposition des bitmaps
        SDL_Rect dstrect = { 0, 0, 1024, 1024 };  //coordonnees x y et taille
        SDL_Rect dstrect_connexion = { 1024, 0, 128, 1024 };  //coordonnees x y et taille
        //SDL_Rect srcrect_pawn = { 0, 0, 87, 131 };

        SDL_Rect dstrect_rond = { 296, 318 , 40, 40 };
        SDL_Rect dstrect_rond_jaune = { x, y , 40, 40 };
			// inclusion d'une image dans une autre si on mets NULL,NULL en arg de RENDER_COPY
        SDL_RenderCopy(renderer, texture, NULL, &dstrect); // On met en arg le rectangle dans lequel il y a les coordonnées et la taille
        SDL_RenderCopy(renderer, texture_connexion, NULL, &dstrect_connexion); // On met en arg le rectangle dans lequel il y a les coordonnées et la taille
        //SDL_RenderCopy(renderer, texture, &srcrect, NULL);
        //SDL_RenderCopy(renderer, texture, NULL, NULL);

        SDL_RenderCopy(renderer, texture_rond, NULL, &dstrect_rond);
        SDL_RenderCopy(renderer, texture_rond_jaune, NULL, &dstrect_rond_jaune);
	//printf("\n%d\n",numJoueur);
	if(numJoueur >=0){
		if (copie == 0){
        copie++;
		strcpy(pion,"");
		sprintf(numJoueurC, "%d", numJoueur);
		strcat(pion,"pawn_");
		strcat(pion,numJoueurC);
		strcat(pion,".bmp");
		//printf("\n%s\n",pion);
	}
	    	SDL_Surface * pawn = SDL_LoadBMP(pion); // charge l'image du pion
	   	SDL_SetColorKey( pawn, SDL_TRUE, SDL_MapRGB( pawn->format, 255, 255, 255 ) ); // Définit la couleur transparente(le noir)
	   	SDL_Texture * texture_pawn = SDL_CreateTextureFromSurface(renderer, pawn);// pareil pour le pion
		SDL_Rect dstrect_pawn = { mx-22, my-50, 40, 60 };
		SDL_RenderCopy(renderer, texture_pawn, NULL, &dstrect_pawn);
	}

        SDL_RenderPresent(renderer);


    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
