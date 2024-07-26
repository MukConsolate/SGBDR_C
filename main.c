#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "champ.h"
#include "enregistrement.h"

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

int main()
{
    char **nom_bdd;
    int ligne;
    char table;
    char commande[255];
    char *champs[] = {"champ1", "champ2", "champ3"};
    char *enregistrement_1[] = {"oeufs", "2500FC", "Chez Muk"};
    while (1) {
        printf("Entrez une commande en respectant la casse et le terme: \n1.creer_bdd \n2.creer_table \n3.inserer_donnees \n4.selectionner_donnees \n5.afficher_donnees \n6.supprimer_donnees \n7.mettre_a_jour_donnees \n8.trier_donnees \n9.quitter \n Votre commande:");
        fgets(commande, sizeof(commande), stdin);

        // Supprimer le retour à la ligne de la commande
        commande[strcspn(commande, "\n")] = '\0';

        if (strcmp(commande, "creer_bdd") == 0) {
            printf("Saississez le nom de votre base de données: ");
            scanf("%s",&nom_bdd);
            creer_bdd(&nom_bdd);
        } else if (strcmp(commande, "creer_table") == 0) {
            printf("Saisissez la base de données dans laquelle vous voulez ajouter une table: ");
            scanf("%c", &nom_bdd);
            printf ("Saisissez le nom de la table que vous voulez creer dans la base de données %c :",nom_bdd);
            scanf("%c", &table);
            creer_table(nom_bdd, "resto.txt", champs, 3);

        } else if (strcmp(commande, "inserer_donnees") == 0) {

            inserer_donnees("clau", "resto.txt", enregistrement_1, 3);

        } else if (strcmp(commande, "selectionner_donnees") == 0){
            //la selection en utilisant des conditions simples supérieure, inférieure, et egalité

        }else if (strcmp(commande, "charger_ligne_donnees_en_memoire") == 0) {

            charger_ligne_donnees_en_memoire("clau", "resto.txt", 2);

        } else if (strcmp(commande, "afficher_donnees") == 0) {
            //affichage de menière formater
        }else if (strcmp(commande, "supprimer_donnees") == 0){
            printf("Entrez le numéro de la ligne que vous souhaitez supprimer:");
            scanf("%d",&ligne);
        }else if (strcmp(commande, "mettre_a_jour_donnees") == 0){

        }else if (strcmp(commande, "trier_donnees") == 0){

        }else if (strcmp(commande, "quitter") == 0) {
            printf("Programme terminé.\n");
            break;
        }else {
            printf("Commande non reconnue. Veuillez réessayer.\n");
        }
    }

    return 0;
}
