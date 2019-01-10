#ifndef POOKIEMON_H
#define POOKIEMON_H
#include "vector"
#include <ctime>
#include "carte.h"


// Constantes pokémon
const int niveau_max = 100; // niveau max d'un pokémon
const int nombre_types = 5; // nombre de types différents
const int nbre_attaques = 6; // Nombre d'attaques maximum d'un pokemon

void InitRandom();

// ------------- Classe de gestion des attaques
class Attaque{
public:
    // INFORMATIONS GENERALES
    int id; // id de l'attaque
    string nom; // nom de l'attaque
    int degats; // Degats de l'attaque
    int chance; // Probabilite de réussite de l'attaque
    string type; // Type de l'attaque
    int animation; // Indice de l'animation de l'image

    // CHANGEMENT STATS
    int coef_att_adv; // L'attaque du pokemon adverse devient att = att * coef_att_adv/100
    int coef_def_adv; // La defense du pokemon adverse devient def = def * coef_def_adv/100
    int coef_vit_adv; // La vitesse du pokemon adverse devient vit = vit * coef_vit_adv/100
    int coef_att; // L'attaque du pokemon devient att = att * coef_att/100;
    int coef_def; // La defense du pokemon devient def = def * coef_def/100;
    int coef_vit; // La vitesse du pokemon devient vit = vit * coef_vit/100;

    // CHANGEMENT ETATS
    int coef_empoisonne; // Le pokemon adverse a coef_empoisonne % de chance d'être empoisonne;
    int coef_endormi; // Le pokemon adverse a coef_endormi % de chance d'être endormi;
    int coef_paralyse; // Le pokemon adverse a coef_paralyse % de chance d'être paralyse;

    // METHODES
    Attaque(){}
    Attaque(int id0,string nom0, int degats0, int chance0, string type0, int animation0, // Constructeur des attaques,
            int coef_att_adv0 = 100, int coef_def_adv0 = 100, int coef_vit_adv0 = 100,  // Ne change pas les stats
            int coef_att0 = 100, int coef_def0 = 100, int coef_vit0 = 100,              // ni les etats par defaut
            int coef_empoisonne0 = 0,int coef_endormi0 = 0,int coef_paralyse0 = 0);
    void operator= (Attaque attaque);
};


// ------------- Initialisation attaques

// Attaques
// ---> lire un fichier texte, comme pour les pokémons
const int nbre_attaques_total = 10; // Nombre d'attaques dans le jeu au total
const Attaque charge(0,"CHARGE",35,95,"normal",0);
const Attaque trempette(1,"TREMPETTE",0,100,"eau",0);
const Attaque rugissement(2,"RUGISSEMENT",0,100,"normal",0,80);
const Attaque mimi_queue(3,"MIMI-QUEUE",0,100,"normal",0,100,80);
const Attaque eclair(4,"ÉCLAIR",40,100,"electricite",0,100,100,100,100,100,100,0,0,20);
const Attaque fatal_foudre(5,"FATAL-FOUDRE",120,70,"electricite",0,100,100,100,100,100,100,0,0,40);
const Attaque cage_eclair(6,"CAGE-ÉCLAIR",0,100,"electricite",0,100,100,100,100,100,100,0,0,100);
const Attaque armure(7,"ARMURE",0,100,"normal",0,100,100,100,100,125);
const Attaque ultralaser(8,"ULTRALASER",170,100,"normal",0);
const Attaque coupe(9,"COUPE",30,100,"normal",0);
const Attaque liste_attaques[nbre_attaques_total] = {charge, trempette, rugissement, mimi_queue, eclair, fatal_foudre,
                                                     cage_eclair, armure, ultralaser, coupe};


// ------------- Classe de gestion des pokemons
class Pokemon{
public:
    // VARIABLES
    // INFORMATIONS GENERALES
    NativeBitmap sprite; // images
    NativeBitmap sprite_back; // image de dos
    string nom; // Nom du pokemon
    int id; // id du Pokemon, attaché au SPRITE également
    string type; // Indice du type du pokemon
    vector <string> type_super_eff; // Liste des types qui sont super efficaces contre le pokemon
    vector <string> type_pas_eff; // Liste des types qui ne sont pas efficaces
    int vie_max; // Nombre de points de vie maximum du pokemon
    int att_normal; // Attaque du pokemon au tout debut du combat
    int def_normal; // Defense du pokemon au tout debut du combat
    int vit_normal; // Vitesse du pokemon au tout debut du combat

    // COMBAT
    int taux; // Taux de capture du pokemon (entre 1 et 255) : 255 facile a attrape, 1 difficile a attrape
    bool position; // true : au premier plan, false : au deuxieme plan
    int vie; // Nombre de points de vie actuel du pokemon
    int att; // Attaque du pokemon
    int def; // Defense du pokemon
    int vit; // Vitesse du pokemon
    string etat; // Etat du pokemon (empoisonné, endormi,etc...)
    int attaques[nbre_attaques][2]; // Liste des id des attaques du pokemon. Exemple : attaques[i][0] = niveau auquel
                        // on apprend l'attaque d'indice attaques[i][1]

    // EVOLUTION
    int niveau; // Niveau du pokemon
    int xp; // Experience du pokemon
    string vitesse_croissance; // Dit si le pokemon monte de niveau rapidement ou non
    int bonus_vie; // lorsque le pokemon gagne un niveau, sa vie devient vie = vie + bonus_vie/100
    int residu_bonus_vie;
    int bonus_att; // lorsque le pokemon gagne un niveau, son attaque devient att = att + bonus_att/100
    int residu_bonus_att;
    int bonus_def; // lorsque le pokemon gagne un niveau, sa défense devient def = def + bonus_def/100
    int residu_bonus_def;
    int bonus_vit; // lorsque le pokemon gagne un niveau, sa vitesse devient vit = vit + bonus_vit/100
    int residu_bonus_vit;

    // --------> Id du pokémon suivant pour évolution + niveau à atteindre

    // METHODES
    Pokemon();
    Pokemon(NativeBitmap b0, NativeBitmap b1, string nom0,int id0,string type0,int vie_max0,int att_normal0, int def_normal0,
            int vit_normal0,int attaques0[nbre_attaques][2],int niveau0,int bonus_vie0,
            int bonus_att0,int bonus_def0,int bonus_vit0,vector<string> &type_super_eff0,
            vector<string> &type_pas_eff0,string vitesse_croissance0,int taux0);
    int xp_niveau(int niv); // Donne l'xp nécessaire pour aller au niveau niv
    bool Monter_niveau(); // Fait monter d'un ou plusieurs niveaux le pokemon si son xp est suffisante
    void Soin(); // Fixe la vie du pokemon a vie_max et son etat a "normal"
    void Initialisation(); // Fixe les variables de combat à leurs taux normaux
    void operator=(Pokemon &pokemon);
    void operator=(const Pokemon &pokemon);
    bool Mort(); // Test si le pokemon est mort
    int NombreAttaques(); // Donne le nombre d'attaques actuelle du pokemon
    int xp_donne(); // xp que donne le pokemon si il est mis K.O.
    void SetNiveau(int niv); // Met un pokemon au niveau niv avec toutes les stats correspondantes

    // Graphique
    inline void drawFront(int x0, int y0) {
        putNativeBitmap(x0,y0,sprite);
    }

    inline void drawBack(int x0, int y0) {
        putNativeBitmap(x0,y0,sprite_back);
    }
};



#endif // POOKIEMON_H
