#ifndef HEADER_H
#define HEADER_H

#define MAX_PRODUCTS 100
#define CODE_LENGTH 6
#define NOM_LENGTH 50
#define CATEGORY_LENGTH 30

typedef struct {
    char code[CODE_LENGTH];
    char designation[NOM_LENGTH];
    float prix;
    int quantite;
    char categorie[CATEGORY_LENGTH];
    char date_peremption[11];
} Produit;


void ajouterProduit();
void modifierProduit();
void supprimerProduit();
void afficherProduits();
void rechercherProduitParCode();
void mettreAJourStock();
void sauvegarderProduits();
void chargerProduits();
#endif
