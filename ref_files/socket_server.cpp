/*! \file socket_server.cpp */
//
//  
//  
//
//  Created by Jean Jacques Akoffodji on 2015-11-05.
//
//

#include "socket_server.hpp"
#include <opencv2/opencv.hpp>
#include "video_capture.h"
#include "input_interface.hpp"

using namespace std;
using namespace cv;


/**
 * \fn configuration_server(int &Server, int &Client, int PORT)
 *
 * \brief Permet de configurer le socket du Serveur
 */
void configuration_server(int &Server, int &Client, int PORT)
/**
 * @param Server la caracterisation du socket Serveur
 * @param Client la caracterisation du socket Client qui se connecte au serveur
 * @param PORT le port sur lequel on se connecte
 */
{
	  int socket_error; 
	  struct sockaddr_in server_ad, client_ad;
	  socklen_t server_size, client_size;

	  Server = socket(AF_INET, SOCK_STREAM, 0);
	  if (Server < 0)
	  {
	      printf("erreur de creation du socket\n");
	      exit(1);
	  }
	  
	  server_size = sizeof(server_ad);
	  client_size = sizeof(client_ad);
	  
	  bzero((char *)&server_ad,sizeof(server_ad));
	  server_ad.sin_family      = AF_INET;
	  server_ad.sin_addr.s_addr = INADDR_ANY;
	  server_ad.sin_port = htons(PORT);
	  

	  bzero((char *)&client_ad,sizeof(client_ad));
	  client_ad.sin_family      = AF_INET; 
	  client_ad.sin_addr.s_addr = INADDR_ANY;
	  client_ad.sin_port = htons(PORT);
	  
	  
	  if (bind(Server, (struct sockaddr *)&server_ad, sizeof(server_ad)) < 0)
	  {
	      printf("erreur au bind()\n");
	      exit(1);
	  }
	  
	  
	  if (listen(Server, LISTEN_BACKLOG) < 0)
	  {
	      printf("erreur au listen()\n");
	      exit(1); 
	  }
	  
	    printf("Attente d'une connexion du client\n");
	    Client = accept(Server, (struct sockaddr *)&client_ad, &client_size);
	    printf("Un client se connecte avec la socket %d de %s:%d\n \n", Client, inet_ntoa(client_ad.sin_addr),htons(client_ad.sin_port)); 
}

/**
 * \fn ServerLivrable_2()
 *
 * \brief Permet de gérer le socket du Serveur pour le Livrable 2
 */
int ServerLivrable_2()
{
      int Server, Client, socket_error, compteur; // main
	  taille une_taille; // main
	  double height, width; // main
	  int nb_frame = 0; // main
	  int frame_rate = 7; // main
	  menu un_menu; // main
	  un_menu.selection = 0; // main
	  un_menu.good_selection = false; // main
	  int image_size; // main
	  reponse une_reponse;// main
	  string decision = "OK"; // main
      VideoCapture capture;
      
       // Configuration des socket client et Server
        configuration_server(Server,Client,PORT_4099);
    
    // Fin config socket Client & Server
    init_resolution(un_menu);
    
    while (un_menu.good_selection == false)
    {
        display_menu(un_menu);
    }
    
    
    
    // Configuration of the camera
    init_capture(capture, un_menu);
    
    // Get the properties from the camera
    get_property(capture, width, height);
	
	// Create a matrix to keep the retrieved frame
	Mat frame;
	
	compteur = 0;
	
	// Debut de l'envoie des frames au client
	while (decision == "OK")
	{
	      capture_frame(frame, capture);
	      
	      image_size = frame.total()*frame.elemSize();

	      une_taille.x = width; une_taille.y = height;
		
	      if (send(Client, &une_taille, sizeof(une_taille), 0) < 0)
	      {
		printf("erreur au sent()\n");
		exit(1);
	      }
	           
	      if (send(Client, frame.data, image_size, 0) < 0)
	      {
		printf("erreur au sent()\n");
		exit(1);
	      }
	         
	      if (recv(Client, &une_reponse, sizeof(une_reponse), 0) < 0)
	      {
		  printf("erreur au recv()\n");
		  exit(1);
	      }
	      decision.assign(une_reponse.texte);
	      if (compteur == 0)
	      {
		  cout << "Le Client envoie la reponse -" << decision;
		  cout << "- , le streaming peut continuer\n";
	      }
	      compteur = 1;
	      
	}
    
	cout << "Le Client envoie la reponse -" << decision;
	cout << "- , le streaming doit s'arreter\n";
	
	// Fin
	close(Server);
	printf("Socket Server Fermer\n");
	return 0;

}

/**
 * \fn ServerLivrable_3()
 *
 * \brief Permet de gérer le socket du Serveur pour le Livrable 3
 */
