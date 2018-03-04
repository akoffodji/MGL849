/**
 * \file SensorData.c
 * \author Jean Jacques Akoffodji
 * \date 03 Mars 2018
 * \brief Fichier qui contient la fonction main du programme dédié à l'aquisition de données du BME280
 */


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
