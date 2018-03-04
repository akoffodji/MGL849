#include "BME280.c"
#include "SocketImplementation.c"


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
