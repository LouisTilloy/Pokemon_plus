#include "carte.h"

// Fonction pratique pour chercher et renvoyer une image bitmap
NativeBitmap initialiserImg(string nom) {
    // Renvoie l'image 'nom.bmp' du répertoire img/
    cout << "Initialisation de "+nom+".bmp : ";
    int w,h;
    byte* rgb;
    bool loaded = loadColorImage(srcPath("img/"+nom+".bmp"),rgb,w,h);
    cout << loaded << endl;
    NativeBitmap img(w,h);
    img.setColorImage(0,0,rgb,w,h);
    delete[] rgb;
    return img;
}

// Fonctions d'affichage à partir d'un objet carte
void affiche(const carte& terrain, NativeBitmap tableImg[nTile]) {
    // Affichage de la carte à partir de la variable terrain
    // Fond opaque statique
    noRefreshBegin();
    for (int x=0;x<largeurTerrain;x++) {
        for (int y=0;y<largeurTerrain;y++) {
            affiche_tile(terrain, tableImg, x, y);
        }
    }
    noRefreshEnd();
}


void affiche(const carte& terrain, Image<AlphaColor> tableAlphaImg[nAlphaTile]) {
    // Affichage de la carte à partir de la variable terrain
    // Tiles transparentes
    noRefreshBegin();
    for (int x=0;x<largeurTerrain;x++) {
        for (int y=0;y<largeurTerrain;y++) {
            affiche_tile(terrain, tableAlphaImg, x, y);
        }
    }
    noRefreshEnd();
}

// Clavier
int Clavier() {
    Event e;
    do {
        getEvent(0,e);
        if (e.type==EVT_KEY_ON)
            return e.key;
    } while (e.type!=EVT_NONE);
    return 0;
}


// Affichage des menus

void affiche_menu(string text, Image<AlphaColor> img, bool jouer_sur_mac) {
    // Affiche un message en bas de l'écran qui disparait lorsqu'on appuie sur A
    flushEvents();
    milliSleep(100); // retard qui donne tout son charme retro
    display(img, 72, largeurTerrain*largeurTile-150);
    drawString(95,largeurTerrain*largeurTile-92,text,BLACK,10+jouer_sur_mac*10);
    while(Clavier()!='a') {
        flushEvents();
        milliSleep(500);
    }
}

void affiche_menu(string text, int t, Image<AlphaColor> img, bool jouer_sur_mac) {
    // Affiche un message en bas de l'écran qui disparait après un temps t
    flushEvents();
    milliSleep(100);
    display(img, 72, largeurTerrain*largeurTile-150);
    drawString(95,largeurTerrain*largeurTile-92,text,BLACK,10+jouer_sur_mac*10);
    milliSleep(t);
}

int affiche_menu(string t1, string t2, string t3, string t4, Image<AlphaColor> img, bool jouer_sur_mac) {
    // Affiche un menu à quatre choix, renvoie le choix selectionné
    flushEvents();
    milliSleep(100); // retard qui donne tout son charme retro
    display(img, 72, largeurTerrain*largeurTile-150);
    drawString(110,largeurTerrain*largeurTile-110,t1,BLACK,10+jouer_sur_mac*10);
    drawString(320,largeurTerrain*largeurTile-110,t2,BLACK,10+jouer_sur_mac*10);
    drawString(110,largeurTerrain*largeurTile-75,t3,BLACK,10+jouer_sur_mac*10);
    drawString(320,largeurTerrain*largeurTile-75,t4,BLACK,10+jouer_sur_mac*10);
    fillRect(100-jouer_sur_mac*5,largeurTerrain*largeurTile-120-jouer_sur_mac*5,5+jouer_sur_mac*5,5+jouer_sur_mac*5,BLACK);

    int clav=0;
    int selecteur = 0;
    while(clav!='a') {
        clav=Clavier();

        if(clav>=16777234) {
            // Effacement ancien selecteur
            fillRect(100-jouer_sur_mac*5+(selecteur%2)*210,largeurTerrain*largeurTile-120-jouer_sur_mac*5+(selecteur>1)*35,5+jouer_sur_mac*5,5+jouer_sur_mac*5,WHITE);

            // Déplacement touches directionnelles
            clav = (clav - 16777234)%4;
            if (clav==0) {selecteur=(selecteur+3)%4;}
            else if (clav==2) {selecteur=(selecteur+1)%4;}
            else {selecteur=(selecteur+2)%4;}

            // Affichage nouveau
            fillRect(100-jouer_sur_mac*5+(selecteur%2)*210,largeurTerrain*largeurTile-120-jouer_sur_mac*5+(selecteur>1)*35,5+jouer_sur_mac*5,5+jouer_sur_mac*5,BLACK);

            flushEvents();
        }

        milliSleep(100);
    }

    return selecteur;
}


