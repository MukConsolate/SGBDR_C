#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void creer_bdd(char *nom_bdd) {
    int create = mkdir(nom_bdd);

    if(create == 0) {
        printf("Base de donnees creee : %s\n", nom_bdd);
    } else {
        printf("Erreur de creation de cette base de donnees : %s\n", nom_bdd);
    }
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

void creer_chemin(char *chemin, char *folder, char *file){
    strcat(chemin, folder);
    strcat(chemin, "\\");
    strcat(chemin, file);
}

void inserer_donnees(char *nom_bdd, char *nom_table, char *donnees[], int nb_donnees) {
    FILE *fichier_table = NULL;
    char chemin[255];
    strcpy(chemin, "");
    creer_chemin(chemin, nom_bdd, nom_table);
    fichier_table = fopen(chemin, "a");

    if(fichier_table == NULL) {
        printf("Erreur d'ouverture de la table : %s\n", nom_table);
        return;
    }

    printf("Insertion de donnees : ");
    for (int i = 0; i < nb_donnees; i++)
    {
        fprintf(fichier_table, "%s;", donnees[i]);
        printf("%s;", donnees[i]);
    }
    printf("\n");
    fprintf(fichier_table, "\n");

    fclose(fichier_table);
}

//, char *donnees[]

void charger_ligne_donnees_en_memoire(char *nom_bdd, char *nom_table, int num_ligne) {
    FILE *fichier_table = NULL;
    char chemin[255];
    strcpy(chemin, "");
    creer_chemin(chemin, nom_bdd, nom_table);
    fichier_table = fopen(chemin, "r");

    if(fichier_table == NULL) {
        printf("Erreur de chargement des donnees dans la memoire");
        return;
    }

    int n_ligne = 0;
    int compteur_champs = 0;
    int flag_debut = 0;
    char caractere;

    char champs[20][255];

    while((caractere = fgetc(fichier_table)) != EOF) {
        if(caractere == '\n') {
            n_ligne++;
            continue;
        }

        if(n_ligne + 1 == num_ligne) {
            flag_debut = 1;
        }

        if(n_ligne == num_ligne) {
            break;
        }

        if(caractere == ';' && flag_debut) {
            compteur_champs++;
            continue;
        }

        if(flag_debut){
            char str_data[2];
            str_data[0] = caractere;
            str_data[1] = '\0';
            strcat(champs[compteur_champs], str_data);
        }
    }

    printf("Voici les champs : \n");
    for (int i = 0; i < compteur_champs; i++)
    {
        printf("\n* %s\n", champs[i]);
    }

    printf("\nNB Champs : %d\n", compteur_champs);

    fclose(fichier_table);
}

void afficher_donnees(char *donnees) {

}

int main()
{
    char *champs[] = {"champ1", "champ2", "champ3"};
    char *enregistrement_1[] = {"oeufs", "2500FC", "Chez Muk"};
    char *enregistrement_2[] = {"eau", "55000FC", "Chez Kafat"};
    char *enregistrement_3[] = {"Jus", "75000FC", "Chez Clau"};
    creer_bdd("muk");
    creer_table("clau", "resto.txt", champs, 3);
    inserer_donnees("clau", "resto.txt", enregistrement_1, 3);
    inserer_donnees("clau", "resto.txt", enregistrement_2, 3);
    inserer_donnees("clau", "resto.txt", enregistrement_3, 3);

    charger_ligne_donnees_en_memoire("clau", "resto.txt", 2);

    return 0;
}
