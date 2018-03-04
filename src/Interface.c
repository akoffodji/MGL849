#include <semaphore.h>
#include <pthread.h>
#include <sched.h>
#include<unistd.h>
#include<string.h>
#include <sys/types.h>
#include "BME280.c"
#include "SocketImplementation.c"

struct charValue dataBME280;
double globalPower;
double temperatureDesired;
sem_t mutex;
int socketAffichage;



int socketConfiguration(int PORT)
{
    int socket_desc;
    struct sockaddr_in server;
    
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
    
    struct sched_param param;
    
    param.sched_priority = 3;
    sched_setscheduler(0, SCHED_FIFO, &param);
    
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
    
    struct sched_param param;
    
    param.sched_priority = 2;
    sched_setscheduler(0, SCHED_FIFO, &param);
    
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
    
    struct sched_param param;
    
    param.sched_priority = 1;
    sched_setscheduler(0, SCHED_FIFO, &param);
    
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
    struct sched_param param;
    
    param.sched_priority = 4;
    sched_setscheduler(0, SCHED_FIFO, &param);
    
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
    char message[50];
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
