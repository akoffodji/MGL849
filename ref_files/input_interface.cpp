/*! \file input_interface.cpp */
//
//
//  
//
//  Created by Jean Jacques Akoffodji on 2015-11-07.
//
//

#include "input_interface.hpp"

/**
 * \fn photo_status()
 *
 * \brief Retourne VRAI s'il y a de la lumière, FAUX sinon
 */
bool photo_status()
{
    int led;
    std::string ledPath = LED_PATH;
    std::string exportPath = "/sys/class/gpio/export";
    std::string directPath = "/sys/class/gpio/gpio60/direction";
    
    
    std::ofstream fichier2(exportPath.c_str(), std::ofstream::out);  // ouverture en écriture avec effacement du fichier ouvert
    if(fichier2)
    {
        fichier2 << 60;
        fichier2.close();
    }
    else
    {
        std::cout << "Impossible d'ouvrir le fichier export!" << std::endl;
    }
    fichier2.clear();
    
    std::ofstream fichier1(directPath.c_str(), std::ofstream::out);  // ouverture en écriture avec effacement du fichier ouvert
    if(fichier1)
    {
        fichier1 << "in";
        fichier1.close();
    }
    else
    {
        std::cout << "Impossible d'ouvrir le fichier direction!" << std::endl;
    }
    fichier1.clear();
    
    std::ifstream fichier(ledPath.c_str(), std::ifstream::in);  // ouverture en lecture avec effacement du fichier ouvert
    if(fichier)
    {
        fichier >> led;
        fichier.close();
    }
    else
    {
        std::cout << "Impossible d'ouvrir le fichier led!" << std::endl;
    }
    fichier.clear();
    if (led < LED_THRESHOLD)
        return true;
    else
        return false;
}

/**
 * \fn button_status()
 *
 * \brief Retourne VRAI si le button est appuyé, FAUX sinon
 */
bool button_status()
{
    int not_push;
    std::string pushPath = PUSH_PATH;
    
    std::ifstream fichier(pushPath.c_str(), std::ifstream::in);  // ouverture en lecture avec effacement du fichier ouvert
    if(fichier)
    {
        fichier >> not_push;
        fichier.close();
    }
    else
    {
        std::cout << "Impossible d'ouvrir le fichier button!" << std::endl;
    }
    fichier.clear();
    if (not_push == 0)
        return true;
    else
        return false;
}

/**
 * \fn convert_commande(commande &une_commande, std::string com)
 *
 * \brief Converti une chaine de caractères pour faciliter l'envoi par TCP/IP
 */
void convert_commande(commande &une_commande, std::string com)
/**
 * @param une_commande la structure contenant IDOWN, READY ou PUSHB
 * @param com la chaîne de caractère à convertir
 */
{
    une_commande.texte[0] = com[0];
    une_commande.texte[1] = com[1];
    une_commande.texte[2] = com[2];
    une_commande.texte[3] = com[3];
    une_commande.texte[4] = com[4];
    une_commande.texte[5] = '\0';
}

/**
 * \fn choose_commande(commande &une_commande, bool &push)
 *
 * \brief Permet de sélectionner la bonne commande selon l'intensité lumineuse et la position du boutton
 */
void choose_commande(commande &une_commande, bool &push)
{
/**
 * @param une_commande la structure contenant IDOWN, READY ou PUSHB
 * @param push vaut VRAI si le boutton est appuyé, FAUX sinon
 */
    // On regarde s'il y a de la lumière
    if (photo_status())
    {
        if (button_status())
        {
        
            
            convert_commande(une_commande,"READY");
            push = true;

        }
        else
        {
            if (push == true)
            {
            convert_commande(une_commande,"PUSHB");
            }
            else
            {
             convert_commande(une_commande,"READY");
            }
            push = false;
        }
    }
    else
    {
        convert_commande(une_commande,"IDOWN");
    }
}

/**
 * \fn currentDateTime()
 *
 * \brief Retourne l'heure actuelle au format YYYY-mm-dd_HH_MM_SS
 */
const std::string currentDateTime()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d_%H_%M_%S", &tstruct);
    
    return buf;
}

/**
 * \fn commande_status(commande &current_command, commande &new_command)
 *
 * \brief Donne le status actuelle du streaming
 */
void commande_status(commande &current_command, commande &new_command)
/**
 * @param current_command la commande actuelle
 * @param new_command la commande précédente
 */
{
    std::string current_com, new_com;
    current_com.assign(current_command.texte);
    new_com.assign(new_command.texte);
    
    if (current_com != new_com)
    {
        if (new_com == "IDOWN")
        {
            std::cout << "Il n'y a pas de lumière on arrête le streaming" << std::endl;
        }
        else if (new_com == "READY" && current_com != "PUSHB")
        {
            std::cout << "Il y a de la lumière on démarre le streaming" << std::endl;
        }
        else
        {
        }
        current_command = new_command;
    }
}


/**
 * \fn cpu_monitoring(std::string governor)
 *
 * \brief Permet d'ajuster la puissance du CPU
 */
bool cpu_monitoring(std::string governor)
/**
 * @param governor le mode d'opération du CPU
 */
{
    std::string cpuPath;
    cpuPath = FREQ_PATH;
    
    std::ofstream fichier2(cpuPath.c_str(), std::ofstream::out);  // ouverture en écriture avec effacement du fichier ouvert
    if(fichier2)
    {
        if (governor == "powersave" || governor == "performance")
        {
            fichier2 << governor;
            fichier2.close();
            fichier2.clear();
            return true;
        }
        else
        {   fichier2.close();
            fichier2.clear();
            return false;
        }
        
    }
    else
    {
        std::cout << "Impossible d'ouvrir le fichier export!" << std::endl;
        fichier2.clear();
        return false;
    }
    
}


/**
 * \fn cpu_status()
 *
 * \brief Permet de connaître le mode d'opération actuel duCPU
 */
std::string cpu_status()
{
    std::string cpuPath;
    cpuPath = FREQ_PATH;
    
    std::ifstream fichier2(cpuPath.c_str(), std::ifstream::in);  // ouverture en écriture avec effacement du fichier ouvert
    if(fichier2)
    {
        fichier2 >> cpuPath;
        fichier2.close();
        fichier2.clear();
        return cpuPath;
        
    }
    else
    {
        std::cout << "Impossible d'ouvrir le fichier scaling_governor!" << std::endl;
        fichier2.clear();
        return "error";
    }
    
}