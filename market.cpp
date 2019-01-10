#include "market.h"

objet::objet() {
    // objet null
    nom = "NULL";
    prix = 0;
    quantite = 0;
    isBall = false;
}

objet::objet(string s,int q,int p) {
    nom = s;
    quantite = q;
    prix = p;
    isBall = (nom=="Pookieball"||nom=="Boule"||nom=="Bièreball");
}


market::market() {
    // On ajoute un objet nul pour le renvoyer dans la recherche
    stock.push_back(objet());
}

objet& market::find(string s) {
    int i=0;
    for(int i=0;i<stock.size();i++) {
        if (stock[i].nom == s)
            return stock[i];
    }
    return stock[0]; // objet vide
}

objet& market::operator()(string s) {
    return find(s);
}

objet& market::operator[](int i) {
    if(i<size())
        return stock[i+1];
    else
        return stock[0]; // objet nul
}

void market::add(string s,int n,int p) {
    stock.push_back(objet(s,n,p));
}
