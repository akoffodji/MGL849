/*! \file socket_client.cpp */
//
//  
//  
//
//  Created by Jean Jacques Akoffodji on 2015-11-05.
//
//

#include "socket_server.hpp"
#include "video_capture.h"
#include <opencv2/opencv.hpp>
#include "input_interface.hpp"

using namespace std;
using namespace cv;



/**
 * \fn configuration_client(int &Client, int PORT, char* IP)
 *
 * \brief Permet de configurer le socket du Client
 */
void configuration_client(int &Client, int PORT, char* IP)
/**
 * @param Client la caracterisation du socket Client qui veut se connecter
 * @param PORT le port sur lequel on se connecte
 * @param IP l'adresse IP du serveur
 */
{
      struct sockaddr_in client_ad;
      Client = socket(AF_INET, SOCK_STREAM, 0);
      if (Client < 0)
      { printf("erreur de creation du socket\n");
	  exit(1);
      }
      
      bzero((char *)&client_ad,sizeof(client_ad));
      client_ad.sin_family      = AF_INET;
      client_ad.sin_addr.s_addr = inet_addr(IP);
      client_ad.sin_port = htons(PORT);
      

      if (connect(Client, (struct sockaddr *)&client_ad, sizeof(client_ad)) < 0)
      {
	  printf("erreur au connect()\n");
	  exit(1);
      }
      
      printf("Connexion a %s sur le port %d\n", inet_ntoa(client_ad.sin_addr), htons(client_ad.sin_port));

}

/**
 * \fn ClientLivrable_2()
 *
 * \brief Permet de gérer le socket du Client pour le Livrable 2
 */
int ClientLivrable_2()
{
    // Declaration des variables
    int  Client, socket_error;
    taille une_taille;
    int bytes;
    int key = 0;
    reponse une_reponse;
    
     //Configuration du socket Client
     configuration_client(Client, PORT_4099, (char*)IP_192_168_7_2);

     // Debut de la reception des frames depuis le serveur
     while (key != 1048603 && key != 27)
     {
        if (recv(Client, &une_taille, sizeof(une_taille), 0) < 0)
	{
	    printf("erreur au recv()\n");
	    exit(1);
	}
      
      Mat frame(Size(une_taille.x,une_taille.y), CV_8UC3);
      int frame_size = frame.total()*frame.elemSize();
      uchar frame_data[frame_size];

	for (int i = 0; i < frame_size; i+= bytes)
	{
	  if((bytes = recv(Client, frame_data + i, frame_size - i, 0)) < 0)
	  {
	    printf("erreur au recv()\n");
	    exit(1);
	  }
	}
	
	Mat img(Size(une_taille.x,une_taille.y), CV_8UC3, frame_data);
	
	imshow("Stream_Client", img);
	
	key = waitKey(30);
	
	if (key != 1048603 && key != 27)
	{
	    une_reponse.texte[0] = 'O';
	    une_reponse.texte[1] = 'K';
	    une_reponse.texte[2] = '\0';
	    une_reponse.texte[3] = '\0';
	    une_reponse.texte[4] = '\0';
	    send(Client, &une_reponse, sizeof(une_reponse),0);
	}
	else
	{
	    une_reponse.texte[0] = 'Q';
	    une_reponse.texte[1] = 'U';
	    une_reponse.texte[2] = 'I';
	    une_reponse.texte[3] = 'T';
	    une_reponse.texte[4] = '\0';
	    send(Client, &une_reponse, sizeof(une_reponse),0);
	}
	
     }
    
     close(Client);
     printf("Socket Client fermer\n");
    return 0;
}


/**
 * \fn ClientLivrable_3()
 *
 * \brief Permet de gérer le socket du Client pour le Livrable 3
 */
int ClientLivrable_3()
{
    // Declaration des variables
    int  Client, Client_i, socket_error;
    taille une_taille;
    int bytes;
    int key = 0;
    reponse une_reponse;
    commande une_commande, current_command;
    string decision;
    bool streaming = false;
    pid_t pid;
    string nom_fichier;
    
    
    //Configuration du socket Client sur les deux ports
    configuration_client(Client, PORT_4098, (char*)IP_192_168_7_2);
    sleep(1);
    configuration_client(Client_i, PORT_4099, (char*)IP_192_168_7_2);
    
    convert_commande(current_command,"RESET");
    
    while (key != 1048603 && key != 27)
    {
        //Client recoit la commande du client
        if (recv(Client, &une_commande, sizeof(une_commande), 0) < 0)
        {
            printf("erreur au recv()\n");
            exit(1);
        }
        decision.assign(une_commande.texte);
    
        commande_status(current_command, une_commande);
        
        if (decision == "IDOWN")
        {
            if (streaming == true)
            {
                destroyWindow("Stream_Client");
                waitKey(30);
            }
            streaming = false;
        }
        else
        {
            streaming = true;
            if (recv(Client_i, &une_taille, sizeof(une_taille), 0) < 0)
            {
                printf("erreur au recv()\n");
                exit(1);
            }
            
            Mat frame(Size(une_taille.x,une_taille.y), CV_8UC3);
            int frame_size = frame.total()*frame.elemSize();
            uchar frame_data[frame_size];
            
            for (int i = 0; i < frame_size; i+= bytes)
            {
                if((bytes = recv(Client_i, frame_data + i, frame_size - i, 0)) < 0)
                {
                    printf("erreur au recv()\n");
                    exit(1);
                }
            }
            
            Mat img(Size(une_taille.x,une_taille.y), CV_8UC3, frame_data);
            
            imshow("Stream_Client", img);
            key = waitKey(30);
            
            if(decision == "PUSHB")
            {
                pid = fork();
                if (pid == 0)
                {
                    nom_fichier = "capture_" + currentDateTime() + ".png";
                    imwrite(nom_fichier, img);
                    cout << "Un snapchot est disponible sous le nom: " << nom_fichier << endl;
                    return EXIT_SUCCESS;
                }
                else if (pid > 0)
                {
                }
                else
                {
                    // fork failed
                    printf("fork() failed!\n");
                    return 1;
                }
            }

        }
        if (key != 1048603 && key != 27)
        {
            une_reponse.texte[0] = 'O';
            une_reponse.texte[1] = 'K';
            une_reponse.texte[2] = '\0';
            une_reponse.texte[3] = '\0';
            une_reponse.texte[4] = '\0';
            send(Client_i, &une_reponse, sizeof(une_reponse),0);
        }
        else
        {
            une_reponse.texte[0] = 'Q';
            une_reponse.texte[1] = 'U';
            une_reponse.texte[2] = 'I';
            une_reponse.texte[3] = 'T';
            une_reponse.texte[4] = '\0';
            send(Client_i, &une_reponse, sizeof(une_reponse),0);
        }


    }
    // Un petit délai pour éviter que le serveur recoive la réponse avant qu'on ne ferme le socket client.
    sleep(2);
    close(Client); close(Client_i);
    printf("Sockets Client fermés\n");
    return 0;
}


