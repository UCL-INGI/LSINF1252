typedef struct cellAuteur {
    char *auteur;
    struct cellLivre *Livres;
    struct cellAuteur *next;
} cellAuteur;

typedef struct cellLivre {
    char *titre;
    struct cellLivre *suiv;
} cellLivre;

cellAuteur *existe(cellAuteur *librairie, char *strAuteur);
int compteOuvrage(cellAuteur *librairie, char *strAuteur);
void add(cellAuteur *librairie, char *strAuteur, char *strTitre);
void supprimer(cellAuteur **librairie, char *strAuteur);