int affiche_menu(string t[], int taille, Image<AlphaColor> img, bool jouer_sur_mac) {
    flushEvents();
    milliSleep(100); // retard qui donne tout son charme retro
    display(img, largeurTerrain*largeurTile/2, largeurTerrain*largeurTile-350);

    int departT = 0;

    for(int i=0;i<6 && i<taille;i++) {
        // premiers éléments
        drawString(largeurTerrain*largeurTile/2 + 40, largeurTerrain*largeurTile-300+i*40,t[i],BLACK,10+jouer_sur_mac*10);
    }

    // sélecteur
    fillRect(largeurTerrain*largeurTile/2 + 25-jouer_sur_mac*5,largeurTerrain*largeurTile-309-jouer_sur_mac*3,5+jouer_sur_mac*5,5+jouer_sur_mac*5,BLACK);

    if(taille>6) {
        // affichage d'une flèche en bas
        int x[] = {largeurTerrain*largeurTile/2+120-5, largeurTerrain*largeurTile/2+120, largeurTerrain*largeurTile/2+120+5};
        int y[] = {largeurTerrain*largeurTile-75,largeurTerrain*largeurTile-70,largeurTerrain*largeurTile-75};
        fillPoly(x,y,3,BLACK); // triangle indicateur
    }

    int clav=0;
    int selecteur = 0;

    while(clav!='a') {
        clav=Clavier();

        if(clav>=16777234) {
            // Effacement ancien selecteur
            fillRect(largeurTerrain*largeurTile/2 + 25-jouer_sur_mac*5,largeurTerrain*largeurTile-309-jouer_sur_mac*3+40*(selecteur-departT),5+jouer_sur_mac*5,5+jouer_sur_mac*5,WHITE);

            // Déplacement touches directionnelles
            clav = (clav - 16777234)%4;
            if (clav==1) {
                // haut
                selecteur=max(0,selecteur-1);
                if(selecteur<departT) {
                    departT--;
                    fillRect(largeurTerrain*largeurTile/2+15,largeurTerrain*largeurTile-350+15,205,272,WHITE); // effacement
                    for(int i=departT;(i<departT+6) && (i<taille);i++) {
                        // premiers éléments
                        drawString(largeurTerrain*largeurTile/2 + 40, largeurTerrain*largeurTile-300+(i-departT)*40,t[i],BLACK,10+jouer_sur_mac*10);
                    }
                    if(departT!=0) {
                        // affichage d'une flèche en haut
                        int x[] = {largeurTerrain*largeurTile/2+120-5, largeurTerrain*largeurTile/2+120, largeurTerrain*largeurTile/2+120+5};
                        int y[] = {largeurTerrain*largeurTile-320,largeurTerrain*largeurTile-325,largeurTerrain*largeurTile-320};
                        fillPoly(x,y,3,BLACK); // triangle à la con
                    }
                    if(departT!=taille-1-5) {
                        // affichage d'une flèche en bas
                        int x[] = {largeurTerrain*largeurTile/2+120-5, largeurTerrain*largeurTile/2+120, largeurTerrain*largeurTile/2+120+5};
                        int y[] = {largeurTerrain*largeurTile-75,largeurTerrain*largeurTile-70,largeurTerrain*largeurTile-75};
                        fillPoly(x,y,3,BLACK); // triangle indicateur
                    }
                }
            }
            else if (clav==3) {
                // bas
                selecteur=min(taille-1,selecteur+1);
                if(selecteur>departT+5) {
                    departT++;
                    fillRect(largeurTerrain*largeurTile/2+15,largeurTerrain*largeurTile-350+15,205,272,WHITE); // effacement
                    for(int i=departT; (i<departT+6) && (i<taille);i++) {
                        // premiers éléments
                        drawString(largeurTerrain*largeurTile/2 + 40, largeurTerrain*largeurTile-300+(i-departT)*40,t[i],BLACK,10+jouer_sur_mac*10);
                    }
                    if(departT!=0) {
                        // affichage d'une flèche en haut
                        int x[] = {largeurTerrain*largeurTile/2+120-5, largeurTerrain*largeurTile/2+120, largeurTerrain*largeurTile/2+120+5};
                        int y[] = {largeurTerrain*largeurTile-320,largeurTerrain*largeurTile-325,largeurTerrain*largeurTile-320};
                        fillPoly(x,y,3,BLACK); // triangle à la con
                    }
                    if(departT!=taille-1-5) {
                        // affichage d'une flèche en bas
                        int x[] = {largeurTerrain*largeurTile/2+120-5, largeurTerrain*largeurTile/2+120, largeurTerrain*largeurTile/2+120+5};
                        int y[] = {largeurTerrain*largeurTile-75,largeurTerrain*largeurTile-70,largeurTerrain*largeurTile-75};
                        fillPoly(x,y,3,BLACK); // triangle indicateur
                    }

                }
            }

            // Affichage nouveau selecteur
            fillRect(largeurTerrain*largeurTile/2 + 25-jouer_sur_mac*5,largeurTerrain*largeurTile-309-jouer_sur_mac*3+40*(selecteur-departT),5+jouer_sur_mac*5,5+jouer_sur_mac*5,BLACK);
            flushEvents();
        }

        milliSleep(100);
    }

    return selecteur;
}

