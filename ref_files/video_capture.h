/*! \file video_capture.h */

//  video_capture.h
//  
//
//  Created by Jean Jacques Akoffodji on 2015-11-03.
//
//

#ifndef video_capture_h
#define video_capture_h

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


/**
 \struct menu
 menu est une structure qui permet de gérer les paramètres de sélection#include "video_capture.h" de la résolution de capture de la caméra.
**/

struct menu {
    /**
     \var good_selection
     \brief Le statut de la réponse de l'utilisateur après avoir sélectionné la résolution
     **/
    bool good_selection;
    
    /**
     \var selection
     \brief La sélection faite par l'usager parmi celles proposées pour le choix de la résolution
     **/
    int selection;
    
    /**
     \var table
     \brief Ce tableau contient la liste des résolutions et des vitesses d'enregistrement pour chacune des 13 options disponibles
     **/
    int table[12][12];
};

void init_resolution(menu &un_menu);

void display_menu(menu &un_menu);

void find_rate(int &frame_rate, int &nb_frame, menu &un_menu);

int camera_status(class cv::VideoCapture &capture);

void get_property(class cv::VideoCapture &capture, double &width, double &height);

int video_status(class cv::VideoWriter &video);

int capture_frame(class cv::Mat &frame, class cv::VideoCapture &capture);

void save_video(class cv::Mat &frame, class cv::VideoWriter &video, int &i);

int Livrable_1();

void init_capture(class cv::VideoCapture & capture, menu &un_menu);

void rate_manager(class cv::VideoCapture & capture, int increase_resolution, menu &un_menu);

#endif /* video_capture_h */
