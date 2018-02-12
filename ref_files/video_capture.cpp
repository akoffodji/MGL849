/*! \file video_capture.cpp */
#include <iostream>
#include <opencv2/opencv.hpp>
#include "video_capture.h"

using namespace std;
using namespace cv;

/**
 \mainpage
 Ce programme simple sauvegarde 5 secondes de capture vidéo dans fichier nommé capture-liv1.avi. Il permet de sélectionner le nombre d’images par seconde et la résolution des images. La résolution pour la caméra doit être choisie parmi les suivantes :\n
 160 x 120 --
  176 x 144 --
  352 x 288 --
  320 x 176 --
  320 x 240 --
  800 x 600 --
  864 x 480 --
  960 x 544 --
  960 x 720 --
  1184 x 656 --
  1280 x 720 --
  1280 x 960\n
 Le format des images saisies sera le MJPG.\n
 Ce programme permet aussi de faire du streaminf via un protocole TCP/IP et de contrôler la résolution avec le clavier. Le streaming peut être arrêter en couvrant le capteur et un snapshot peut être pris en appuyant sur le bouton.\n
 Il permet enfin de faire de la reconnaissance de caractère sur une carte étudiante. \n
 
 \image html BBB.jpg
 
 **/


/**
 * \fn init_resolution(menu &un_menu)
 *
 * \brief Permet d'initialiser la variable menu qui contient les options pour le choix de la résolution
 */
void init_resolution(menu &un_menu)
/**
 * @param un_menu la structure contenant les données sur les différentes options
 */
{
    un_menu.table [0][0] = 160; un_menu.table [0][1] = 120; un_menu.table[0][2] = 13;
    un_menu.table [1][0] = 176; un_menu.table [1][1] = 144; un_menu.table[1][2] = 13;
    un_menu.table [2][0] = 320; un_menu.table [2][1] = 176; un_menu.table[2][2] = 13;
    un_menu.table [3][0] = 320; un_menu.table [3][1] = 240; un_menu.table[3][2] = 13;
    un_menu.table [4][0] = 352; un_menu.table [4][1] = 288; un_menu.table[4][2] = 13;
   // un_menu.table [5][0] = 432; un_menu.table [5][1] = 240; un_menu.table[5][2] = 13;
    un_menu.table [5][0] = 800; un_menu.table [5][1] = 600; un_menu.table[5][2] = 7;
    un_menu.table [6][0] = 864; un_menu.table [6][1] = 480; un_menu.table[6][2] = 7;
    un_menu.table [7][0] = 960; un_menu.table [7][1] = 544; un_menu.table[7][2] = 6;
    un_menu.table [8][0] = 960; un_menu.table [8][1] = 720; un_menu.table[8][2] = 6;
    un_menu.table [9][0] = 1184; un_menu.table [9][1] = 656; un_menu.table[9][2] = 6;
    un_menu.table [10][0] = 1280; un_menu.table [10][1] = 720; un_menu.table[10][2] = 5;
    un_menu.table [11][0] = 1280; un_menu.table [11][1] = 960; un_menu.table[11][2] = 4;
}


/**
 * \fn display_menu(menu &un_menu)
 *
 * \brief Permet d'afficher les options du menu et d'obtenir le choix de l'usager
 */
void display_menu(menu &un_menu)
/**
 * @param un_menu la structure contenant les données sur les différentes options
 */
{
    cout << " Sélectionner la résolution désirée en \n choisissant l'une des options allant de 0 à 11 \n";
    cout << "Pour 160 x 120, entrez 0 \n";
    cout << "Pour 176 x 144, entrez 1 \n";
    cout << "Pour 320 x 176, entrez 2 \n";
    cout << "Pour 320 x 240, entrez 3 \n";
    cout << "Pour 352 x 288, entrez 4 \n";
 //   cout << "Pour 432 x 240, entrez 5 \n";
    cout << "Pour 800 x 600, entrez 5 \n";
    cout << "Pour 864 x 480, entrez 6 \n";
    cout << "Pour 960 x 544, entrez 7 \n";
    cout << "Pour 960 x 720, entrez 8 \n";
    cout << "Pour 1184 x 656, entrez 9 \n";
    cout << "Pour 1280 x 720, entrez 10 \n";
    cout << "Pour 1280 x 960, entrez 11 \n";
    cout << "*******************************\n";
    cout << "Entrez votre sélection ici: ";
    
    cin >> un_menu.selection;
    
    if (un_menu.selection < 0 || un_menu.selection > 11) {
        cout << "Veuillez sélectionner la bonne résolution \n";
    } else {
        un_menu.good_selection = true;
    }
    
}

/**
 * \fn find_rate(int &frame_rate, int &nb_frame, menu &un_menu)
 *
 * \brief Permet de calculer le nombre de frames nécessaires pour un enregistrement de 5s
 */
void find_rate(int &frame_rate, int &nb_frame, menu &un_menu)
/**
 * @param frame_rate la vitesse d'enregistrement correspondant à la résolution choisie
 * @param nb_frame le nombre de frames nécessaires pour un enregistrement de 5s
 * @param un_menu la structure contenant les données sur les différentes options
 */
{
    frame_rate = un_menu.table[un_menu.selection][2];
    nb_frame = frame_rate*5;
}

/**
 * \fn camera_status(class cv::VideoCapture &capture)
 *
 * \brief Permet de savoir savoir si on peut capturer des images avec la caméra
 */
int camera_status(class cv::VideoCapture &capture)
/**
 * @param capture l'objet permettant de gérer l'enregistrement avec la caméra
 */
{
    if(!capture.isOpened())
    {
        cerr << "Could not create capture";
        return -1;
    }
    else
        return 0;
}


