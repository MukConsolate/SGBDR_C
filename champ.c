#include "champ.h"

void creer_bdd(char *nom_bdd) {
    int create = mkdir(nom_bdd);

    if(create == 0) {
        printf("Base de donnees creee : %s\n", nom_bdd);
    } else {
        printf("Erreur de creation de cette base de donnees : %s\n", nom_bdd);
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
void creer_chemin(char *chemin, char *folder, char *file){
    strcat(chemin, folder);
    strcat(chemin, "\\");
    strcat(chemin, file);
}

void creer_table(char *nom_bdd, char *nom_table, char *champs[], int nb_champs) {
    FILE *fichier_table = NULL;
    char chemin[255];
    creer_chemin(chemin, nom_bdd, nom_table);
    fichier_table = fopen(chemin, "w");

    if(fichier_table == NULL) {
        printf("Erreur de creation de la table : %s\n", nom_table);
    }

    for (int i = 0; i < nb_champs; i++)
    {
        fprintf(fichier_table, "%s;", champs[i]);
    }
    fprintf(fichier_table, "\n");

    fclose(fichier_table);
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
void selectionner_donnees(char *nom_bdd, char *nom_table) {
    FILE *fichier_table = NULL;
    char chemin[255];
    strcpy(chemin, "");
    creer_chemin(chemin, nom_bdd, nom_table);
    fichier_table = fopen(chemin, "r");

    if(fichier_table == NULL) {
        printf("Erreur d'ouverture de la table : %s\n", nom_table);
        return;
    }

    char ligne[255];
    while(fgets(ligne, sizeof(ligne), fichier_table) != NULL) {
        printf("%s", ligne);
    }

    fclose(fichier_table);
}
void afficher_donnees(char *donnees) {

}

void mettre_a_jour_donnees(char *nom_bdd, char *nom_table, int num_ligne, char *nouvelles_donnees[]) {
    FILE *fichier_table = NULL;
    char chemin[255];
    strcpy(chemin, "");
    creer_chemin(chemin, nom_bdd, nom_table);
    
    // Ouvrir le fichier en mode lecture
    fichier_table = fopen(chemin, "r");
    if (fichier_table == NULL) {
        printf("Erreur d'ouverture de la table : %s\\n", nom_table);
        return;
    }

    // Lire toutes les lignes dans un tableau
    char lignes[100][255]; // Il y a au maximum 100 lignes
    int compteur_lignes = 0;

    while (fgets(lignes[compteur_lignes], sizeof(lignes[compteur_lignes]), fichier_table) != NULL) {
        compteur_lignes++;
    }
    fclose(fichier_table);

    if (num_ligne >= 0 && num_ligne < compteur_lignes) {
        snprintf(lignes[num_ligne], sizeof(lignes[num_ligne]), "%s\\n", nouvelles_donnees[0]); // Met à jour avec la première nouvelle donnée
        // Vous pouvez ajouter plus de logique ici si vous avez plusieurs champs

        // Réécrire le fichier avec les données mises à jour
        fichier_table = fopen(chemin, "w");
        for (int i = 0; i < compteur_lignes; i++) {
            fprintf(fichier_table, "%s", lignes[i]);
        }
        fclose(fichier_table);
    } else {
        printf("Numéro de ligne invalide.\\n");
    }
}

void supprimer_donnees(char *nom_bdd, char *nom_table, int num_ligne) {
    FILE *fichier_table = NULL;
    char chemin[255];
    strcpy(chemin, "");
    creer_chemin(chemin, nom_bdd, nom_table);
    
    // Ouvrir le fichier en mode lecture
    fichier_table = fopen(chemin, "r");
    if (fichier_table == NULL) {
        printf("Erreur d'ouverture de la table : %s\\n", nom_table);
        return;
    }

    // Lire toutes les lignes dans un tableau
    char lignes[100][255]; // Il y a au maximum 100 lignes
    int compteur_lignes = 0;

    while (fgets(lignes[compteur_lignes], sizeof(lignes[compteur_lignes]), fichier_table) != NULL) {
        compteur_lignes++;
    }
    fclose(fichier_table);

    // Supprimer la ligne spécifiée
    if (num_ligne >= 0 && num_ligne < compteur_lignes) {
        for (int i = num_ligne; i < compteur_lignes - 1; i++) {
            strcpy(lignes[i], lignes[i + 1]);
        }
        compteur_lignes--; // Réduire le nombre de lignes

        // Réécrire le fichier sans la ligne supprimée
        fichier_table = fopen(chemin, "w");
        for (int i = 0; i < compteur_lignes; i++) {
            fprintf(fichier_table, "%s", lignes[i]);
        }
        fclose(fichier_table);
    } else {
        printf("Numéro de ligne invalide.\\n");
    }
}
void trier_donnees(char *nom_bdd, char *nom_table) {
    FILE *fichier_table = NULL;
    char chemin[255];
    strcpy(chemin, "");
    creer_chemin(chemin, nom_bdd, nom_table);
    
    // Ouvrir le fichier en mode lecture
    fichier_table = fopen(chemin, "r");
    if (fichier_table == NULL) {
        printf("Erreur d'ouverture de la table : %s\\n", nom_table);
        return;
    }

    // Lire toutes les lignes dans un tableau
    char lignes[100][255]; // Supposons qu'il y a au maximum 100 lignes
    int compteur_lignes = 0;

    while (fgets(lignes[compteur_lignes], sizeof(lignes[compteur_lignes]), fichier_table) != NULL) {
        compteur_lignes++;
    }
    fclose(fichier_table);

    // tri à bulles par odre alphabétique
    for (int i = 0; i < compteur_lignes - 1; i++) {
        for (int j = 0; j < compteur_lignes - i - 1; j++) {
            if (strcmp(lignes[j], lignes[j + 1]) > 0) {
                char temp[255];
                strcpy(temp, lignes[j]);
                strcpy(lignes[j], lignes[j + 1]);
                strcpy(lignes[j + 1], temp);
            }
        }
    }

    // Réécrire le fichier avec les données triées
    fichier_table = fopen(chemin, "w");
    for (int i = 0; i < compteur_lignes; i++) {
        fprintf(fichier_table, "%s", lignes[i]);
    }
    fclose(fichier_table);
}