void clignote_dididi(int temps, int pas_temps){
    byte *r,*g,*b;
    int w,h;
    captureWindow(r,g,b,w,h);
    NativeBitmap img(w,h);
    for (int i=0;i<temps/pas_temps;i++){
        milliSleep(pas_temps);
        img.setColorImage(0,0,r,g,b,w,h);
        putNativeBitmap(0,0,img,true);
    }
    delete [] r;
    delete [] g;
    delete [] b;
}

void didididididi(int choix){
    int w = largeurTile;
    int h = largeurTile;
    NativeBitmap bloc(w,h);
    byte *rgb = new byte [3*w*h];
    for(int i=0;i<3*w*h;i++)
        rgb[i] = 0;
    bloc.setColorImage(0,0,rgb,w,h); //case noire de cote largeurTile/2

    delete [] rgb;

    bool cases[largeurTerrain][largeurTerrain];
    for (int i=0;i<largeurTerrain;i++){
        for(int j=0;j<largeurTerrain;j++){
            cases[i][j]=false;
        }
    }
    if (choix==-1) choix = rand()%9;
    if(choix == 0){  // Spirale exterieur -> interieur
        int x=0,y=0;
        int n = 1;
        cases[x][y] = true;
        putNativeBitmap(0,0,bloc);
        while(n<largeurTerrain*largeurTerrain){
            while( y<largeurTerrain-1 && !cases[x][y+1] ){
                milliSleep(5);
                cases[x][y+1]=true;
                y ++;
                n ++;
                putNativeBitmap(x*largeurTile,y*largeurTile,bloc);
            }
            while( x<largeurTerrain-1 && !cases[x+1][y] ){
                milliSleep(5);
                cases[x+1][y] = true;
                x ++;
                n ++;
                putNativeBitmap(x*largeurTile,y*largeurTile,bloc);
            }
            while( 0<y && !cases[x][y-1] ){
                milliSleep(5);
                cases[x][y-1] = true;
                y --;
                n++ ;
                putNativeBitmap(x*largeurTile,y*largeurTile,bloc);
            }
            while( 0<x && !cases[x-1][y] ){
                milliSleep(5);
                cases[x-1][y] = true;
                x --;
                n ++;
                putNativeBitmap(x*largeurTile,y*largeurTile,bloc);
            }
        }
        milliSleep(1000);
    }
    if (choix == 1){ // Spirale interieur -> exterieur
        int x=largeurTerrain/2,y=largeurTerrain/2;
        int n = 2;
        cases[x][y] = true;
        putNativeBitmap(x*largeurTile,y*largeurTile,bloc);
        y--;
        cases[x][y] = true;
        putNativeBitmap(x*largeurTile,y*largeurTile,bloc);
        while(n<largeurTerrain*largeurTerrain){
            while( x<largeurTerrain-1 && y<largeurTerrain-1 && cases[x+1][y] ){
                milliSleep(5);
                cases[x][y+1]=true;
                y ++;
                n ++;
                putNativeBitmap(x*largeurTile,y*largeurTile,bloc);
            }
            while( 0<y && x<largeurTerrain-1 && cases[x][y-1] ){
                milliSleep(5);
                cases[x+1][y] = true;
                x ++;
                n ++;
                putNativeBitmap(x*largeurTile,y*largeurTile,bloc);
            }
            while( 0<y && 0<x && cases[x-1][y] ){
                milliSleep(5);
                cases[x][y-1] = true;
                y --;
                n++ ;
                putNativeBitmap(x*largeurTile,y*largeurTile,bloc);
            }
            while( 0<x && y<largeurTerrain-1 && cases[x][y+1] ){
                milliSleep(5);
                cases[x-1][y] = true;
                x --;
                n ++;
                putNativeBitmap(x*largeurTile,y*largeurTile,bloc);
            }
        }
        milliSleep(1000);
    }
    if (choix == 2){ // Cercle simple
        clignote_dididi();
        int x0=largeurTerrain*largeurTile/2,y0=largeurTerrain*largeurTile/2;
        int x1=largeurTerrain*largeurTile-1,y1=largeurTerrain*largeurTile/2;
        int x=x1,y=y1;
        double X,Y;
        for (;y>0;y = y-largeurTile){
            milliSleep(5);
            noRefreshBegin();
            for (int i=0;i<100;i ++){
               X = x0*i/100.+x*(100-i)/100.;
               Y = y0*i/100.+y*(100-i)/100.;
               putNativeBitmap(int(X/largeurTile)*largeurTile,int(Y/largeurTile)*largeurTile,bloc);
            }
            noRefreshEnd();
        }
        for (;x>0;x=x-largeurTile){
            milliSleep(5);
            noRefreshBegin();
            for (int i=0;i<100;i ++){
               X = x0*i/100.+x*(100-i)/100.;
               Y = y0*i/100.+y*(100-i)/100.;
               putNativeBitmap(int(X/largeurTile)*largeurTile,int(Y/largeurTile)*largeurTile,bloc);
            }
            noRefreshEnd();
        }
        for (;y<largeurTerrain*largeurTile;y += largeurTile){
            milliSleep(5);
            noRefreshBegin();
            for (int i=0;i<100;i ++){
               X = x0*i/100.+x*(100-i)/100.;
               Y = y0*i/100.+y*(100-i)/100.;
               putNativeBitmap(int(X/largeurTile)*largeurTile,int(Y/largeurTile)*largeurTile,bloc);
            }
            noRefreshEnd();
        }
        for (;x<largeurTerrain*largeurTile;x += largeurTile){
            milliSleep(5);
            noRefreshBegin();
            for (int i=0;i<100;i ++){
               X = x0*i/100.+x*(100-i)/100.;
               Y = y0*i/100.+y*(100-i)/100.;
               putNativeBitmap(int(X/largeurTile)*largeurTile,int(Y/largeurTile)*largeurTile,bloc);
            }
            noRefreshEnd();
        }
        for (;largeurTerrain*largeurTile/2<y;y -= largeurTile){
            milliSleep(5);
            noRefreshBegin();
            for (int i=0;i<100;i ++){
               X = x0*i/100.+x*(100-i)/100.;
               Y = y0*i/100.+y*(100-i)/100.;
               putNativeBitmap(int(X/largeurTile)*largeurTile,int(Y/largeurTile)*largeurTile,bloc);
            }
            noRefreshEnd();
        }
        milliSleep(1000);
    }
    if (choix==3){ // Cercle double
        clignote_dididi();
        int x0=0,y0=largeurTerrain*largeurTile/2;
        int x1=largeurTerrain*largeurTile-1,y1=largeurTerrain*largeurTile/2;
        double X,Y;
        for(;y1>0;y1-=largeurTile){
            milliSleep(5);
            y0 +=largeurTile;
            noRefreshBegin();
            for (int i=0;i<100;i ++){
               X = x0*i/100.+x1*(100-i)/100.;
               Y = y0*i/100.+y1*(100-i)/100.;
               putNativeBitmap(int(X/largeurTile)*largeurTile,int(Y/largeurTile)*largeurTile,bloc);
            }
            noRefreshEnd();
        }
        for(;x1>0;x1-=largeurTile){
            milliSleep(5);
            x0 +=largeurTile;
            noRefreshBegin();
            for (int i=0;i<100;i ++){
               X = x0*i/100.+x1*(100-i)/100.;
               Y = y0*i/100.+y1*(100-i)/100.;
               putNativeBitmap(int(X/largeurTile)*largeurTile,int(Y/largeurTile)*largeurTile,bloc);
            }
            noRefreshEnd();
        }
        for(;x1>0;x1-=largeurTile){
            milliSleep(5);
            x0 +=largeurTile;
            noRefreshBegin();
            for (int i=0;i<100;i ++){
               X = x0*i/100.+x1*(100-i)/100.;
               Y = y0*i/100.+y1*(100-i)/100.;
               putNativeBitmap(int(X/largeurTile)*largeurTile,int(Y/largeurTile)*largeurTile,bloc);
            }
            noRefreshEnd();
        }
        for(;y1<largeurTerrain*largeurTile/2;y1+=largeurTile){
            milliSleep(5);
            y0 -=largeurTile;
            noRefreshBegin();
            for (int i=0;i<100;i ++){
               X = x0*i/100.+x1*(100-i)/100.;
               Y = y0*i/100.+y1*(100-i)/100.;
               putNativeBitmap(int(X/largeurTile)*largeurTile,int(Y/largeurTile)*largeurTile,bloc);
            }
            noRefreshEnd();
        }
        milliSleep(1000);
    }
    if (choix==4){ // barres horizontales
        for(int x=0;x<largeurTerrain*3+1;x++){
            milliSleep(30);
            noRefreshBegin();
            for (int y=0;y<largeurTerrain/2;y++){
                putNativeBitmap(x*largeurTile/3,y*2*largeurTile,bloc);
                putNativeBitmap((largeurTerrain*3-x)*largeurTile/3,(y*2+1)*largeurTile,bloc);
            }
            noRefreshEnd();
        }
        milliSleep(1000);
    }
    if (choix==5){ // barres verticales
        for(int y=0;y<largeurTerrain*3+1;y++){
            milliSleep(30);
            noRefreshBegin();
            for (int x=0;x<largeurTerrain/2;x++){
                putNativeBitmap(x*2*largeurTile,y*largeurTile/3,bloc);
                putNativeBitmap((x*2+1)*largeurTile,(largeurTerrain*3-y)*largeurTile/3,bloc);
            }
            noRefreshEnd();
        }
        milliSleep(1000);
    }
    if (choix==6){ // Ecartement en 4 images
        byte *r,*g,*b;
        int w,h;
        captureWindow(r,g,b,w,h);

        captureRectangle(0,0,r,g,b,w/2,h/2);
        NativeBitmap hautGauche(w/2,h/2);
        hautGauche.setColorImage(0,0,r,g,b,w/2,h/2);

        captureRectangle(w/2,0,r,g,b,w/2,h/2);
        NativeBitmap hautDroite(w/2,h/2);
        hautDroite.setColorImage(0,0,r,g,b,w/2,h/2);

        captureRectangle(0,h/2,r,g,b,w/2,h/2);
        NativeBitmap basGauche(w/2,h/2);
        basGauche.setColorImage(0,0,r,g,b,w/2,h/2);

        captureRectangle(w/2,h/2,r,g,b,w/2,h/2);
        NativeBitmap basDroite(w/2,h/2);
        basDroite.setColorImage(0,0,r,g,b,w/2,h/2);

        for (int i=0;i<largeurTerrain/2+1;i++){
            noRefreshBegin();
            milliSleep(100);
            fillRect(0,0,largeurTerrain*largeurTile,largeurTerrain*largeurTile,BLACK);
            putNativeBitmap(-i*largeurTile,-i*largeurTile,hautGauche);
            putNativeBitmap((i+largeurTerrain/2)*largeurTile,-i*largeurTile,hautDroite);
            putNativeBitmap(-i*largeurTile,(i+largeurTerrain/2)*largeurTile,basGauche);
            putNativeBitmap((i+largeurTerrain/2)*largeurTile,(i+largeurTerrain/2)*largeurTile,basDroite);
            noRefreshEnd();
        }
        delete [] r;
        delete [] g;
        delete [] b;
        milliSleep(1000);
    }
    if (choix==7){ // Reduction de l'image
        byte *r,*g,*b;
        int w,h;
        captureWindow(r,g,b,w,h);

        NativeBitmap hautGauche[largeurTerrain/2+1];
        NativeBitmap hautDroite[largeurTerrain/2+1];
        NativeBitmap basGauche[largeurTerrain/2+1];
        NativeBitmap basDroite[largeurTerrain/2+1];
        for (int i=0;i<largeurTerrain/2;i++){
            captureRectangle(0,0,r,g,b,w/2-largeurTile*i,h/2-largeurTile*i);
            hautGauche[i] = NativeBitmap (w/2-i*largeurTile,h/2-i*largeurTile);
            hautGauche[i].setColorImage(0,0,r,g,b,w/2-largeurTile*i,h/2-largeurTile*i);
            captureRectangle(w/2+largeurTile*i,0,r,g,b,w/2-largeurTile*i,h/2-largeurTile*i);
            hautDroite[i] = NativeBitmap (w/2-i*largeurTile,h/2-i*largeurTile);
            hautDroite[i].setColorImage(0,0,r,g,b,w/2-largeurTile*i,h/2-largeurTile*i);
            captureRectangle(0,h/2+i*largeurTile,r,g,b,w/2-largeurTile*i,h/2-largeurTile*i);
            basGauche[i] = NativeBitmap (w/2-i*largeurTile,h/2-i*largeurTile);
            basGauche[i].setColorImage(0,0,r,g,b,w/2-largeurTile*i,h/2-largeurTile*i);
            captureRectangle(w/2+largeurTile*i,h/2+largeurTile*i,r,g,b,w/2-largeurTile*i,h/2-largeurTile*i);
            basDroite[i] = NativeBitmap (w/2-i*largeurTile,h/2-i*largeurTile);
            basDroite[i].setColorImage(0,0,r,g,b,w/2-largeurTile*i,h/2-largeurTile*i);
        }

        for (int i=0;i<largeurTerrain/2+1;i++){
            milliSleep(100);
            noRefreshBegin();
            fillRect(0,0,largeurTerrain*largeurTile,largeurTerrain*largeurTile,BLACK);
            putNativeBitmap(i*largeurTile,i*largeurTile,hautGauche[i]);
            putNativeBitmap(w/2,i*largeurTile,hautDroite[i]);
            putNativeBitmap(i*largeurTile,h/2,basGauche[i]);
            putNativeBitmap(w/2,h/2,basDroite[i]);
            noRefreshEnd();
        }
        fillRect(0,0,largeurTerrain*largeurTile,largeurTerrain*largeurTile,BLACK);
        delete [] r;
        delete [] g;
        delete [] b;
        milliSleep(1000);
    }
    if (choix==8){ // Pixelisation random
        clignote_dididi();
        int y0;
        for(int k=0;k<largeurTerrain;k++){
            for(int x=0;x<largeurTerrain;x++){
                do {
                    y0 = rand()%largeurTerrain;
                } while (cases[x][y0] == true);
                cases[x][y0] = true;
                putNativeBitmap(x*largeurTile,y0*largeurTile,bloc);
            }
        }
        milliSleep(1000);
    }
}
