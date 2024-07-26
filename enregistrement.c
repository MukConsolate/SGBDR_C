#include "enregistrement.h"

BaseDeDonnees bdd;

void creer_bdd(const char *nom) {
    bdd.nom = strdup(nom);
    bdd.table.nombre_champs = 0;
}

void creer_table(const char *nom_table, const char *fichier, char *champs[], int nombre_champs) {
    bdd.table.nombre_champs = nombre_champs;
    for (int i = 0; i < nombre_champs; i++) {
        bdd.table.champs[i] = strdup(champs[i]);
    }
    
    // Créer le fichier pour la table
    FILE *fp = fopen(fichier, "w");
    if (fp == NULL) {
        perror("Erreur lors de la création du fichier");
        return;
    }
    
    // Écrire les champs dans le fichier
    for (int i = 0; i < nombre_champs; i++) {
        fprintf(fp, "%s", bdd.table.champs[i]);
        if (i < nombre_champs - 1) {
            fprintf(fp, ",");
        }
    }
    fprintf(fp, "\n");
    fclose(fp);
}

void inserer_donnees(const char *nom_table, const char *fichier, char *enregistrement[], int nombre_champs) {
    FILE *fp = fopen(fichier, "a");
    if (fp == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    for (int i = 0; i < nombre_champs; i++) {
        fprintf(fp, "%s", enregistrement[i]);
        if (i < nombre_champs - 1) {
            fprintf(fp, ",");
        }
    }
    fprintf(fp, "\n");
    fclose(fp);
}

void charger_ligne_donnees_en_memoire(const char *nom_table, const char *fichier, int ligne) {
    FILE *fp = fopen(fichier, "r");
    if (fp == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    char buffer[256];
    int current_line = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {
        if (current_line == ligne) {
            printf("Données de la ligne %d : %s", ligne, buffer);
            break;
        }
        current_line++;
    }

    fclose(fp);
}
