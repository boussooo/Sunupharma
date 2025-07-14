#ifndef CATEGORIES_H
#define CATEGORIES_H

typedef struct {
    int id;
    char libelle[50];
} Categorie;

void menuGestionCategories();
void ajouterCategorie();
void afficherCategories();
void rechercherCategorie();
void supprimerCategorie();


#endif

