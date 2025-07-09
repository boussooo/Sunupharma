#include <stdio.h>
#include "ventes.h"

int main() {
    char login[6];

    printf("===== MODULE DE TEST : GESTION DES VENTES =====\n");
    printf("Entrez le login du pharmacien (5 lettres majuscules) : ");
    scanf("%s", login);

    effectuerVente(login);

    return 0;
}
