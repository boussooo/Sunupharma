
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "categories.h"

#define FICHIER_CATEGORIE "CATEGORIES.dat"

void menuGestionCategories(){
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
}

void ajouterCategorie() {
    FILE *f = fopen(FICHIER_CATEGORIE, "ab+");
    if (!f) {
        printf("Erreur d'ouverture du fichier.\n");
        return;
    }

    Categorie nouvelle, temp;
    int dernierId = 0;

    while (fread(&temp, sizeof(Categorie), 1, f)) {
        dernierId = temp.id;
    }
    nouvelle.id = dernierId + 1;

    printf("Entrez le libelle : ");
    fgets(nouvelle.libelle, sizeof(nouvelle.libelle), stdin);
    nouvelle.libelle[strcspn(nouvelle.libelle, "\n")] = '\0';

    fwrite(&nouvelle, sizeof(Categorie), 1, f);
    fclose(f);
    printf("Categorie ajoutee.\n");
}

void afficherCategories() {
    FILE *f = fopen(FICHIER_CATEGORIE, "rb");
    if (!f) {
        printf("Fichier introuvable.\n");
        return;
    }

    Categorie cat;
    printf("Liste des categories :\n");

    while (fread(&cat, sizeof(Categorie), 1, f)) {
        printf("ID : %d, Libelle : %s\n", cat.id, cat.libelle);
    }

    fclose(f);
}

void rechercherCategorie() {
    int idRecherche;
    printf("Entrez l'ID : ");
    scanf("%d%*c", &idRecherche);

    FILE *f = fopen(FICHIER_CATEGORIE, "rb");
    if (!f) {
        printf("Erreur d'acces au fichier.\n");
        return;
    }

    Categorie cat;
    int trouve = 0;
    while (fread(&cat, sizeof(Categorie), 1, f)) {
        if (cat.id == idRecherche) {
            printf("Categorie trouvee : %s\n", cat.libelle);
            trouve = 1;
            break;
        }
    }
    if (!trouve) {
        printf("ID non trouve.\n");
    }

    fclose(f);
}

void supprimerCategorie() {
    int idSupprimer;
    printf("Entrez l'ID � supprimer : ");
    scanf("%d%*c", &idSupprimer);

    FILE *f = fopen(FICHIER_CATEGORIE, "rb");
    FILE *temp = fopen("TEMP.dat", "wb");
    if (!f || !temp) {
        printf("Erreur fichier.\n");
        return;
    }

    Categorie cat;
    int trouve = 0;
    while (fread(&cat, sizeof(Categorie), 1, f)) {
        if (cat.id != idSupprimer) {
            fwrite(&cat, sizeof(Categorie), 1, temp);
        } else {
            trouve = 1;
        }
    }

    fclose(f);
    fclose(temp);

    remove(FICHIER_CATEGORIE);
    rename("TEMP.dat", FICHIER_CATEGORIE);

    if (trouve) {
        printf("Categorie supprimee.\n");
    } else {
        printf("ID non trouve.\n");
    }
    void exporterCategories() {
    FILE *fb = fopen(FICHIER_CATEGORIE, "rb");
    FILE *ft = fopen("categories_export.txt", "w");

    if (!fb || !ft) {
        printf("Erreur d'acc�s aux fichiers.\n");
        return;
    }

    Categorie cat;
    while (fread(&cat, sizeof(Categorie), 1, fb)) {
        fprintf(ft, "ID : %d | Libell� : %s\n", cat.id, cat.libelle);
    }

    fclose(fb);
    fclose(ft);
    printf("Exportation termin�e vers 'categories_export.txt'.\n");
}

}
