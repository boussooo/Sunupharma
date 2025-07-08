#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ventes.h"

// Generation du numero de vente
void genererNumeroVente(char* numero) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(numero, 15, "%Y%m%d%H%M%S", tm_info); // Format AAAAMMDDHHmmSS
}

// Creation de la facture dans le dossier /BILLS/
void creerFacture(ProduitVendu ventes[], int taille, float total, char* login) {
    char numero[15];
    genererNumeroVente(numero);

    char nomFichier[100];
    sprintf(nomFichier, "BILLS/RECU_%s_001_%s.txt", numero, login);

    FILE* f = fopen(nomFichier, "w");
    if (!f) {
        printf("[ERREUR] Impossible de creer la facture.\n");
        return;
    }

    fprintf(f, "========================================\n");
    fprintf(f, "              FACTURE DE VENTE          \n");
    fprintf(f, "========================================\n");
    fprintf(f, "Numero de vente : %s\nPharmacien : %s\n\n", numero, login);
    fprintf(f, "Code\tNom\tQte\tPU\tTotal\n");

    for (int i = 0; i < taille; i++) {
        fprintf(f, "%s\t%s\t%d\t%.2f\t%.2f\n",
                ventes[i].code, ventes[i].nom, ventes[i].quantiteVendue,
                ventes[i].prixUnitaire, ventes[i].prixTotal);
    }

    fprintf(f, "\nTOTAL A PAYER : %.2f XOF\n", total);
    fclose(f);
    printf("[OK] Facture enregistree : %s\n", nomFichier);
}

// Mise a jour automatique du stock
void mettreAJourStock(ProduitVendu ventes[], int nb) {
    Produit p;
    FILE* f = fopen("PRODUCTS.dat", "rb+");
    if (!f) {
        printf("[ERREUR] Fichier PRODUCTS.dat introuvable.\n");
        return;
    }

    while (fread(&p, sizeof(Produit), 1, f)) {
        for (int i = 0; i < nb; i++) {
            if (strcmp(p.code, ventes[i].code) == 0) {
                p.quantite -= ventes[i].quantiteVendue;
                fseek(f, -sizeof(Produit), SEEK_CUR);
                fwrite(&p, sizeof(Produit), 1, f);
                break;
            }
        }
    }

    fclose(f);
}

// Fonction principale pour realiser une vente
void effectuerVente(char* login) {
    ProduitVendu ventes[50];
    int nb = 0;
    float total = 0.0;
    char code[6];
    int quantite;

    while (1) {
        printf("\nEntrer le code du medicament (ou 0 pour terminer) : ");
        scanf("%s", code);
        if (strcmp(code, "0") == 0) break;

        printf("Quantite souhaitee : ");
        scanf("%d", &quantite);

        FILE* f = fopen("PRODUCTS.dat", "rb");
        Produit p;
        int trouve = 0;

        if (!f) {
            printf("[ERREUR] Impossible de lire le stock.\n");
            return;
        }

        while (fread(&p, sizeof(Produit), 1, f)) {
            if (strcmp(p.code, code) == 0) {
                trouve = 1;

                if (p.quantite >= quantite) {
                    strcpy(ventes[nb].code, p.code);
                    strcpy(ventes[nb].nom, p.nom);
                    ventes[nb].quantiteVendue = quantite;
                    ventes[nb].prixUnitaire = p.prix;
                    ventes[nb].prixTotal = quantite * p.prix;
                    total += ventes[nb].prixTotal;
                    nb++;
                    printf("[OK] Medicament ajoute a la commande.\n");
                } else {
                    printf("[ERREUR] Stock insuffisant pour %s.\n", p.nom);
                }
                break;
            }
        }

        fclose(f);
        if (!trouve) {
            printf("[ERREUR] Medicament introuvable.\n");
        }
    }

    if (nb > 0) {
        creerFacture(ventes, nb, total, login);
        mettreAJourStock(ventes, nb);
        printf("\n[OK] Vente effectuee. Total : %.2f XOF\n", total);
    } else {
        printf("[INFO] Aucune vente enregistree.\n");
    }
}

