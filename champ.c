#include "champ.h"


void creer_bdd(const char *nom) {
    // Crée un fichier pour la base de données
    FILE *fichier = fopen(nom, "w");
    if (fichier) {
        fclose(fichier);
        printf("Base de données '%s' créée.\n", nom);
    } else {
        perror("Erreur lors de la création de la base de données");
    }
}

void creer_table(const char *nom_table, const char *fichier, char **champs, int nombre_champs) {
    // Crée une table dans le fichier spécifié
    FILE *f = fopen(fichier, "a");
    if (f) {
        fprintf(f, "%s\n", nom_table); // Écrire le nom de la table
        for (int i = 0; i < nombre_champs; i++) {
            fprintf(f, "%s;", champs[i]); // Écrire les champs séparés par des points-virgules
        }
        fprintf(f, "\n"); // Nouvelle ligne après les champs
        fclose(f);
        printf("Table '%s' créée avec succès.\n", nom_table);
    } else {
        perror("Erreur lors de la création de la table");
    }
}

void inserer_donnees(const char *nom_table, const char *fichier, char **donnees, int nombre_donnees) {
    // Insère des données dans la table spécifiée
    FILE *f = fopen(fichier, "a");
    if (f) {
        fprintf(f, "%s;", nom_table); // Écrire le nom de la table
        for (int i = 0; i < nombre_donnees; i++) {
            fprintf(f, "%s;", donnees[i]); // Écrire les données séparées par des points-virgules
        }
        fprintf(f, "\n"); // Nouvelle ligne après les données
        fclose(f);
        printf("Données insérées dans la table '%s'.\n", nom_table);
    } else {
        perror("Erreur lors de l'insertion des données");
    }
}

void charger_ligne_donnees_en_memoire(const char *nom_table, const char *fichier, int ligne) {
    // Charge une ligne de données en mémoire
    FILE *f = fopen(fichier, "r");
    if (f) {
        char buffer[1024];
        int current_line = 0;
        
        while (fgets(buffer, sizeof(buffer), f)) {
            if (current_line == ligne) {
                printf("Ligne %d: %s", ligne, buffer);
                break;
            }
            current_line++;
        }
        
        fclose(f);
    } else {
        perror("Erreur lors du chargement des données");
    }
}