/**
 * \fn get_property(class cv::VideoCapture &capture, double &width, double &height)
 *
 * \brief Permet de connaître les paramètres d'enregistrement de la caméra
 */
void get_property(class cv::VideoCapture &capture, double &width, double &height)
/**
 * @param capture l'objet permettant de gérer l'enregistrement avec la caméra
 * @param width la largeur de l'image
 * @param height la hauteur de l'image
 */
{
    width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
}

/**
 * \fn video_status(class cv::VideoWriter &video)
 *
 * \brief Permet de savoir savoir si on peut sauvegarder les images capturées
 */
int video_status(class cv::VideoWriter &video)
/**
 * @param video l'objet permettant de gérer la sauvegarde des images
 */
{
    if(!video.isOpened())
    {
        cerr << "Could not create video.\n";
        return -1;
    }
    else
        return 0;
}


/**
 * \fn capture_frame(class cv::Mat &frame, class cv::VideoCapture &capture)
 *
 * \brief Permet de capture des images à partir de la caméra
 */
int capture_frame(class cv::Mat &frame, class cv::VideoCapture &capture)
/**
 * @param frame l'objet permettant de gérer les images provenant de la capture
 * @param capture l'objet permettant de gérer l'enregistrement avec la caméra
 */
{
    
    // Get frame from capture
    capture >> frame;
    
    // Check if the frame was retrieved
    if(!frame.data)
    {
        cerr << "Could not retrieve frame.";
        return -1;
    }
    else
        return 0;
    
}

/**
 * \fn save_video(class cv::Mat &frame, class cv::VideoWriter &video, int &i)
 *
 * \brief Permet de sauvegarder les images capturées de la caméra
 */
void save_video(class cv::Mat &frame, class cv::VideoWriter &video, int &i)
/**
 * @param frame l'objet permettant de gérer les images provenant de la capture
 * @param video l'objet permettant de gérer la sauvegarde des images
 * @param i le nombre de frame actuellement capturées et enregistrées
 */
{
    // Save frame to video
    video << frame;
    
    i = i+1;
}

/**
 * \fn Livrable_1()
 *
 * \brief Permet d'exécuter le programme du Livrable 1
 */
int Livrable_1()
{
    double height, width;
    int i = 0;
    int nb_frame = 0;
    int frame_rate = 7;
    menu un_menu;
    un_menu.selection = 0;
    un_menu.good_selection = false;
    
    init_resolution(un_menu);
    
    while (un_menu.good_selection == false)
    {
        display_menu(un_menu);
    }
    
    // Open the default camera
    VideoCapture capture(0);
    capture.set(CV_CAP_PROP_FRAME_WIDTH,un_menu.table[un_menu.selection][0]);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,un_menu.table[un_menu.selection][1]);
    
    // Calculating total of frame for 5 seconds
    find_rate(frame_rate, nb_frame,un_menu);
    
    
    // Check if the camera was opened
    camera_status(capture);
    
    
    // Get the properties from the camera
    
    get_property(capture, width, height);
    
    
    // Create a matrix to keep the retrieved frame
    Mat frame;
    
    
    // Create the video writer
    VideoWriter video("capture-liv1.avi",CV_FOURCC('M','J','P','G'),frame_rate, cvSize((int)width,(int)height) );
    
    
    // Check if the video was opened
    video_status(video);
    
    cout << "You will have 5 seconds recording" << endl;
    
    // Get the next frame until we get nb_frame for 5 second recording
    while(i < nb_frame)
    {
        capture_frame(frame, capture);
        save_video(frame, video, i);
    }
    
    cout << "Votre vidéo est enregistrée sous le nom capture-liv1.avi" << endl;
    // Exit
    return 0;
}

/**
 * \fn init_capture(class cv::VideoCapture & capture, menu &un_menu)
 *
 * \brief Permet d'initialiser les paramètre de la caméra
 */
void init_capture(class cv::VideoCapture & capture, menu &un_menu)
/**
 * @param capture l'objet permettant de gérer l'enregistrement avec la caméra
 * @param un_menu la structure contenant les données sur les différentes options
 */
{    
    // Open the default camera
    capture.set(CV_CAP_PROP_FRAME_WIDTH,un_menu.table[un_menu.selection][0]);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,un_menu.table[un_menu.selection][1]);
    
    // Check if the camera was opened
    camera_status(capture);
    
}

/**
 * \fn rate_manager(class cv::VideoCapture & capture, int increase_resolution, menu &un_menu)
 *
 * \brief Permet d'initialiser les paramètre de la caméra
 */
void rate_manager(class cv::VideoCapture & capture, int increase_resolution, menu &un_menu)
/**
* @param capture l'objet permettant de gérer l'enregistrement avec la caméra
 * @param increase_resolution perment changer la résolution
 * @param un_menu la structure contenant les données sur les différentes options
 */
{
    if (un_menu.selection > 0 && un_menu.selection < 11)
    {
        un_menu.selection = un_menu.selection + increase_resolution - 1;
        capture.release();
        capture.open(0);
        init_capture(capture,un_menu);
        
    }
    else if (un_menu.selection == 0)
    {
        if(increase_resolution > 0)
        {
            un_menu.selection = un_menu.selection + increase_resolution - 1;
            capture.release();
            capture.open(0);
            init_capture(capture,un_menu);
        }
    }
    else if (un_menu.selection == 11)
    {
        if(increase_resolution < 2)
        {
            un_menu.selection = un_menu.selection + increase_resolution - 1;
            capture.release();
            capture.open(0);
            init_capture(capture,un_menu);
        }
    }
    else
    {
        
    }
  //  if (un_menu.selection == 5)
   //     un_menu.selection = 4;
}

