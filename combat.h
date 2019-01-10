#ifndef COMBAT_H
#define COMBAT_H
#include "heros.h"
#include "pokemon.h"

string intToString(int i);
double puissance(int nbre,int puis);
int trajet_pkball(int x,int x0,int x1,int y0,int y1,int angle);

// Affichage de menus spécifiques à la classe pokémon
void affiche_stat(Pokemon a, Pokemon b, bool jouer_sur_mac = true);
int affiche_menu(int liste[nbre_attaques][2], int niveau, Image<AlphaColor> img, bool jouer_sur_mac = true); // affichage attaques
int affiche_menu(market& m, Image<AlphaColor> img, bool jouer_sur_mac = true); // affiche items
int choix_pokemon(heros h, bool jouer_sur_mac = true);

// Animation
void animation_vie(Pokemon a,int ancienne_vie);
void secousse(Pokemon a, int aX, int aY, float coefX, float coefY, int refresh=50);
void clignotement(Pokemon a, int aX, int aY, int n, int refresh=250);
void animation(int i, Pokemon& a, Pokemon& b,bool jouer_sur_mac=true);
// POOKIEBALL
bool Capture_pokemon(Image<AlphaColor> pookieballs[],Image<AlphaColor> explosions[4],Image<AlphaColor> imgMenu, Pokemon& a, Pokemon& b,bool jouer_sur_mac=true); // Lance de la pookieball

// Combat
bool attaque(Pokemon &attaquant,Pokemon &defenseur, int id_attaque, Image<AlphaColor> imgMenu, bool jouer_sur_mac);

#endif // COMBAT_H
