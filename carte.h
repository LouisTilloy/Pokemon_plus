#ifndef CARTE_H
#define CARTE_H

#include "vector"
#include "Imagine/Graphics.h"
#include "Imagine/Images.h"
#include <cstdlib>
using namespace Imagine;
using namespace std;


// Constantes carte
const int largeurTile = 32; // largeur des cases en pixels
const int largeurCarte = 4; // nombre de terrains différents
const int largeurTerrain=20; // largeur du terrain en cases
const int nTile=13; // nombre de types de cases différentes
const int nAlphaTile=1;


// Classe qui gère l'affichage des terrains
class carte {
public:
    carte() {}

    int t[largeurTerrain][largeurTerrain]; // la carte symbolique
    NativeBitmap img; // image résultante, stockée pour accélérer l'affichage

    vector<int> listePnj; // id des personnages présents sur la carte

    inline void affiche() {
        putNativeBitmap(0,0,img);
    }

    inline void affiche(int x) {
        putNativeBitmap(x,0,img);
    }
};

// Fonction pratique pour chercher et renvoyer une image bitmap
NativeBitmap initialiserImg(string nom);

// Fonctions d'affichages à partir d'un objet carte

// - affichage de la tile bitmap en x,y de l'objet carte
inline void affiche_tile(const carte& terrain, NativeBitmap tableImg[nTile], int x, int y) {
    if(0<=x && x<largeurTerrain && 0<=y && y<largeurTerrain) {
        putNativeBitmap(x*largeurTile,y*largeurTile,tableImg[terrain.t[y][x]]);
    }
}

// - affichage de la tile transparente en x,y de l'objet carte
inline void affiche_tile(const carte& terrain, Image<AlphaColor> tableAlphaImg[nAlphaTile], int x, int y) {
    if(0<=x && x<largeurTerrain && 0<=y && y<largeurTerrain) {
        if(terrain.t[y][x]==2) {
            display(tableAlphaImg[0],x*largeurTile,y*largeurTile);
        }
    }
}

// - boucle d'affichage pour les objets terrain
void affiche(const carte& terrain, NativeBitmap tableImg[nTile]);
void affiche(const carte& terrain, Image<AlphaColor> tableAlphaImg[nAlphaTile]);

int Clavier();

// Affichage des menus
void affiche_menu(string text, Image<AlphaColor> img, bool jouer_sur_mac = true); // texte simple, disparait au clic
void affiche_menu(string text, int t, Image<AlphaColor> img, bool jouer_sur_mac = true); // texte simple, disparait après un temps t
int affiche_menu(string t1, string t2, string t3, string t4, Image<AlphaColor> img, bool jouer_sur_mac = true); // menu à choix
int affiche_menu(string t[], int taille, Image<AlphaColor> img, bool jouer_sur_mac = true); // menu à choix listé

// Animation
void clignote_dididi(int temps=500,int pas_temps=50);
void didididididi(int choix=-1);

#endif // CARTE_H
