/*! \file Interface.c */

/**
 * \file Interface.c
 * \author Jean Jacques Akoffodji
 * \date 03 Mars 2018
 * \brief Fichier qui contient la fonction main du programme principale et les fonctions utilisées pour les threads
 */

#include <semaphore.h>
#include <pthread.h>
#include <sched.h>
#include<unistd.h>
#include<string.h>
#include <sys/types.h>
#include "BME280.c"
#include "SocketImplementation.c"

/**
 * \var struct charValue dataBME280
 * \brief Variable globale contenant les données environnementales mesurées.
 */
struct charValue dataBME280;


/**
 * \var double globalPower
 * \brief Variable globale pour la puissance calculée.
 */
double globalPower;


/**
 * \var double temperatureDesired
 * \brief Variable globale pour la température entrée par l'utilisateur.
 */
double temperatureDesired;


/**
 * \var sem_t mutex
 * \brief Variable globale pour la protection des ressources partagées par les threads.
 */
sem_t mutex;


/**
 * \var int socketAffichage
 * \brief Variable globale contenant le socket connecté au programme d'affichage.
 */
int socketAffichage;



/**
 * \fn void sendTempD (int socket_desc)
 * \brief Fonction d'envoi de la température désirée et de la puissance calculée au programme d'affichage.
 */
void sendTempD (int socket_desc)
/**
 * @param socket_desc le socket client connecté au programme d'affichage
 */
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


/**
 * \fn void* socketBME280(void * x)
 * \brief Fonction associée au thread d'acquisition des données environnementales.
 */
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

/**
 * \fn void* affichageBME280(void * x)
 * \brief Fonction associée au thread d'envoi périodique des données environnementales au programme d'affichage.
 */
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


/**
 * \fn void* affichageTempDesired(void * x)
 * \brief Fonction associée au thread d'envoi périodique de la température désirée au programme d'affichage.
 */
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



/**
 * \fn void powerCalculation()
 * \brief Fonction de calcul de la puissance nécessaire.
 */
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


/**
 * \fn void* socketTempDesired(void * x)
 * \brief Fonction associée au thread de reception de la température désirée entrée par l'utilisateur.
 */
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
