#include "SocketImplementation.c"

void main()
{
	int socket_temp;
    
    socket_temp = socketConfiguration(4599);

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
    
            send(socket_temp, &d, sizeof(d),0);
            
		}
	}

	close(socket_temp);
}
