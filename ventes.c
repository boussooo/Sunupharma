#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ventes.h"

// Génération du numéro de vente : AAAAMMDDHHmmSS
void genererNumeroVente(char* numero) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(numero, 15, "%Y%m%d%H%M%S", tm_info);
}

// Affichage stylisé pour la console
void afficherMessage(const char* message, const char* couleur) {
    printf("%s%s\033[0m\n", couleur, message);
}

// Création de la facture
void creerFacture(ProduitVendu ventes[], int taille, float total, char* login) {
    char numero[15];
    system("mkdir BILLS");
    genererNumeroVente(numero);

    char nomFichier[100];
    sprintf(nomFichier, "BILLS/RECU_%s_001_%s.txt", numero, login);

    FILE* f = fopen(nomFichier, "w");
    if (!f) {
        afficherMessage("[ERREUR] Impossible de créer la facture.", "\033[0;31m");
        return;
    }

    fprintf(f, "========================================\n");
    fprintf(f, "              FACTURE DE VENTE          \n");
    fprintf(f, "========================================\n");
    fprintf(f, "Numéro de vente : %s\nPharmacien : %s\n\n", numero, login);
    fprintf(f, "%-6s %-25s %-5s %-10s %-10s\n", "Code", "Produit", "Qté", "PU (XOF)", "Total");
    fprintf(f, "---------------------------------------------------------------\n");
    for (int i = 0; i < taille; i++) {
        fprintf(f, "%-6s %-25s %-5d %-10.2f %-10.2f\n",
        ventes[i].code, ventes[i].nom,
        ventes[i].quantiteVendue, ventes[i].prixUnitaire, ventes[i].prixTotal);

    }

    fprintf(f, "\nTOTAL À PAYER : %.2f XOF\n", total);
       fprintf(f, "\n----------------------------------------\n");
fprintf(f, "  Merci pour votre confiance chez Sunupharma 💊\n");
fprintf(f, "  À bientôt pour vos prochains besoins de santé !\n");
fprintf(f, "----------------------------------------\n");
    fclose(f);

    char msg[150];
    sprintf(msg, "[✓] Facture enregistrée : %s", nomFichier);
    afficherMessage(msg, "\033[0;32m");
}

// Mise à jour automatique du stock
void mettreAJourStock(ProduitVendu ventes[], int nb) {
    FILE* f = fopen("PRODUCTS.dat", "rb+");
    if (!f) {
        afficherMessage("[ERREUR] Fichier PRODUCTS.dat introuvable.", "\033[0;31m");
        return;
    }

    Produit p;
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

// Fonction principale de vente
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
        if (!f) {
            afficherMessage("[ERREUR] Impossible de lire le stock.", "\033[0;31m");
            return;
        }

        Produit p;
        int trouve = 0;
        while (fread(&p, sizeof(Produit), 1, f)) {
            if (strcmp(p.code, code) == 0) {
                trouve = 1;
                time_t maintenant = time(NULL);
                if (difftime(p.datePeremption, maintenant) <= 0) {
                    char msg[80];
                    sprintf(msg, "[ERREUR] Le médicament %s est périmé.", p.nom);
                    afficherMessage(msg, "\033[0;31m");
                    break;
                }

                if (p.quantite >= quantite) {
                    strcpy(ventes[nb].code, p.code);
                    strcpy(ventes[nb].nom, p.nom);
                    ventes[nb].quantiteVendue = quantite;
                    ventes[nb].prixUnitaire = p.prix;
                    ventes[nb].prixTotal = quantite * p.prix;
                    total += ventes[nb].prixTotal;
                    nb++;
                    afficherMessage("[✓] Medicament ajoute a la commande.", "\033[0;32m");
                } else {
                    char msg[80];
                    sprintf(msg, "[ERREUR] Stock insuffisant pour %s.", p.nom);
                    afficherMessage(msg, "\033[0;31m");
                }

                break;
            }
        }

        fclose(f);
        if (!trouve) {
            afficherMessage("[ERREUR] Medicament introuvable.", "\033[0;31m");
        }
    }

    if (nb > 0) {
        char confirm;
        printf("Confirmer la vente ? (O/N) : ");
        scanf(" %c", &confirm);
        if (confirm == 'O' || confirm == 'o') {
            creerFacture(ventes, nb, total, login);
            mettreAJourStock(ventes, nb);
            printf("\n\033[0;32m[✓] Vente effectuee. Total : %.2f XOF\033[0m\n", total);
        } else {
            afficherMessage("[INFO] Vente annulee par l'utilisateur.", "\033[0;33m");
        }
    } else {
        afficherMessage("[INFO] Aucune vente enregistree.", "\033[0;33m");
    }
}
