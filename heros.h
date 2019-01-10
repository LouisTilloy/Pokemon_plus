#ifndef HEROS_H
#define HEROS_H
#include "pokemon.h"
#include "market.h"

// constante déplacement héros
const int vecteurDir[4][2] = {{-1,0},{0,-1},{1,0},{0,1}};


// Super-Classe qui gère les personnages, doit contenir des pokémons, des objets, etc.
class heros {
public:
    heros();
    // Attention : coordonnées en cases
    int x;
    int y;
    int argent; // Argent du hero
    market objets; // Liste des objets
    string nom;

    vector <Pokemon> pokemons; // liste des pokemons du hero

    vector <string> dialogue; // liste de textes à dire pour les pnj

    Image<AlphaColor> img[8]; // images pour tous les sens et pour les déplacements
    int sens; // vers quel côté le personnage regarde

    // Accesseurs
    inline int nextX() {
        return x+vecteurDir[sens][0];
    }
    inline int nextY() {
        return y+vecteurDir[sens][1];
    }

    // Jeu
    void affiche();
    void affiche(int x0); // utilisé pour l'animation de changement de terrain
    bool collision(const int terrain[largeurTerrain][largeurTerrain]);
    bool collision(heros pnj);
    void avancer(const int dir,  const carte& terrain, NativeBitmap tableImg[nTile], Image<AlphaColor> tableAlphaImg[nAlphaTile]);
    int indice_premier(); // premier pokemon disponible
};



#endif // HEROS_H
