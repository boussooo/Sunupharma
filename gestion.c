#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Produit produits[MAX_PRODUCTS];
int nb_produits = 0;

void ajouterProduit() {
    if (nb_produits >= MAX_PRODUCTS) {
        printf("Stock plein, impossible d'ajouter un produit.\n");
        return;
    }

    Produit p;
    printf("Entrez le code du produit (5 caracteres): ");
    scanf("%s", p.code);
    printf("Entrez la designation: ");
    scanf(" %[^\n]", p.designation);
    printf("Entrez le prix: ");
    scanf("%f", &p.prix);
    printf("Entrez la quantité en stock: ");
    scanf("%d", &p.quantite);
    printf("Entrez la categorie: ");
    scanf(" %[^\n]", p.categorie);
    printf("Entrez la date de peremption (YYYY-MM-DD): ");
    scanf("%s", p.date_peremption);

    produits[nb_produits++] = p;
    printf("Produit ajouté avec succès.\n");
}

void modifierProduit() {
    char code[CODE_LENGTH];
    printf("Entrez le code du produit à modifier: ");
    scanf("%s", code);

    for (int i = 0; i < nb_produits; i++) {
        if (strcmp(produits[i].code, code) == 0) {
            printf("Modifier la designation (actuel: %s): ", produits[i].designation);
            scanf(" %[^\n]", produits[i].designation);
            printf("Modifier le prix (actuel: %.2f): ", produits[i].prix);
            scanf("%f", &produits[i].prix);
            printf("Modifier la quantite (actuel: %d): ", produits[i].quantite);
            scanf("%d", &produits[i].quantite);
            printf("Modifier la categorie (actuel: %s): ", produits[i].categorie);
            scanf(" %[^\n]", produits[i].categorie);
            printf("Modifier la date de peremption (actuel: %s): ", produits[i].date_peremption);
            scanf("%s", produits[i].date_peremption);
            printf("Produit modifie avec succes.\n");
            return;
        }
    }
    printf("Produit non trouvé.\n");
}

void supprimerProduit() {
    char code[CODE_LENGTH];
    printf("Entrez le code du produit à supprimer: ");
    scanf("%s", code);

    for (int i = 0; i < nb_produits; i++) {
        if (strcmp(produits[i].code, code) == 0) {
            for (int j = i; j < nb_produits - 1; j++) {
                produits[j] = produits[j + 1];
            }
            nb_produits--;
            printf("Produit supprimé avec succès.\n");
            return;
        }
    }
    printf("Produit non trouvé.\n");
}

void afficherProduits() {
    printf("Liste des produits:\n");
    for (int i = 0; i < nb_produits; i++) {
        printf("Code: %s, Désignation: %s, Prix: %.2f, Quantité: %d, Catégorie: %s, Date de péremption: %s\n",
               produits[i].code, produits[i].designation, produits[i].prix,
               produits[i].quantite, produits[i].categorie, produits[i].date_peremption);
    }
}
