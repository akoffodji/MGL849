# MGL849 LABORATOIRE 1

---
### Clonage du répertoire git et compilation

1. Ouvrir un terminal dans le répertoire de votre choix sur le Raspberry PI

2. Faire les commandes suivantes:

        $ git clone https://akoffodji@bitbucket.org/akoffodji/mgl849.git
		$ cd mgl849/
        $ cd src/
		$ gcc SensorData.c -o excecutableSensor
		$ gcc DesiredTemp.c -o excecutableUser
		$ gcc -pthread Interface.c -o executableInterface

---

### Lancement du programme d'affichage

1. Ouvrir un terminal dans le répertoire précédemment créé

2. Faire les commandes suivantes:

        $ cd java/
		$ java -jar Lab1Afficheur.jar

---

### Lancement du programme d'interface principal

1. Ouvrir un terminal dans le répertoire précédemment créé

2. Faire les commandes suivantes:

        $ cd src/
		$ ./executableInterface


---

### Lancement du programme d'utilisateur

1. Ouvrir un terminal dans le répertoire précédemment créé

2. Faire les commandes suivantes:

        $ cd src/
		$ ./excecutableUser


---

### Lancement du programme pour le BME280

1. Ouvrir un terminal dans le répertoire précédemment créé

2. Faire les commandes suivantes:

        $ cd src/
		$ ./excecutableSensor

---