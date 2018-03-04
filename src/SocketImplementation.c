/**
 * \file SocketImplementation.c
 * \author Jean Jacques Akoffodji
 * \date 03 Mars 2018
 * \brief Fichier qui contient l'implémentation des fonctions de configuration pour le socket
 */
#include <sys/time.h>
#include <sys/resource.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr

#define LISTEN_BACKLOG 50



/**
 * \struct socketInt
 * \brief Structure de données pour la configuration du socket
 */
struct socketInt
{
    int Client; /*!< Entier correspondant au socket client. */
    int Server; /*!< Entier correspondant au socket serveur. */
};



/**
 * \fn int socketConfiguration(int PORT)
 * \brief Fonction de configuration d'un socket client en localhost
 */
int socketConfiguration(int PORT)
/**
 * @param PORT le port sur lequel on se connecte
 * /return socket_desc le socket client
 */
{
    int socket_desc;
    struct sockaddr_in server;
    
    
    
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    else
    {
        puts("Socket created");
    }
    
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    
    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
    }
    
    puts("Connected");
    
    return socket_desc;
}


/**
 * \fn struct socketInt configuration_server(int PORT)
 * \brief Fonction de configuration d'un socket serveur en localhost
 */
struct socketInt configuration_server(int PORT)
/**
 * @param PORT le port sur lequel on se connecte
 * /return unSocket la structure de données contenant les socket serveur et client
 */
{
    int socket_error, Client, Server;
    struct sockaddr_in server_ad, client_ad;
    socklen_t server_size, client_size;
    struct socketInt unSocket;
    
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
    
    unSocket.Client = Client;
    unSocket.Server = Server;
    
    return unSocket;
}
