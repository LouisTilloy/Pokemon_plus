#ifndef MARKET_H
#define MARKET_H
#include "vector"
#include <iostream>
using namespace std;


// Structure pour décrire un objet
struct objet {
    objet();
    objet(string s,int q=1,int p=1);
    string nom;
    int prix;
    int quantite; // -----> Ou float pour la bière ?
    bool isBall; // est-ce une pokeball ?

    inline bool exists() {
        return nom!="NULL"; // objet non défini vide
    }

    inline bool disponible(){
        return quantite>0;
    }

    inline void utiliser(){
        if(disponible()) quantite--;
    }
};

// Classe qui gère de manière complète les objets, utile pour la future partie Finance
class market {
    vector<objet> stock; // possède un objet [0] null
public:
    market(); // ajoute un objet vide

    inline int size() {
        return stock.size()-1; // moins l'objet vide
    }
    objet& find(string s); // Recherche par nom, renvoie l'objet vide si rien trouvé
    objet& operator()(string s); // methode précédente via opérateur
    objet& operator[](int i); // Recherche par id, accesseur rapide et ignore le premier objet vide
    void add(string s,int n=1,int p=1);
};

#endif // MARKET_H
