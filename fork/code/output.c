/**
 *
 *Fonction permettant de lire une chaîne de caractères pour ensuite l'insérer dans un fichier.
 *@author Ludovic Taffin
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "lecture.h"

void output(char *stringFromStudent){
    FILE* readingFile = NULL;
    
    //Création d'un dossier par student
    char *file = "sortieStudent.txt";
    //Voir comment la structure Inginious gère les fichiers des étudiants.
    readingFile = fopen(file,"a");
    if(readingFile != NULL){
        fputs(stringFromStudent, readingFile);
        fputc('\n',readingFile);
        fclose(readingFile);    
    }else{
        //message d'erreur
        printf("Your output is unreadable");
    }
    
}
