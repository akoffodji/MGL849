/*! \file input_interface.hpp */
//
//  
//
//  Created by Jean Jacques Akoffodji on 2015-11-07.
//
//

#ifndef input_interface_hpp
#define input_interface_hpp

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <time.h>
#define LED_THRESHOLD 3900
#define LED_PATH "/sys/bus/iio/devices/iio\:device0/in_voltage0_raw"
#define PUSH_PATH "/sys/class/gpio/gpio60/value"
#define FREQ_PATH "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"

/**
 \struct commande
 commande est la structure qui permet de conserver la decision du socket Serveur
 **/
struct commande
{
    /**
     \var texte
     \brief La reponse du serveur "READY", "IDOWN" ou "PUSHB"
     **/
    char texte[6];
};


bool photo_status();
bool button_status();
void convert_commande(commande &une_commande, std::string com);

void choose_commande(commande &une_commande, bool &push);

const std::string currentDateTime();

void commande_status(commande &current_command, commande &new_command);

bool cpu_monitoring(std::string governor);

std::string cpu_status();

#endif /* input_interface_hpp */