/**
 * \fn ClientLivrable_4()
 *
 * \brief Permet de gérer le socket du Client pour le Livrable 4
 */
int ClientLivrable_4()
{
    // Declaration des variables
    int  Client, Client_i, socket_error;
    taille une_taille;
    int bytes;
    int key = 0;
    reponse une_reponse;
    commande une_commande, current_command;
    string decision;
    bool streaming = false;
    pid_t pid;
    string nom_fichier;
    int increase_resolution = 1;
    
    //Configuration du socket Client sur les deux ports
    configuration_client(Client, PORT_4098, (char*)IP_192_168_7_2);
    sleep(1);
    configuration_client(Client_i, PORT_4099, (char*)IP_192_168_7_2);
    
    convert_commande(current_command,"RESET");
    
    while (key != 1048603 && key != 27)
    {
        //Client recoit la commande du client
        if (recv(Client, &une_commande, sizeof(une_commande), 0) < 0)
        {
            printf("erreur au recv()\n");
            exit(1);
        }
        decision.assign(une_commande.texte);
        
        commande_status(current_command, une_commande);
        
        if (decision == "IDOWN")
        {
            if (streaming == true)
            {
                destroyWindow("Stream_Client");
                waitKey(30);
            }
            streaming = false;
        }
        else
        {
            streaming = true;
            if (recv(Client_i, &une_taille, sizeof(une_taille), 0) < 0)
            {
                printf("erreur au recv()\n");
                exit(1);
            }
            
            Mat frame(Size(une_taille.x,une_taille.y), CV_8UC3);
            int frame_size = frame.total()*frame.elemSize();
            uchar frame_data[frame_size];
            
            for (int i = 0; i < frame_size; i+= bytes)
            {
                if((bytes = recv(Client_i, frame_data + i, frame_size - i, 0)) < 0)
                {
                    printf("erreur au recv()\n");
                    exit(1);
                }
            }
            
            Mat img(Size(une_taille.x,une_taille.y), CV_8UC3, frame_data);
            
            imshow("Stream_Client", img);
            key = waitKey(60);
            
            if(decision == "PUSHB")
            {
                pid = fork();
                if (pid == 0)
                {
                    nom_fichier = "capture_" + currentDateTime() + ".png";
                    imwrite(nom_fichier, img);
                    cout << "Un snapchot est disponible sous le nom: " << nom_fichier << endl;
                    return EXIT_SUCCESS;
                }
                else if (pid > 0)
                {
                }
                else
                {
                    // fork failed
                    printf("fork() failed!\n");
                    return 1;
                }
            }
            
        }
        
        // Envoie de la commande sur le changement de résolution
        if(key == 63232 || key == 63233)
        {
            increase_resolution = 2*(63233 - key);
        }
        
        send(Client, &increase_resolution, sizeof(int),0);
        
        //cout << increase_resolution << endl;
        
        increase_resolution = 1;
        
        // Envoie de la commande de poursuite
        if (key != 1048603 && key != 27)
        {
            une_reponse.texte[0] = 'O';
            une_reponse.texte[1] = 'K';
            une_reponse.texte[2] = '\0';
            une_reponse.texte[3] = '\0';
            une_reponse.texte[4] = '\0';
            send(Client_i, &une_reponse, sizeof(une_reponse),0);
        }
        else
        {
            une_reponse.texte[0] = 'Q';
            une_reponse.texte[1] = 'U';
            une_reponse.texte[2] = 'I';
            une_reponse.texte[3] = 'T';
            une_reponse.texte[4] = '\0';
            send(Client_i, &une_reponse, sizeof(une_reponse),0);
        }
    }
    // Un petit délai pour éviter que le serveur recoive la réponse avant qu'on ne ferme le socket client.
    sleep(2);
    close(Client); close(Client_i);
    printf("Sockets Client fermés\n");
    return 0;
}