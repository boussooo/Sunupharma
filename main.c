#include "header.h"
#include <stdio.h>

int main() {
    int choix;
    do {
        printf("1. Ajouter un produit\n");
        printf("2. Modifier un produit\n");
        printf("3. Supprimer un produit\n");
        printf("4. Afficher les produits\n");
        printf("0. Quitter\n");
        printf("Choisissez une option: ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                ajouterProduit();
                break;
            case 2:
                modifierProduit();
                break;
            case 3:
                supprimerProduit();
                break;
            case 4:
                afficherProduits();
                break;
            case 0:
                printf("Au revoir!\n");
                break;
            default:
                printf("Option invalide, veuillez réessayer.\n");
        }
    } while (choix != 0);

    return 0;
}
