#include "BME280.c"
#include "SocketImplementation.c"


void main()
{
	int socketBME;
    struct charValue val;
    
    socketBME = socketConfiguration(4598);

    while(1)
    {
        
        sleep(1);
        val = dataReader();
        send(socketBME, &val, sizeof(val),0);
        
	}

	close(socketBME);
}
