#ifndef __CHAMP_H
#define __CHAMP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHAMPS 10
#define MAX_TAILLE_NOM_CHAMPS 50


typedef struct _champ CHAMP; 
struct  _champ
{
 

};
typedef struct _table TABLE;
struct _table {
    char **champs;
    int nombre_champs;
} ;

void creer_bdd(const char *nom);
void creer_chemin(char *chemin, char *folder, char *file);
void creer_table(const char *nom_table, const char *fichier, char **champs, int nombre_champs);
void inserer_donnees(const char *nom_table, const char *fichier, char **donnees, int nombre_donnees);
void charger_ligne_donnees_en_memoire(const char *nom_table, const char *fichier, int ligne);
void selectionner_donnees(char *nom_bdd, char *nom_table);
void afficher_donnees(char *donnees);
void mettre_a_jour_donnees(char *nom_bdd, char *nom_table, int num_ligne, char *nouvelles_donnees[]);
void supprimer_donnees(char *nom_bdd, char *nom_table, int num_ligne);
void trier_donnees(char *nom_bdd, char *nom_table);

#endif
