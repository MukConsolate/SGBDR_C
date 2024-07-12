#include <stdio.h>

int main() {
    char choix;
    do {
        printf("\nMenu :\n Veillez respecter la casse des commandes");
        printf("1.Tapez la commande *inserer_donnee* pour Insérer des données\n");
        printf("2.Tapez la commande *selectionner_donnee* pour Selectionner des données\n");
        printf("3.Tapez la commande *afficher_donnee* pour Afficher des données\n");
        printf("4.Tapez la commande *mis_a_jour_donnee* pour Mettre à jour des données\n");
        printf("5.Tapez la commande *supprimer_donnee* pour Supprimer des données\n");

        printf("Tapez la commande : ");
        scanf("%c", choix);

        switch (choix) {
            case 1:
                insererDonnee();
                break;
            case 2:
                selectionnerDonnee();
                break;
            case 3:
                afficherDonnee();
                break;
            case 4:
                supprimerDonnee();
                break;
            case 5:
                misajourDonnee();
                break;
            default:
                printf("Option invalide. Réessayez.\n");
        }
    } while (choix != 5);

    return 0;
}