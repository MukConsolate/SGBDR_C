#ifndef __ENREGISTREMENT_H
#define __ENREGISTREMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAMPS 10
#define MAX_ENREGISTREMENT 100
#define MAX_LONGUEUR 50

typedef struct _enregistrement Enregistrement; 
struct  _enregistrement
{

};
typedef struct _table TABLE;
struct _table {
    char *champs[MAX_CHAMPS];
    int nombre_champs;
} ;

typedef struct {
    char *nom;
    TABLE table;
} BaseDeDonnees;

void creer_bdd(const char *nom);
void creer_table(const char *nom_table, const char *fichier, char *champs[], int nombre_champs);
void inserer_donnees(const char *nom_table, const char *fichier, char *enregistrement[], int nombre_champs);
void charger_ligne_donnees_en_memoire(const char *nom_table, const char *fichier, int ligne);


Enregistrement * creerEnregistrement();

#endif