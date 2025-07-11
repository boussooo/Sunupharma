

#ifndef CATEGORIE_H
#define CATEGORIE_H

typedef struct {
    int id;
    char libelle[50];
} Categorie;

void ajouterCategorie();
void afficherCategories();
void rechercherCategorie();
void supprimerCategorie();


#endif

