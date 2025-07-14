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
        printf("5. Rechercher un produit par code\n");
        printf("6. Mettre a jour le stock d 'un produit\n");
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
                case 5:
                rechercherProduitParCode();
                break;
            case 6:
                mettreAJourStock();
                break;
            case 0:
                 sauvegarderProduits();
                printf("Au revoir!\n");
                break;
            default:
                printf("Option invalide, veuillez reessayer.\n");
        }
    } while (choix != 0);

    return 0;
}
