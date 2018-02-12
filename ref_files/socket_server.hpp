/*! \file socket_server.hpp */
//
//  
//  
//
//  Created by Jean Jacques Akoffodji on 2015-11-05.
//
//

#ifndef socket_server_hpp
#define socket_server_hpp

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#define PORT_4098 4098
#define PORT_4099 4099
#define IP_192_168_7_2 "192.168.7.2"
#define LISTEN_BACKLOG 500

/**
 \struct taille
 taille est une structure qui permet de garder les dimensions d'une image
**/
struct taille
{
  /**
  \var x
  \brief La largeur de l'image
  **/
  double x;
  
  /**
  \var y
  \brief La hauteur de l'image
  **/
  double y;
};

/**
 \struct reponse
 reponse est la structure qui permet de conserver la reponse du serveur client
**/
struct reponse
{
  /**
  \var texte
  \brief La reponse du serveur client "OK" ou "QUIT"
  **/
  char texte[5];
};

int ServerLivrable_2();

void configuration_server(int &Server, int &Client, int PORT);
int ClientLivrable_2();

void configuration_client(int &Client, int PORT, char* IP);


int ServerLivrable_3();

int ClientLivrable_3();

int ServerLivrable_4();

int ClientLivrable_4();


#endif /* socket_server_hpp */
