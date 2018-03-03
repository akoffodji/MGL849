#include <sys/time.h>
#include <sys/resource.h>
#include <semaphore.h>
#include <pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include "BME280.c"
#define LISTEN_BACKLOG 50

struct charValue dataBME280;
double globalPower;
double temperatureDesired;
sem_t mutex;
int socketAffichage;
//boolean temperatureChange;

struct socketInt
{
    int Client;
    int Server;
};

int socketConfiguration(int PORT)
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

struct socketInt configuration_server(int PORT)
/**
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


void sendTempD (int socket_desc)
{
    char message[12];
    
    sprintf(message, "TD%.2f\n",temperatureDesired);
    puts(message);
    if( send(socket_desc , &message , strlen(message) , 0) < 0)
    {
        puts("Temperature Desired Send failed");
    }
    
    sprintf(message, "PW%.2f\n",globalPower);
    puts(message);
    if( send(socket_desc , &message , strlen(message) , 0) < 0)
    {
        puts("Power Send failed");
    }

}


void* socketBME280(void * x)
{
    char message[30];
    struct socketInt socketBME;
    socketBME = configuration_server(4598);
    
    while (1)
    {
        
        sleep(3);
        sem_wait(&mutex);
        if (recv(socketBME.Client, &dataBME280, sizeof(dataBME280), 0) < 0)
        {
            printf("erreur au recv() data BME280\n");
        }
        sprintf(message, "Temperature mesured %.2f\n",dataBME280.Temp);
        printf(message);
        sprintf(message, "Humidity mesured %.2f\n",dataBME280.Humidity);
        printf(message);
        sprintf(message, "Pressure mesured %.2f\n",dataBME280.Pressure);
        printf(message);
        sem_post(&mutex);
    }
}


void* affichageBME280(void * x)
{
    char message[15];
    //int socketBME;
    //socketBME = socketConfiguration(1237);
    
    while (1)
    {
        
        sleep(3);
        sem_wait(&mutex);
        sprintf(message, "TP%.2f\n",dataBME280.Temp);
        puts(message);
        if( send(socketAffichage , &message , strlen(message) , 0) < 0)
        {
            puts("Temperature ambiante Send failed");
        }

        sprintf(message, "PR%.2f\n",dataBME280.Pressure);
        if( send(socketAffichage , &message , strlen(message) , 0) < 0)
        {
            puts("Pressure Send failed");
        }
        
        sprintf(message, "HU%.2f\n",dataBME280.Humidity);
        if( send(socketAffichage , &message , strlen(message) , 0) < 0)
        {
            puts("Humidity Send failed");
        }
        sem_post(&mutex);

    }
}

void* affichageTempDesired(void * x)
{
    char message[15];
    
    while (1)
    {
        
        sleep(3);
        sem_wait(&mutex);
        
        sprintf(message, "TD%.2f\n",temperatureDesired);
        puts(message);
        if( send(socketAffichage , &message , strlen(message) , 0) < 0)
        {
            puts("Temperature ambiante Send failed");
        }
        
        sem_post(&mutex);
        
    }
}


void powerCalculation()
{
    globalPower = ((temperatureDesired-dataBME280.Temp)/6)*100;
    
    if (globalPower < 0 )
    {
        globalPower = 0;
    }
    
    if (globalPower > 100 )
    {
        globalPower = 100;
    }
}

void* socketTempDesired(void * x)
{
    char message[30];
    struct socketInt socketTemp;
    double oneTemp;
    socketTemp = configuration_server(4599);
    
    
    sleep(1);
    
    while (1)
    {
        
        if (recv(socketTemp.Client, &oneTemp, sizeof(oneTemp), 0) < 0)
        {
            printf("erreur au recv() data Temp desired\n");
        }
        sprintf(message, "Temperature desired %.2f\n",oneTemp);
    
        printf(message);
        
        sem_wait(&mutex);
        
        temperatureDesired =oneTemp;
        powerCalculation();
        sendTempD(socketAffichage);
        
        sem_post(&mutex);
    }
}





void main()
{
    struct charValue oneData;
    int Server, Client;
    struct socketInt socketTemp, socketBME;
    double temp_desired;
    double powerNeed;
    char message[30];
    struct charValue val;
    
    pthread_t threadBME;
    pthread_t threadTempD;
    pthread_t threadAffichage;
    pthread_t threadAffichageTempD;
    
    sem_init(&mutex,0,1);
    
    socketAffichage = socketConfiguration(1234);
    
    pthread_create(&threadBME,NULL,socketBME280,NULL);
    pthread_create(&threadTempD,NULL,socketTempDesired,NULL);
    pthread_create(&threadAffichage,NULL,affichageBME280,NULL);
    pthread_create(&threadAffichageTempD,NULL,affichageTempDesired,NULL);
    
    pthread_join(threadBME,NULL);
    pthread_join(threadTempD,NULL);
    pthread_join(threadAffichage,NULL);
    pthread_join(threadAffichageTempD,NULL);

}
