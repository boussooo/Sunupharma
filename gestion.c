#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "gestion_utilisateurs.h"

#define FICHIER_UTILISATEURS "users.dat"

int verifierLoginUnique(const char* login) {
    Utilisateur u;
    FILE *f = fopen(FICHIER_UTILISATEURS, "rb");
    if (!f) return 1; // Fichier vide, login dispo
    while (fread(&u, sizeof(Utilisateur), 1, f)) {
        if (strcmp(u.login, login) == 0) {
            fclose(f);
            return 0; // Login d�j� existant
        }
    }
    fclose(f);
    return 1; // Login unique
}

void ajouterUtilisateur() {
    Utilisateur u;
    printf("=== AJOUT UTILISATEUR ===\n");

    do {
        printf("Login (5 lettres MAJ) : ");
        scanf("%s", u.login);
        if (strlen(u.login) != 5) {
            printf("Login doit avoir 5 lettres !\n");
            continue;
        }
        int ok = 1;
        for (int i = 0; i < 5; i++) {
            if (!isupper(u.login[i])) ok = 0;
        }
        if (!ok) {
            printf("Login invalide (MAJ uniquement)\n");
        } else if (!verifierLoginUnique(u.login)) {
            printf("Login deja utilise !\n");
        } else {
            break;
        }
    } while (1);

    printf("Nom : ");
    scanf("%s", u.nom);
    printf("Prenom : ");
    scanf("%s", u.prenom);
    printf("Telephone : ");
    scanf("%s", u.telephone);

    do {
        printf("Role (ADMIN ou PHARMACIEN) : ");
        scanf("%s", u.role);
        if (strcmp(u.role, "ADMIN") != 0 && strcmp(u.role, "PHARMACIEN") != 0) {
            printf("Role invalide !\n");
        } else break;
    } while (1);

    strcpy(u.motDePasse, "pharma123");
    u.estActif = 1;
    u.premiereConnexion = 1;

    FILE *f = fopen(FICHIER_UTILISATEURS, "ab");
    if (f) {
        fwrite(&u, sizeof(Utilisateur), 1, f);
        fclose(f);
        printf("\033[32m[OK] Utilisateur ajoute avec succes.\033[0m\n");
    } else {
        printf("\033[31m[ERREUR] Impossible d'ecrire dans le fichier.\033[0m\n");
    }
}

// LISTER UTILISATEURS

void listerUtilisateurs() {
    FILE *f = fopen("users.dat", "rb");
    if (!f) {
        printf("\033[31m[ERREUR] Impossible d'ouvrir users.dat\033[0m\n");
        return;
    }

    Utilisateur u;
    int compteur = 0;

    // En-t�te stylis�e
    printf("\033[1;34m\n=== LISTE DES UTILISATEURS ===\033[0m\n");
    printf("\033[0;33m%-6s | %-15s | %-15s | %-12s | %-8s | %-6s | %-6s\033[0m\n",
           "Login", "Nom", "Prenom", "Telephone", "Role", "Actif", "1eCo");
    printf("-------------------------------------------------------------------------------\n");

    while (fread(&u, sizeof(Utilisateur), 1, f)) {
        printf("%-6s | %-15s | %-15s | %-12s | %-8s | %-6s | %-6s\n",
               u.login,
               u.nom,
               u.prenom,
               u.telephone,
               u.role,
               u.estActif ? "Oui" : "Non",
               u.premiereConnexion ? "Oui" : "Non"
        );
        compteur++;
    }

    if (compteur == 0) {
        printf("Aucun utilisateur trouve.\n");
    }

    fclose(f);
}
// CHANGER DE MDP
void changerMotDePasse(const char* login) {
    FILE *f = fopen("users.dat", "r+b"); // mode lecture+�criture binaire
    if (!f) {
        printf("\033[31m[ERREUR] Impossible d'ouvrir users.dat\033[0m\n");
        return;
    }

    Utilisateur u;
    int trouve = 0;

    while (fread(&u, sizeof(Utilisateur), 1, f)) {
        if (strcmp(u.login, login) == 0) {
            trouve = 1;
            break;
        }
    }

    if (!trouve) {
        printf("\033[31mUtilisateur non trouve.\033[0m\n");
        fclose(f);
        return;
    }

    char nouveauMDP[20];
    char confirmation[20];

    do {
        printf("Nouveau mot de passe : ");
        scanf("%19s", nouveauMDP);
        printf("Confirmer mot de passe : ");
        scanf("%19s", confirmation);

        if (strcmp(nouveauMDP, confirmation) != 0) {
            printf("\033[31mLes mots de passe ne correspondent pas. Reessayez.\033[0m\n");
        } else if (strlen(nouveauMDP) < 6) {
            printf("\033[31mLe mot de passe doit contenir au moins 6 caracteres.\033[0m\n");
        } else {
            break;
        }
    } while (1);

    // Mettre � jour la structure utilisateur
    strcpy(u.motDePasse, nouveauMDP);
    u.premiereConnexion = 0;

    // Reculer le curseur au d�but de cet enregistrement pour l'�craser
    fseek(f, -sizeof(Utilisateur), SEEK_CUR);
    fwrite(&u, sizeof(Utilisateur), 1, f);

    fclose(f);

    printf("\033[32mMot de passe modifie avec succes !\033[0m\n");
}

// CHANGER DE STATUT

void changerStatutUtilisateur() {
    char login[6];
    int nouveauStatut;
    int trouve = 0;

    printf("\n\033[1;34m=== BLOCAGE / DEBLOCAGE UTILISATEUR ===\033[0m\n");
    printf("Login de l'utilisateur a modifier : ");
    scanf("%5s", login);

    FILE *f = fopen("users.dat", "r+b");
    if (!f) {
        printf("\033[31m[ERREUR] Impossible d'ouvrir users.dat\033[0m\n");
        return;
    }

    Utilisateur u;
    while (fread(&u, sizeof(Utilisateur), 1, f)) {
        if (strcmp(u.login, login) == 0) {
            trouve = 1;
            printf("Utilisateur trouve : %s %s (%s)\n", u.nom, u.prenom, u.login);
            printf("Statut actuel : %s\n", u.estActif ? "Actif" : "Bloque");

            do {
                printf("Nouveau statut (1 = Actif, 0 = Bloque) : ");
                scanf("%d", &nouveauStatut);
            } while (nouveauStatut != 0 && nouveauStatut != 1);

            u.estActif = nouveauStatut;

            // Revenir � la bonne position pour �craser
            fseek(f, -sizeof(Utilisateur), SEEK_CUR);
            fwrite(&u, sizeof(Utilisateur), 1, f);

            printf("\033[32m[OK] Statut mis a jour avec succes.\033[0m\n");
            break;
        }
    }

    fclose(f);

    if (!trouve) {
        printf("\033[31m[ERREUR] Utilisateur non trouve.\033[0m\n");
    }
}





