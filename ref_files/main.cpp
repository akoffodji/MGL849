/*! \file main.cpp */
//
//  main.cpp
//  
//
//  Created by Jean Jacques Akoffodji on 2015-11-07.
//
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include "video_capture.h"
#include "socket_server.hpp"
#include <stdio.h>

using namespace std;
using namespace cv;



/**
 * \fn main()
 *
 * \brief Permet de gérer l'exécution du livrable 1, 2, 3 & 4
 */
int main()
{
    int livrable, socket_type;
    cout << "Livrable: ";
    cin >> livrable;
    
    if (livrable == 1)
    {
        Livrable_1();
    }
    else if (livrable == 2)
    {
        cout << "Entrer 0 pour un lancer un Socket Serveur ou 1 pour un Socket Client:  ";
        cin >> socket_type;
        if (socket_type == 0) {
            ServerLivrable_2();
        } else {
            ClientLivrable_2();
        }
    }
    else if (livrable == 3)
    {
        cout << "Entrer 0 pour un lancer un Socket Serveur ou 1 pour un Socket Client:  ";
        cin >> socket_type;
        if (socket_type == 0) {
            ServerLivrable_3();
        } else {
            ClientLivrable_3();
        }
        
    }
    else if (livrable == 4)
    {
        cout << "Entrer 0 pour un lancer un Socket Serveur ou 1 pour un Socket Client:  ";
        cin >> socket_type;
        if (socket_type == 0) {
            ServerLivrable_4();
        } else {
            ClientLivrable_4();
        }
        
    }
     else
	cout << "Mauvaise sélection" << endl;
    return 0;
}
