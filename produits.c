#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "produits.h"

#define FICHIER_PRODUITS "PRODUCTS.dat"

Produit produits[MAX_PRODUCTS];
int nb_produits = 0;

// Affichage stylisé
void afficherSucces(const char* msg) {
    printf("\033[32m%s\033[0m\n", msg);
}

void afficherErreur(const char* msg) {
    printf("\033[31m%s\033[0m\n", msg);
}

// Validation du code produit
int estCodeValide(const char* code) {
    if (strlen(code) != 5) return 0;
    for (int i = 0; i < 5; i++) {
        if (!isalnum(code[i])) return 0;
    }
    return 1;
}

// Validation de la date
int estDateValide(const char* date) {
    int y, m, d;
    if (sscanf(date, "%4d-%2d-%2d", &y, &m, &d) != 3) return 0;
    if (y < 1900 || m < 1 || m > 12 || d < 1 || d > 31) return 0;
    return 1;
}

// Vérifier unicité du code
int verifierCodeUnique(const char* code) {
    Produit p;
    FILE* f = fopen(FICHIER_PRODUITS, "rb");
    if (!f) return 1;
    while (fread(&p, sizeof(Produit), 1, f)) {
        if (strcmp(p.code, code) == 0) {
            fclose(f);
            return 0;
        }
    }
    fclose(f);
    return 1;
}
void menuGestionProduits() {
    int choix;
    do {
        printf("\033[1;36m\n=== MENU GESTION DES PRODUITS ===\033[0m\n");
        printf("1. Ajouter un produit\n");
        printf("2. Modifier un produit\n");
        printf("3. Supprimer un produit\n");
        printf("4. Afficher tous les produits\n");
        printf("0. Retour au menu precedent\n");
        printf("Votre choix : ");
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
                printf("Retour...\n");
                break;
            default:
                afficherErreur("[ERREUR] Choix invalide !");
        }
    } while (choix != 0);
}

// Ajouter un produit
void ajouterProduit() {
    Produit p;
    printf("\n=== AJOUT PRODUIT ===\n");

    do {
        printf("Code (5 caracteres alphanumeriques) : ");
        scanf("%s", p.code);
        if (!estCodeValide(p.code)) {
            afficherErreur("Code invalide !");
        } else if (!verifierCodeUnique(p.code)) {
            afficherErreur("Code deja utilise !");
        } else break;
    } while (1);

    printf("Designation : ");
    scanf(" %[^\n]", p.designation);
    printf("Prix : ");
    scanf("%f", &p.prix);
    printf("Quantite : ");
    scanf("%d", &p.quantite);
    printf("Categorie : ");
    scanf(" %[^\n]", p.categorie);

    do {
        printf("Date de peremption (YYYY-MM-DD) : ");
        scanf("%s", p.date_peremption);
        if (!estDateValide(p.date_peremption)) {
            afficherErreur("Date invalide !");
        } else break;
    } while (1);

    FILE* f = fopen(FICHIER_PRODUITS, "ab");
    if (f) {
        fwrite(&p, sizeof(Produit), 1, f);
        fclose(f);
        afficherSucces("[✓] Produit ajoute avec succes.");
    } else {
        afficherErreur("[ERREUR] Impossible d'ecrire dans le fichier.");
    }
}

// Afficher tous les produits
void afficherProduits() {
    FILE* f = fopen(FICHIER_PRODUITS, "rb");
    if (!f) {
        afficherErreur("[ERREUR] Impossible d'ouvrir PRODUCTS.dat");
        return;
    }

    Produit p;
    int cpt = 0;

    printf("\n=== LISTE DES PRODUITS ===\n");
    printf("%-6s | %-20s | %-8s | %-8s | %-15s | %-12s\n",
           "Code", "Designation", "Prix", "Stock", "Categorie", "Peremption");
    printf("--------------------------------------------------------------------------\n");

    while (fread(&p, sizeof(Produit), 1, f)) {
        printf("%-6s | %-20s | %-8.2f | %-8d | %-15s | %-12s\n",
               p.code, p.designation, p.prix, p.quantite, p.categorie, p.date_peremption);
        cpt++;
    }

    if (cpt == 0) {
        printf("Aucun produit trouve.\n");
    }

    fclose(f);
}

// Modifier un produit
void modifierProduit() {
    char code[CODE_LENGTH];
    printf("\n=== MODIFIER PRODUIT ===\n");
    printf("Code du produit a modifier : ");
    scanf("%s", code);

    FILE* f = fopen(FICHIER_PRODUITS, "r+b");
    if (!f) {
        afficherErreur("[ERREUR] Impossible d'ouvrir PRODUCTS.dat");
        return;
    }

    Produit p;
    int trouve = 0;

    while (fread(&p, sizeof(Produit), 1, f)) {
        if (strcmp(p.code, code) == 0) {
            trouve = 1;
            printf("Designation (%s) : ", p.designation);
            scanf(" %[^\n]", p.designation);
            printf("Prix (%.2f) : ", p.prix);
            scanf("%f", &p.prix);
            printf("Quantite (%d) : ", p.quantite);
            scanf("%d", &p.quantite);
            printf("Categorie (%s) : ", p.categorie);
            scanf(" %[^\n]", p.categorie);

            do {
                printf("Date de peremption (%s) : ", p.date_peremption);
                scanf("%s", p.date_peremption);
                if (!estDateValide(p.date_peremption)) {
                    afficherErreur("Date invalide !");
                } else break;
            } while (1);

            fseek(f, -sizeof(Produit), SEEK_CUR);
            fwrite(&p, sizeof(Produit), 1, f);
            afficherSucces("[✓] Produit modifie avec succes.");
            break;
        }
    }

    fclose(f);
    if (!trouve) {
        afficherErreur("[ERREUR] Produit non trouve.");
    }
}

// Supprimer un produit
void supprimerProduit() {
    char code[CODE_LENGTH];
    printf("\n=== SUPPRIMER PRODUIT ===\n");
    printf("Code du produit a supprimer : ");
    scanf("%s", code);

    FILE* f = fopen(FICHIER_PRODUITS, "rb");
    if (!f) {
        afficherErreur("[ERREUR] Impossible d'ouvrir PRODUCTS.dat");
        return;
    }

    Produit temp[MAX_PRODUCTS];
    int count = 0, trouve = 0;
    Produit p;

    while (fread(&p, sizeof(Produit), 1, f)) {
        if (strcmp(p.code, code) != 0) {
            temp[count++] = p;
        } else {
            trouve = 1;
        }
    }
    fclose(f);

    if (!trouve) {
        afficherErreur("[ERREUR] Produit non trouve.");
        return;
    }

    f = fopen(FICHIER_PRODUITS, "wb");
    if (!f) {
        afficherErreur("[ERREUR] Impossible d'ecrire dans PRODUCTS.dat");
        return;
    }

    fwrite(temp, sizeof(Produit), count, f);
    fclose(f);
    afficherSucces("[✓] Produit supprime avec succes.");
}
