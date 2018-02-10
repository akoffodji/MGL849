#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include "BME280.c"

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
    dataSender();
}
