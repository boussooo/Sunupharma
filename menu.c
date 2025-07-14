#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "menu.h"
#include "produits.h"
#include "categories.h"
#include "utilisateurs.h"
#include "ventes.h"
#include "rapports.h"


// Effacer la console
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// === MENU ADMIN ===
void menuAdmin() {
    int choix;
    do {
        printf("\033[1;36m\n=== MENU ADMINISTRATEUR ===\n\033[0m");
        printf("1. Gestion des categories\n");
        printf("2. Gestion des produits\n");
        printf("3. Ajouter un utilisateur\n");
        printf("4. Afficher la liste les utilisateurs\n");
        printf("5. Bloquer / Debloquer un utilisateur\n");
        printf("0. Deconnexion\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch(choix) {
           case 1: clearScreen(); menuGestionCategories(); break;
            case 2: clearScreen(); menuGestionProduits(); break;
            case 3: clearScreen(); ajouterUtilisateur(); break;
            case 4: clearScreen(); afficherUtilisateurs(); break;
            case 5: clearScreen(); changerStatutUtilisateur(); break;
            case 0: clearScreen(); printf("Deconnexion...\n"); break;
            default: printf("Choix invalide !\n");
        }
    } while(choix != 0);
}

// === MENU PHARMACIEN ===
void menuPharmacien() {
    int choix;
    do {
        printf("\033[1;35m\n=== MENU PHARMACIEN ===\n\033[0m");
        printf("1. Vendre un produit \n");
        printf("2. Consulter le stock \n");
        printf("3. Imprimer rapport journalier\n");
        printf("0. Deconnexion\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch(choix) {
            case 1:  menuGestionVentes();  break;
          //  case 2:  consulterStock(); break;
            case 3:  genererRapportJournalier();  break;
            case 0:  clearScreen(); printf("Deconnexion...\n"); break;
            default: printf("Choix invalide !\n");
        }
    } while(choix != 0);
}

int loginUtilisateur() {
    Utilisateur u;
    char login[6], motdepasse[20];
    int trouve = 0;

    FILE *f = fopen("users.dat", "rb");
    if (!f) {
        printf("\033[31mAucun utilisateur trouv� ! Veuillez en cr�er un.\033[0m\n");
        return 0;
    }

    printf("\n\033[1;34m=== CONNEXION ===\033[0m\n");
    printf("Login : ");
    scanf("%5s", login);
    printf("Mot de passe : ");
    scanf("%19s", motdepasse);

    while (fread(&u, sizeof(Utilisateur), 1, f)) {
        if (strcmp(u.login, login) == 0 && strcmp(u.motDePasse, motdepasse) == 0) {
            trouve = 1;
            break;
        }
    }
    fclose(f);

    if (!trouve) {
        printf("\033[31m[ERREUR] Identifiants invalides !\033[0m\n");
        return 0;
    }

    if (u.estActif == 0) {
        printf("\033[31m[ERREUR] Ce compte est bloque.\033[0m\n");
        return 0;
    }

    if (u.premiereConnexion == 1) {
        printf("\033[33mPremiere connexion detectee. Vous devez changer votre mot de passe.\033[0m\n");
        changerMotDePasse(u.login);  // Assure-toi que cette fonction est bien dans utilisateur.c
    }

    if (strcmp(u.role, "ADMIN") == 0) {
        clearScreen();
        menuAdmin();
    } else if (strcmp(u.role, "PHARMA") == 0) {
        clearScreen();
        menuPharmacien();
    } else {
        printf("\033[31m[ERREUR] Role inconnu !\033[0m\n");
    }

    return 1;
}


