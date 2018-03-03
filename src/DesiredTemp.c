#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr

int socketConfiguration()
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
    server.sin_port = htons( 1717 );
    
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
	int socket_temp;
    
    socket_temp = socketConfiguration();

    while(1)
    {
        double d;
		char temp[20];
		char input[20];

		printf("\nEnter desired temp [5.00, 30.00]\n");
		fgets(input, 20, stdin);

		d = atof(input);

		if(d < 5 || d > 30) {
			printf("\nTemp outside valid interval\n");
		} else {
			sprintf(temp, "%.2f", d);
			printf("\nSending temp: %s\n", temp);

			// TODO Send the temp
            
            send(socket_temp, &d, sizeof(d),0);
            
            
		}
	}

	close(socket_temp);
}
