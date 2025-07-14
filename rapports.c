#include <stdio.h>
#include <time.h>
#include <string.h>
#include "rapports.h"
#include "produits.h"
#include "ventes.h"

void genererRapportJournalier() {
    //  Generer date AAAAMMDD
    char date[9]; // "AAAAMMDD\0"
    time_t now = time(NULL);
    struct tm today = *localtime(&now);
    sprintf(date, "%04d%02d%02d", today.tm_year + 1900, today.tm_mon + 1, today.tm_mday);

    //  Creer le nom du fichier rapport
    char nomFichier[30];
    sprintf(nomFichier, "ETAT_%s.txt", date);

    FILE *fRapport = fopen(nomFichier, "w");
    if (fRapport == NULL) {
        printf("[ERREUR] Impossible de creer le fichier %s\n", nomFichier);
        return;
    }

    //  entete
    fprintf(fRapport, "Rapport du : %04d-%02d-%02d\n", today.tm_year + 1900, today.tm_mon + 1, today.tm_mday);

    // Recuperer ventes du jour
    float totalVentes = 0;
    int nbMedicament = 0;
    int ventesOK = calculerVentesDuJour(date, &totalVentes, &nbMedicament);

    if (ventesOK) {
        fprintf(fRapport, "Total des ventes : %.2f XOF\n", totalVentes);
        fprintf(fRapport, "Nombre total de medicaments vendus : %d\n\n", nbMedicament);

        // Verifier les stocks critiques
        Produit produits[100];
        int nbProduits;
        if (chargerProduits(produits, &nbProduits)) {
            fprintf(fRapport, "stock critique :\n");
            verifierStocksCritiques(produits, nbProduits, fRapport);
        } else {
            fprintf(fRapport, "[INFO] Impossible de charger les produits pour les alertes de stock.\n");
        }
    } else {
        fprintf(fRapport, "[ERREUR] Aucun fichier de vente trouver pour aujourdhui.\n");
    }

    fclose(fRapport);
    printf("Rapport journalier generer : %s\n", nomFichier);
}

