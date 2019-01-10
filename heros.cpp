#include "heros.h"

heros::heros() {
    x=2;
    y=2;
    sens=3;
    nom = "Dresseur";
}

void heros::affiche() {
    display(img[sens],x*largeurTile,y*largeurTile);
}

void heros::affiche(int x0) {
    display(img[sens],x0,y*largeurTile);
}

bool heros::collision(const int terrain[largeurTerrain][largeurTerrain]) {
    int x2=nextX();
    int y2=nextY();
    if (0<=x2 && x2<largeurTerrain && 0<=y2 && y2<largeurTerrain)
        return (terrain[y2][x2]>2);
    else
        return false;
}


bool heros::collision(heros pnj) {
    int x2=nextX();
    int y2=nextY();
    return (x2 == pnj.x) && (y2 == pnj.y);
}

void heros::avancer(const int dir, const carte& terrain, NativeBitmap tableImg[nTile], Image<AlphaColor> tableAlphaImg[nAlphaTile]) {
    // -- Animation déplacement
    // L'astuce c'est qu'on ne réaffiche que les tiles devant et derrière le personage pendant son déplacement
    int etapes = 4; // nb d'étapes de l'animation, paramètre ajustable en fonction de l'effet voulu
    int x2 = nextX();
    int y2 = nextY();
    for(float t=0;t<etapes;t++) {
        noRefreshBegin();
        // tiles opaques
        affiche_tile(terrain, tableImg, x, y);
        affiche_tile(terrain, tableImg, x2, y2);
        // heros
        display(img[sens+4*(int(t)%2)], (x+t/etapes*(x2-x))*largeurTile, (y+t/etapes*(y2-y))*largeurTile);
        // Tiles transparentes, affichage en fonction de la disposition
        // par exemple : le héros doit passer sur l'herbe en se déplacement de haut en bas
        // alors que sur le côté, il reste derrière l'herbe durant le déplacement
        if(y2<y) {
            affiche_tile(terrain, tableAlphaImg, x, y);
        }
        else if(y<y2) {
            affiche_tile(terrain, tableAlphaImg, x2, y2);
        }
        else {
            affiche_tile(terrain, tableAlphaImg, x, y);
            affiche_tile(terrain, tableAlphaImg, x2, y2);
        }

        noRefreshEnd();
        milliSleep(60);
    }
    // flush event pour ne pas accumuler d'events pendant le déplacement
    flushEvents();

    x=x2;
    y=y2;
}

int heros::indice_premier(){
    for(int i=0;i<pokemons.size();i++){
        if (!pokemons[i].Mort())
            return i;
    }
    return -1;
}
