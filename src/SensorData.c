#include <sys/time.h>
#include <sys/resource.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include "BME280.c"
#define LISTEN_BACKLOG 50

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

void main()
{
	int socketBME;
    struct charValue val;
    
    socketBME = socketConfiguration(4598);
    
    setpriority(PRIO_PROCESS, 0, -14);
    
    puts("Priority set to -14");

    while(1)
    {
        sleep (3);
        val = dataReader();
        send(socketBME, &val, sizeof(val),0);
        
	}

	close(socketBME);
}
