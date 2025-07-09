#ifndef VENTES_H
#define VENTES_H

#include <time.h>

typedef struct {
    char code[6];
    char nom[50];
    int quantiteVendue;
    float prixUnitaire;
    float prixTotal;
} ProduitVendu;

typedef struct {
    char code[6];
    char nom[50];
    float prix;
    int quantite;
    char categorie[30];
    time_t datePeremption;
} Produit;

void genererNumeroVente(char* numero);
void creerFacture(ProduitVendu ventes[], int taille, float total, char* login);
void mettreAJourStock(ProduitVendu ventes[], int nb);
void effectuerVente(char* login);

#endif 
