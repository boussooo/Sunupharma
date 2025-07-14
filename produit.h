#ifndef PRODUITS_H
#define PRODUITS_H

#define MAX_PRODUCTS 100
#define CODE_LENGTH 6
#define NAME_LENGTH 50
#define CATEGORY_LENGTH 30

typedef struct {
    char code[CODE_LENGTH];
    char designation[NAME_LENGTH];
    float prix;
    int quantite;
    char categorie[CATEGORY_LENGTH];
    char date_peremption[11];
} Produit;



// Fonctions principales
void menuGestionProduits();
void afficherProduits();

// Fonctions de gestion
void ajouterEtSauvegarderProduit();
void modifierEtSauvegarderProduit();
void supprimerEtSauvegarderProduit();

// Fonctions de persistance
void chargerProduitsDepuisFichier();
void sauvegarderProduits();

// Fonctions utilitaires
int estCodeValide(const char* code);
int estDateValide(const char* date);
void afficherSucces(const char* message);
void afficherErreur(const char* message);

#endif



