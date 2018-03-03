#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include "BME280.c"
#define LISTEN_BACKLOG 50

struct socketInt
{
    int Client;
    int Server;
};


struct socketInt configuration_server(int PORT)
/**
 * @param Server la caracterisation du socket Serveur
 * @param Client la caracterisation du socket Client qui se connecte au serveur
 * @param PORT le port sur lequel on se connecte
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

void dataSender()
{
    int socket_desc;
    struct sockaddr_in server;
    char message[12];
    struct charValue val;
    
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
    server.sin_port = htons( 1234 );
    
    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
    }
    
    puts("Connected");
    
    while(1)
    {
    val = dataReader();
    //Send some data
    sprintf(message, "TP%.2f\n",val.Temp);
    puts(message);
    if( send(socket_desc , &message , strlen(message) , 0) < 0)
    {
        puts("Temperature ambiante Send failed");
    }
    // puts("Temperature ambiante Send\n");
    
    //sleep (2);
    
    strcpy(message , "TD35.25\n");
    if( send(socket_desc , &message , strlen(message) , 0) < 0)
    {
        puts("Temperature Desired Send failed");
        return 1;
    }
    // puts("Temperature Desired Send\n");
    
    //sleep (2);
    
    strcpy(message ,"PW91.91\n");
    if( send(socket_desc , &message , strlen(message) , 0) < 0)
    {
        puts("Power Send failed");
    }
    // puts("Power Send\n");
    
    //sleep (2);
    
    sprintf(message, "PR%.2f\n",val.Pressure);
    if( send(socket_desc , &message , strlen(message) , 0) < 0)
    {
        puts("Pressure Send failed");
    }
    // puts("Pressure Send\n");
    
    //sleep (2);
    
    sprintf(message, "HU%.2f\n",val.Humidity);
    if( send(socket_desc , &message , strlen(message) , 0) < 0)
    {
        puts("Humidity Send failed");
    }
    // puts("Humidity Send\n");
        
        sleep(5);
    }
    
    //sleep (2);
    
    close(socket_desc);
}


void main()
{
    struct charValue oneData;
    //dataSender();
    
    int Server, Client;
    struct socketInt unSocket;
    double temp_desired;
    char message[30];
    
    // Configuration des socket client et Server
    unSocket = configuration_server(1717);
    
    Client = unSocket.Client;
    Server = unSocket.Server;
    
    while (1)
    {
    
        sleep(1);
        
        if (recv(Client, &temp_desired, sizeof(temp_desired), 0) < 0)
        {
            printf("erreur au recv()\n");
        }
        
        sprintf(message, "Temperature desired %.2f\n",temp_desired);
        
        printf(message);
    }
}
