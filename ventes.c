#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include "ventes.h"
#include "produits.h"
#include "utilisateurs.h"


Produit p;
void menuGestionVentes(){
 char login[6];

    printf("===== MODULE DE TEST : GESTION DES VENTES =====\n");
    printf("Entrez le login du pharmacien (5 lettres majuscules) : ");
    scanf("%s", login);

    effectuerVente(login);
}
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
fprintf(f, "  Merci pour votre confiance chez Sunupharma \n");
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

time_t convertirDate(const char* dateStr) {
    struct tm tm_date = {0};
    sscanf(dateStr, "%4d-%2d-%2d", &tm_date.tm_year, &tm_date.tm_mon, &tm_date.tm_mday);
    tm_date.tm_year -= 1900;  // Année depuis 1900
    tm_date.tm_mon -= 1;      // Mois de 0 à 11
    return mktime(&tm_date);
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
                time_t datePeremption = convertirDate(p.date_peremption);
                if (difftime(datePeremption, maintenant) <= 0) {
                    char msg[80];
                    sprintf(msg, "[ERREUR] Le médicament %s est périmé.", p.designation);
                    afficherMessage(msg, "\033[0;31m");
                    break;
                }

                if (p.quantite >= quantite) {
                    strcpy(ventes[nb].code, p.code);
                    strcpy(ventes[nb].nom, p.designation);
                    ventes[nb].quantiteVendue = quantite;
                    ventes[nb].prixUnitaire = p.prix;
                    ventes[nb].prixTotal = quantite * p.prix;
                    total += ventes[nb].prixTotal;
                    nb++;
                    afficherMessage("[✓] Medicament ajoute a la commande.", "\033[0;32m");
                } else {
                    char msg[80];
                    sprintf(msg, "[ERREUR] Stock insuffisant pour %s.", p.designation);
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
int calculerVentesDuJour(const char* date, float* totalVentes, int* nbMedicament) {
    char dossier[] = "BILLS";
    char prefix[20];
    sprintf(prefix, "RECU_%s", date);

    *totalVentes = 0;
    *nbMedicament = 0;

    // Ouvre le dossier et lit les fichiers
    DIR* dir = opendir(dossier);
    if (!dir) return 0;

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, prefix)) {
            char chemin[120];
            sprintf(chemin, "%s/%s", dossier, entry->d_name);

            FILE* f = fopen(chemin, "r");
            if (!f) continue;

            char ligne[200];
            while (fgets(ligne, sizeof(ligne), f)) {
                float prix;
                int qte;
                if (sscanf(ligne, "%*s %*s %d %*f %f", &qte, &prix) == 2) {
                    *totalVentes += prix;
                    *nbMedicament += qte;
                }
            }

            fclose(f);
        }
    }

    closedir(dir);
    return 1;
}
void verifierStocksCritiques(Produit produits[], int nb, FILE* fRapport) {
    for (int i = 0; i < nb; i++) {
        if (produits[i].quantite < 5) {
            fprintf(fRapport, "- %s (%s) : %d unités restantes\n",
                    produits[i].designation, produits[i].code, produits[i].quantite);
        }
    }
}
int chargerProduits(Produit produits[], int* nb) {
    FILE* f = fopen("PRODUCTS.dat", "rb");
    if (!f) return 0;

    *nb = 0;
    while (fread(&produits[*nb], sizeof(Produit), 1, f)) {
        (*nb)++;
    }

    fclose(f);
    return 1;
}
