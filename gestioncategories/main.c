#include <stdio.h>
#include "categorie.h"

int main() {
    int choix;
    do {
        printf("Menu Categories :\n");
        printf("1. Ajouter\n");
        printf("2. Afficher\n");
        printf("3. Rechercher\n");
        printf("4. Supprimer\n");
        printf("0. Quitter\n");
        printf("Choix : ");
        scanf("%d%*c", &choix);

        switch (choix) {
            case 1: ajouterCategorie(); break;
            case 2: afficherCategories(); break;
            case 3: rechercherCategorie(); break;
            case 4: supprimerCategorie(); break;
            case 0: printf("Fermeture.\n"); break;
            default: printf("Choix invalide.\n");
        }

    } while (choix != 0);

    return 0;

}