int ServerLivrable_3()
{
    int Server, Client, Server_i, Client_i, socket_error; // main
    taille une_taille; // main
    double height, width; // main
    int nb_frame = 0; // main
    int frame_rate = 7; // main
    menu un_menu; // main
    un_menu.selection = 0; // main
    un_menu.good_selection = false; // main
    int image_size; // main
    reponse une_reponse;// main
    string decision = "IDOWN"; // main
    commande une_commande, current_command; // ???
    bool streaming = false;
    VideoCapture capture;
    bool push = false;
    int compteur = 0;
    string appel_client = "OK";
    
    // Configuration des socket client et Server
    configuration_server(Server,Client,PORT_4098);
    configuration_server(Server_i,Client_i,PORT_4099);
    
    convert_commande(current_command,"RESET");
    
    // Fin config socket Client & Server
    init_resolution(un_menu);
    
    while (un_menu.good_selection == false)
    {
        display_menu(un_menu);
    }
    
    while (appel_client == "OK")
    {
    
        choose_commande(une_commande, push);
        
        commande_status(current_command, une_commande);
    
        // Envoie de l'une des trois messages au client
        if (send(Client, &une_commande, sizeof(une_commande), 0) < 0)
        {
            printf("erreur au sent()\n");
            exit(1);
        }
        
        // On peut maintenant envoyer les images:
        decision.assign(une_commande.texte);
        
        if (decision == "IDOWN")
        {
            if (streaming == true)
            {
                capture.release();
                streaming = false;
            }
        }
        else
        {
            if (streaming == false)
            {
                // Configuration of the camera
                capture.open(0);
                init_capture(capture,un_menu);
                streaming = true;
            }
            
            // Get the properties from the camera
            get_property(capture, width, height);
            
            // Create a matrix to keep the retrieved frame
            Mat frame;
            streaming = true;
            capture_frame(frame, capture);
            
            image_size = frame.total()*frame.elemSize();
            
            une_taille.x = width; une_taille.y = height;
            
            if (send(Client_i, &une_taille, sizeof(une_taille), 0) < 0)
            {
                printf("erreur au sent()\n");
                exit(1);
            }
	           
            if (send(Client_i, frame.data, image_size, 0) < 0)
            {
                printf("erreur au sent()\n");
                exit(1);
            }
            
            
            if (recv(Client_i, &une_reponse, sizeof(une_reponse), 0) < 0)
            {
                printf("erreur au recv()\n");
                exit(1);
            }
            appel_client.assign(une_reponse.texte);

        }    
    
    }
    
    cout << "Le Client envoie la reponse -" << appel_client;
    cout << "- , le streaming doit s'arreter\n";
    
    // Fin
    close(Server); close(Server_i);
    printf("Sockets Serveur Fermés\n");
    return 0;
}


/**
 * \fn ServerLivrable_4()
 *
 * \brief Permet de gérer le socket du Serveur pour le Livrable 4
 */
int ServerLivrable_4()
{
    int Server, Client, Server_i, Client_i, socket_error; // main
    taille une_taille; // main
    double height, width; // main
    int nb_frame = 0; // main
    int frame_rate = 7; // main
    menu un_menu; // main
    un_menu.selection = 0; // main
    un_menu.good_selection = false; // main
    int image_size; // main
    reponse une_reponse;// main
    string decision = "IDOWN"; // main
    commande une_commande, current_command; // ???
    bool streaming = false;
    VideoCapture capture;
    bool push = false;
    int compteur = 0;
    string appel_client = "OK";
    int increase_resolution = 1;
    
    // Configuration des socket client et Server
    configuration_server(Server,Client,PORT_4098);
    configuration_server(Server_i,Client_i,PORT_4099);
    
    cpu_monitoring("powersave");
    convert_commande(current_command,"RESET");
    
    // Fin config socket Client & Server
    init_resolution(un_menu);
    
    while (un_menu.good_selection == false)
    {
        display_menu(un_menu);
    }
    
    while (appel_client == "OK")
    {
        
        choose_commande(une_commande, push);
        
        commande_status(current_command, une_commande);
        
        // Envoie de l'une des trois messages au client
        if (send(Client, &une_commande, sizeof(une_commande), 0) < 0)
        {
            printf("erreur au sent()\n");
            exit(1);
        }
        
        // On peut maintenant envoyer les images:
        decision.assign(une_commande.texte);
        
        if (decision == "IDOWN")
        {
            if (streaming == true)
            {
                capture.release();
                cpu_monitoring("powersave");
                streaming = false;
            }
        }
        else if (decision == "READY" || decision == "PUSHB")
        {
            if (streaming == false)
            {
                cpu_monitoring("performance");
                // Configuration of the camera
                capture.open(0);
                init_capture(capture,un_menu);
                streaming = true;
            }
            
            // Get the properties from the camera
            get_property(capture, width, height);
            
            // Create a matrix to keep the retrieved frame
            Mat frame;
            streaming = true;
            capture_frame(frame, capture);
            
            image_size = frame.total()*frame.elemSize();
            
            une_taille.x = width; une_taille.y = height;
            
            if (send(Client_i, &une_taille, sizeof(une_taille), 0) < 0)
            {
                printf("erreur au sent()\n");
                exit(1);
            }
	           
            if (send(Client_i, frame.data, image_size, 0) < 0)
            {
                printf("erreur au sent()\n");
                exit(1);
            }
            
            // Reception du controle de la resolution
            if (recv(Client, &increase_resolution, sizeof(int), 0) < 0)
            {
                printf("erreur au recv()\n");
                exit(1);
            }
            if (increase_resolution != 1)
            {
                    un_menu.good_selection = false;
                    while (un_menu.good_selection == false)
                    {
                        display_menu(un_menu);
                    }
                    init_capture(capture,un_menu);
            }
            
            // Reception de la reponse de poursuite
            if (recv(Client_i, &une_reponse, sizeof(une_reponse), 0) < 0)
            {
                printf("erreur au recv()\n");
                exit(1);
            }

            appel_client.assign(une_reponse.texte);
            
        }
        else
        {
            cout << "Corrupted decision received" << endl;
        }
    }
    
    cout << "Le Client envoie la reponse -" << appel_client;
    cout << "- , le streaming doit s'arreter\n";
    
    // Fin
    close(Server); close(Server_i);
    printf("Sockets Serveur Fermés\n");
    return 0;
}
