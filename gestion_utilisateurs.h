#ifndef UTILISATEUR_H
#define UTILISATEUR_H

typedef struct {
    char login[8];          // 5 lettres majuscules
    char motDePasse[20];    // par d�faut : pharma123
    char nom[20];
    char prenom[30];
    char telephone[15];
    char role[10];          // ADMIN ou PHARMA
    int estActif;           // 1 = actif, 0 = bloqu�
    int premiereConnexion;  // 1 = oui, 0 = non
} Utilisateur;

void ajouterUtilisateur();
void afficherUtilisateurs();
int verifierLoginUnique(const char* login);
void changerMotDePasse();
void changerStatutUtilisateur();

#endif
