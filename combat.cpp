#include <sstream>
#include "combat.h"
#include<math.h>

string intToString(int i) {
     ostringstream oss;
     oss << i;
     return oss.str();
}

double puissance(int nbre, int puis){
    double x = 1;
    if (puis<0){
        for(int i=0;i>puis;i--)
            x /= nbre;
    }
    if(puis>=0){
        for(int i=0;i<puis;i++)
            x *= nbre;
    }
    return x;
}

int trajet_pkball(int x,int x0, int x1, int y0, int y1, int alpha){
    // L'equation de la trajectoire d'un objet lance à la vitesse V0 qui part de (x0,y0) et arrive en (x1,y1)
    // en etant soumis a la gravite
    int V0 = sqrt(1/((2*(y1-y0+tan((alpha*M_PI)/180)*(x1-x0))/(10*(x1-x0)*(x1-x0)))))/cos((alpha*M_PI)/180);
    return (10/2*((x-x0)/(V0*cos((alpha*M_PI)/180)))*((x-x0)/(V0*cos((alpha*M_PI)/180)))-tan((alpha*M_PI)/180)*(x-x0)+y0);
}

string majuscules(string s){
    for (int i=0;i<s.size();i++){
        if (s[i]>='a' && s[i]<='z')
            s[i] -= 32;
    }
    return s;
}

// Menus spécifiques aux pookiemon

void affiche_stat(Pokemon a, Pokemon b, bool jouer_sur_mac) {
    noRefreshBegin();
    fillRect(0,0,largeurTerrain*largeurTile,largeurTerrain*largeurTile,WHITE); // effacement des anciens menus
    if (a.position == false){
        Pokemon temp = a;
        a = b;
        b = temp;
    }

    // pokémon A
    a.drawBack(20,largeurTerrain*largeurTile-330);

    drawString(largeurTerrain*largeurTile-200,largeurTerrain*largeurTile-230,a.nom,BLACK,10+10*jouer_sur_mac); // nom
    drawString(largeurTerrain*largeurTile-200,largeurTerrain*largeurTile-210,"Lv "+intToString(a.niveau),BLACK,8+8*jouer_sur_mac,0,false,true); // niveau
    if (a.etat != "normal") drawString(largeurTerrain*largeurTile-140,largeurTerrain*largeurTile-220,majuscules(a.etat.substr(0,3)),BLACK,7+jouer_sur_mac*7,true); // etat
    drawString(largeurTerrain*largeurTile-220,largeurTerrain*largeurTile-192,"PV",BLACK,5+5*jouer_sur_mac,0,false,true); // Vie
    drawString(largeurTerrain*largeurTile-190,largeurTerrain*largeurTile-170,intToString(a.vie)+"/"+intToString(a.vie_max),Color(50,50,50),8+jouer_sur_mac*8);
    drawRect(largeurTerrain*largeurTile-200,largeurTerrain*largeurTile-200,180,10,BLACK,2);
    Color pv1(10,210,70);
    if(float(a.vie)/a.vie_max<0.25)
        pv1 = Color(200,0,0);
    else if (float(a.vie)/a.vie_max<0.5)
        pv1 = Color(255,150,0);
    fillRect(largeurTerrain*largeurTile-196,largeurTerrain*largeurTile-197,(172*a.vie)/a.vie_max,4,pv1);
    int taille_xp = (172*(a.xp-a.xp_niveau(a.niveau)))/(a.xp_niveau(a.niveau+1)-a.xp_niveau(a.niveau));
    fillRect(largeurTerrain*largeurTile-196+(172-taille_xp),largeurTerrain*largeurTile-162,taille_xp,4,BLUE);
    drawRect(largeurTerrain*largeurTile-194,largeurTerrain*largeurTile-164,174,8,BLACK,2);
    drawLine(largeurTerrain*largeurTile-197,largeurTerrain*largeurTile-164,largeurTerrain*largeurTile-23,
             largeurTerrain*largeurTile-164,WHITE,2);

    // pokémon B
    b.drawFront(largeurTerrain*largeurTile-250,50);

    drawString(50,30,b.nom,BLACK,10+jouer_sur_mac*10); // nom
    drawString(50,50,"Lv "+intToString(b.niveau),BLACK,8+jouer_sur_mac*8,0,false,true); // niveau
    if (b.etat != "normal") drawString(100,47,majuscules(b.etat.substr(0,3)),BLACK,7+jouer_sur_mac*7,true); // etat
    drawString(30,68,"PV",BLACK,5+jouer_sur_mac*5,0,false,true); // Vie
    drawString(60,90,intToString(b.vie)+"/"+intToString(b.vie_max),Color(50,50,50),9+jouer_sur_mac*9);
    drawRect(50,60,180,10,BLACK,2);
    Color pv2(10,210,70);
    if(float(b.vie)/b.vie_max<0.25)
        pv2 = Color(200,0,0);
    else if (float(b.vie)/b.vie_max<0.5)
        pv2 = Color(255,150,0);
    fillRect(54,63,(172*b.vie)/b.vie_max,4,pv2);
    noRefreshEnd();
}


int affiche_menu(int liste[nbre_attaques][2], int niveau, Image<AlphaColor> img,bool jouer_sur_mac) {
    // ----------> UTILISER LA FONCTION affiche_menu(t[],...) ?
    flushEvents();
    milliSleep(100); // retard qui donne tout son charme retro
    display(img, largeurTerrain*largeurTile/2, largeurTerrain*largeurTile-350);
    int nAttaquesDispo = 0;

    while(liste[nAttaquesDispo][0]<=niveau && nAttaquesDispo<nbre_attaques) {
        drawString(largeurTerrain*largeurTile/2 + 40, largeurTerrain*largeurTile-300+nAttaquesDispo*40,liste_attaques[liste[nAttaquesDispo][1]].nom,BLACK,10+jouer_sur_mac*10);
        nAttaquesDispo++;
    }

    // sélecteur
    fillRect(largeurTerrain*largeurTile/2 + 25-jouer_sur_mac*5,largeurTerrain*largeurTile-309-jouer_sur_mac*3,5+jouer_sur_mac*5,5+jouer_sur_mac*5,BLACK);

    int clav=0;
    int selecteur = 0;

    while(clav!='a') {
        clav=Clavier();

        if(clav>=16777234) {
            // Effacement ancien selecteur
            fillRect(largeurTerrain*largeurTile/2 + 25-jouer_sur_mac*5,largeurTerrain*largeurTile-309-jouer_sur_mac*3+40*selecteur,5+jouer_sur_mac*5,5+jouer_sur_mac*5,WHITE);

            // Déplacement touches directionnelles
            clav = (clav - 16777234)%4;
            if (clav==1) {selecteur=(selecteur+nAttaquesDispo-1)%nAttaquesDispo;}
            else if (clav==3) {selecteur=(selecteur+1)%nAttaquesDispo;}

            // Affichage nouveau selecteur
            fillRect(largeurTerrain*largeurTile/2 + 25-jouer_sur_mac*5,largeurTerrain*largeurTile-309-jouer_sur_mac*3+40*selecteur,5+jouer_sur_mac*5,5+jouer_sur_mac*5,BLACK);
            flushEvents();
        }

        milliSleep(100);
    }

    return selecteur;
}


int affiche_menu(market& m, Image<AlphaColor> img, bool jouer_sur_mac) {
    string* t = new string[m.size()];
    for (int i=0;i<m.size();i++)
        t[i] = intToString(m[i].quantite) + " x " + m[i].nom;
    int c = affiche_menu(t,m.size(),img,jouer_sur_mac);
    delete[] t;
    return c;
}



int choix_pokemon(heros h, bool jouer_sur_mac) {
    // Affichage des pokémons et choix
    fillRect(0,0,largeurTile*largeurTile,largeurTile*largeurTile,WHITE); // effacement plan
    int nmax = h.pokemons.size();

    // pokémons stats
    for(int i=0;i<nmax;i++) {
        Pokemon p = h.pokemons[i]; // variable locale pour alléger
        p.drawFront(largeurTerrain*largeurTile-130,10+100*i);
        fillRect(largeurTerrain*largeurTile-130,110+100*i,130,50,WHITE);
        drawString(60,50+100*i,p.nom,BLACK,10*(1+jouer_sur_mac));
        drawString(largeurTerrain*largeurTile-180,47+100*i,"LV "+intToString(p.niveau),BLACK,6*(1+jouer_sur_mac),0,false,true);
        drawRect(60,60+100*i,largeurTerrain*largeurTile-210,17,BLACK);
        Color pv1(10,210,70);
        if(float(p.vie)/p.vie_max<0.25)
            pv1 = Color(200,0,0);
        else if (float(p.vie)/p.vie_max<0.5)
            pv1 = Color(255,150,0);
        fillRect(62,62+100*i,((largeurTerrain*largeurTile-213)*p.vie)/p.vie_max,14,pv1);
        drawString(65,73+100*i,intToString(p.vie)+"/"+intToString(p.vie_max),WHITE,5*(1+jouer_sur_mac));
    }
    for(int i=0;i<h.pokemons.size();i++)
        drawLine(0,110+100*i,largeurTerrain*largeurTile,110+100*i,BLACK);

    // sélection

    // sélecteur
    fillRect(20,55,6*(1+jouer_sur_mac),6*(1+jouer_sur_mac),BLACK);

    int clav=0;
    int selecteur = 0;

    while(clav!='a') {
        clav=Clavier();

        if(clav>=16777234) {
            // Effacement ancien selecteur
            fillRect(20,55+100*selecteur,6*(1+jouer_sur_mac),6*(1+jouer_sur_mac),WHITE);

            // Déplacement touches directionnelles
            clav = (clav - 16777234)%4;
            if (clav==1) {selecteur=(selecteur+nmax-1)%nmax; while(h.pokemons[selecteur].vie == 0) selecteur=(selecteur+nmax-1)%nmax;}
            else if (clav==3) {selecteur=(selecteur+1)%nmax; while(h.pokemons[selecteur].vie == 0) selecteur=(selecteur+1)%nmax;}

            // Affichage nouveau selecteur
            fillRect(20,55+100*selecteur,6*(1+jouer_sur_mac),6*(1+jouer_sur_mac),BLACK);
            flushEvents();
        }

        milliSleep(50);
    }

    return selecteur;
}

// Animations

void animation_vie(Pokemon a,int ancienne_vie){
    Color pv(10,210,70);
    for(int vie=ancienne_vie;vie>a.vie;vie--){
        milliSleep(20); // temps de refresh
        if(float(vie)/a.vie_max<0.25)
            pv = Color(200,0,0);
        else if (float(vie)/a.vie_max<0.5)
            pv = Color(255,150,0);
        if (a.position){
            fillRect(largeurTerrain*largeurTile-196,largeurTerrain*largeurTile-197,(172*min((vie+1),a.vie_max))/a.vie_max,4,WHITE);
            fillRect(largeurTerrain*largeurTile-196,largeurTerrain*largeurTile-197,(172*vie)/a.vie_max,4,pv);
        }
        else {
            fillRect(54,63,(172*min((vie+1),a.vie_max))/a.vie_max,4,WHITE);
            fillRect(54,63,(172*vie)/a.vie_max,4,pv);
        }
    }
}

void secousse(Pokemon a, int aX, int aY, float coefX, float coefY, int refresh) {
    for(int t=0;t<4;t++) {
        (a.position) ? a.drawBack(aX+t*coefX, aY-t*coefY) : a.drawFront(aX+t*coefX, aY-t*coefY);
        milliSleep(refresh);
        fillRect(aX+t*coefX,aY-t*coefY,200,200,WHITE);
    }
    for(int t=4;t>0;t--) {
        (a.position) ? a.drawBack(aX+t*coefX, aY-t*coefY) : a.drawFront(aX+t*coefX, aY-t*coefY);
        milliSleep(refresh);
        fillRect(aX+t*coefX,aY-t*coefY,200,200,WHITE);
    }
    (a.position) ? a.drawBack(aX,aY) : a.drawFront(aX,aY);
}

void clignotement(Pokemon a, int aX, int aY, int n, int refresh) {
    for(int t=0;t<2*n;t++) {
        if (a.position)
            (t%2==0) ? fillRect(aX,aY,200,200,WHITE) : a.drawBack(aX,aY);
        else
            (t%2==0) ? fillRect(aX,aY,250,250,WHITE) : a.drawFront(aX,aY);
        milliSleep(refresh);
    }
}

void animation(int i, Pokemon& a, Pokemon& b,bool jouer_sur_mac) {
    // a est le pokemon attaquant et b le pokemon defenseur, ne pas confondre attaquant,defenseur / gentil,mechant
    // rappel : a.position indique si le pokemon est au premier plan ou non (= gentil ou non)
    // Constante à régler plus tard en fonction du positionnement

    // Coordonnes du point en haut a gauche du pokemon
    int gX = 20; // coordonnes du pokemon gentil
    int gY = largeurTerrain*largeurTile-330;
    int mX = largeurTerrain*largeurTile-250; // coordonnes du pokemon mechant
    int mY = 50;

    // i le sélecteur de l'animation

    if (i==0) {
        // CHARGE
        (a.position) ? secousse(a,gX,gY,20.0,10.0) : secousse(a,mX,mY,-20.0,-10.0);
        // pause de propagation
        milliSleep(300);
        (a.position) ? clignotement(b,mX,mY,2) : clignotement(b,gX,gY,2);
    }
    else if (i==1) {
        // TREMPETTE
        for (int k=0;k<2;k++)
            (a.position) ? secousse(a,gX,gY,0,-10) : secousse(a,mX,mY,0,-10);
    }
    else if (i==2) {
        // RUGISSEMENT

    }
    else if (i==3) {
        // MIMI-QUEUE
        for (int k=0;k<2;k++)
            (a.position) ? secousse(a,gX,gY,10,0) : secousse(a,mX,mY,-10,0);
    }
    else if (i==4){
        // ECLAIR
        (a.position) ? clignotement(b,mX,mY,1) : clignotement(b,gX,gY,1);

        milliSleep(200);
        for(int t=0;t<5;t++) {
            (a.position) ? fillCircle(mX+100,mY+100,t*30,BLACK) : fillCircle(gX+100,gY+120,t*30,BLACK);
            milliSleep(100);
        }
        (a.position) ? fillRect(mX-50,mY-50,250,250,WHITE) : fillRect(gX-50,gY-50,300,300,WHITE);
        (a.position) ? clignotement(b,mX,mY,2) : clignotement(b,gX,gY,2);
    }
    else if (i==5){
        // FATAL-FOUDRE
        (a.position) ? clignotement(b,mX,mY,1) : clignotement(b,gX,gY,1);
        milliSleep(200);
        clignote_dididi();
        for(int t=0;t<5;t++) {
            (a.position) ? fillCircle(mX+100,mY+100,t*30,BLACK) : fillCircle(gX+100,gY+120,t*30,BLACK);
            milliSleep(100);
        }
        (a.position) ? fillRect(mX-50,mY-50,250,250,WHITE) : fillRect(gX-50,gY-50,300,300,WHITE);
        (a.position) ? clignotement(b,mX,mY,2) : clignotement(b,gX,gY,2);
    }
    else if (i==6){
        // CAGE-ECLAIR
        (a.position) ? clignotement(b,mX,mY,1) : clignotement(b,gX,gY,1);
        milliSleep(200);
        for(int t=5;t>0;t--) {
            (a.position) ? drawCircle(mX+100,mY+100,t*30,BLACK,5) : drawCircle(gX+100,gY+120,t*30,BLACK,5);
            milliSleep(100);
            affiche_stat(a,b,jouer_sur_mac);
        }
    }
    else if (i==7){
        // ARMURE
        milliSleep(200);
        for(int t=0;t<5;t++) {
            (a.position) ? drawRect(gX+30+t*15,gY+75+t*15,(5-t)*30,(5-t)*30,BLACK,5) : drawRect(mX+5+t*15,mY+5+t*15,(5-t)*30,(5-t)*30,BLACK,5);
            milliSleep(100);
            affiche_stat(a,b,jouer_sur_mac);
        }
    }
    else if (i==8){
        // ULTRALASER
        (a.position) ? clignotement(b,mX,mY,1) : clignotement(b,gX,gY,1);
        milliSleep(200);
        for (int t=0;t<50;t++){
            Color col(255*(t%3>0),255*((t+1)%3>0),255*((t+2)%3>0));
            (a.position)? drawCircle(gX+150+6*t,gY-5*t+80,15+rand()%5,col,2) : drawCircle(mX-6*t,mY+100+5*t,15+rand()%5,col,2);
            milliSleep(7);
            if (t%5==0) clignote_dididi(80,40);
            noRefreshBegin();
            (a.position)? fillCircle(gX+150+6*t,gY-5*t+80,22,WHITE) : fillCircle(mX-6*t,mY+100+5*t,22,WHITE);
            for (int i=0;i<t;i++)
                (a.position)? fillCircle(gX+150+6*i,gY-5*i+80,10,col) : fillCircle(mX-6*i,mY+100+5*i,10,col);
            noRefreshEnd();
        }
        (a.position)? fillCircle(gX+150+6*50,gY-5*50+80,20,BLACK) : fillCircle(mX-6*50,mY+100+5*50,20,BLACK);
        clignote_dididi(200);
        affiche_stat(a,b,jouer_sur_mac);
        milliSleep(100);
        (a.position) ? clignotement(b,mX,mY,2) : clignotement(b,gX,gY,2);
    }
    else if (i==9){
        // COUPE
        (a.position) ? clignotement(b,mX,mY,1) : clignotement(b,gX,gY,1);
        milliSleep(200);
        int ggX[5] = {gX+170,gX+190,gX+155};
        int ggY[5] = {gY+50,gY+70,gY+85};
        int mmX[5] = {mX+130,mX+140,mX+110};
        int mmY[5] = {mY,mY+10,mY+30};
        for (int t=0;t<15;t++){
            for(int j=2;j<5;j++){
                ggX[j] -= 12;
                ggY[j] += 12;
                mmX[j] -= 10;
                mmY[j] += 10;
            }
            milliSleep(2);
            noRefreshBegin();
            (a.position)? drawPoly(mmX,mmY,3,BLACK,5) : drawPoly(ggX,ggY,3,BLACK,5);
            (a.position)? fillPoly(mmX,mmY,3,WHITE) : fillPoly(ggX,ggY,3,WHITE);
            noRefreshEnd();
        }
        affiche_stat(a,b,jouer_sur_mac);
        milliSleep(100);
        (a.position) ? clignotement(b,mX,mY,2) : clignotement(b,gX,gY,2);
    }
}

void gigote_pokeball(Image<AlphaColor> pookieballs[],int x0,int y0){
    int positions[4] = {x0-4,x0,x0+4,x0};
    int indices[4] = {1,0,2,0};
    for (int i=0;i<4;i++){
        milliSleep(130);
        noRefreshBegin();
        fillRect(largeurTerrain*largeurTile-200,50,150,200,WHITE);
        display(pookieballs[indices[i]],positions[i],y0);
        noRefreshEnd();
    }
}

bool Capture_pokemon(Image<AlphaColor> pookieballs[],Image<AlphaColor> explosions[4],Image<AlphaColor> imgMenu,
                            Pokemon &a, Pokemon &b, bool jouer_sur_mac){
    int gX = 20; // coordonnes du pokemon gentil
    int gY = largeurTerrain*largeurTile-330;
    int mX = largeurTerrain*largeurTile-250; // coordonnes du pokemon mechant
    int mY = 50;

    int x1 = mX+15;
    int y1 = mY+50;
    int y=0;
    int x;
    for (x=0;x<x1+x1/40;x += x1/40){ // lancer de la pokeball
        milliSleep(7);
        noRefreshBegin();
        fillRect(mX,mY,300,200,WHITE);
        affiche_stat(a,b,jouer_sur_mac);
        y = trajet_pkball(x,0,x1,(largeurTerrain*largeurTile*2)/3,y1,75);
        if (x<2*x1/3)
            display(pookieballs[1],x,y);
        else
            display(pookieballs[2],x,y);
        noRefreshEnd();
    }
    x-=x1/40;
    int x0=x,y0=y;
    int yy = y;
    int positions[4]={x0,x0-4,x0,x0+4};
    int indices[4]={0,1,0,2};
    for(int ymax=y0-176;ymax<(y0-10);ymax = y0 - (1*(y0-ymax))/3 ){
        int V0;
        yy=y0;
        for (int t=0;t<100000 && yy<=y0;t+=1000){
            V0 = sqrt(20*(y0-ymax));
            yy = 5*t*t/(1000*1000)-(V0*t)/1000+y0;
            milliSleep(30);
            noRefreshBegin();
            fillRect(x-x1/20,y,128,128,WHITE);
            affiche_stat(a,b,jouer_sur_mac);
            display(pookieballs[indices[(t/5000)%4]],positions[(t/5000)%4],yy);
            noRefreshEnd();
        }
    }

    // ouverture de la pokeball
    noRefreshBegin();
    fillRect(x,y,128,64,WHITE);
    affiche_stat(a,b,jouer_sur_mac);
    display(pookieballs[4],x,y);
    noRefreshEnd();
    milliSleep(200);
    for (y=y1-25;y>y1-176;y -= 30){
        milliSleep(10);
        noRefreshBegin();
        fillRect(mX,mY,200,200,WHITE);
        affiche_stat(a,b,jouer_sur_mac);
        display(pookieballs[4],x0,y0);
        display(pookieballs[3],x,y);
        noRefreshEnd();
    }
    y+=30;
    for(int i=0;i<4;i++){ // Explosions
        milliSleep(40);
        noRefreshBegin();
        fillRect(mX,mY,200,200,WHITE);
        affiche_stat(a,b,jouer_sur_mac);
        display(pookieballs[4],x0,y0);
        display(pookieballs[3],x,y);
        display(explosions[i],x0-64,y0-64-20);
        noRefreshEnd();
    }
    for (;y<y0;y += 30){ // fermeture de la pokeball
        milliSleep(15);
        noRefreshBegin();
        affiche_stat(a,b,jouer_sur_mac);
        fillRect(mX,mY,200,200,WHITE);
        display(pookieballs[4],x0,y0);
        display(pookieballs[3],x,y);
        noRefreshEnd();
    }
    noRefreshBegin();
    fillRect(mX,mY,200,200,WHITE);
    display(pookieballs[0],x0,y0);
    noRefreshEnd();
    double a_capture = ((3*b.vie_max-2*b.vie)*b.taux*1*(2+(b.etat != "normal")+(b.etat == "endormi")))/(6*b.vie_max);
    int b_capture = 65535*sqrt(sqrt(a_capture/255));
    int chance1 = int(65535*(float(rand())/RAND_MAX));
    int chance2 = int(65535*(float(rand())/RAND_MAX));
    int chance3 = int(65535*(float(rand())/RAND_MAX));
    int chance4 = int(65535*(float(rand())/RAND_MAX));
    int nbre_capture = (chance1 < b_capture)+(chance2 < b_capture)
                        +(chance3 < b_capture)+(chance4 < b_capture);
    if (nbre_capture > 0){
        milliSleep(800);
        gigote_pokeball(pookieballs,x0,y0);
        if (nbre_capture > 1){
            milliSleep(800);
            gigote_pokeball(pookieballs,x0,y0);
            if (nbre_capture > 2){
                milliSleep(800);
                gigote_pokeball(pookieballs,x0,y0);
                if (nbre_capture > 3){
                    milliSleep(800);
                    display(pookieballs[5],x0,y0);
                    milliSleep(800);
                    affiche_menu("Et hop! " +b.nom+ " est attrapé!", 700, imgMenu, jouer_sur_mac);
                    return true;
                }
            }
        }
    }
    milliSleep(760);
    for(int i=0;i<4;i++){ // Explosions
        milliSleep(40);
        noRefreshBegin();
        fillRect(mX,mY,200,200,WHITE);
        affiche_stat(a,b,jouer_sur_mac);
        display(explosions[i],x0-64,y0-64-20);
        noRefreshEnd();
    }
    noRefreshBegin();
    fillRect(mX,mY,200,200,WHITE);
    affiche_stat(a,b,jouer_sur_mac);
    noRefreshEnd();
    if(nbre_capture==0) affiche_menu("Oh non! Le POKéMON s'est libéré!", 700, imgMenu,jouer_sur_mac);
    if(nbre_capture==1) affiche_menu("Raaah! Ca y était presque!", 700, imgMenu,jouer_sur_mac);
    if(nbre_capture==2) affiche_menu("Aaaaaah! Presque!", 700, imgMenu,jouer_sur_mac);
    if(nbre_capture==3) affiche_menu("Mince! Ca y était presque!", 700, imgMenu,jouer_sur_mac);
    return false;
}

bool attaque(Pokemon &attaquant,Pokemon &defenseur, int id_attaque, Image<AlphaColor> imgMenu, bool jouer_sur_mac){
    Attaque attaque = liste_attaques[id_attaque];
    int jet_malchance = (rand()%100)+1;

    // EFFETS ENDORMI ET PARALYSE
    if(attaquant.etat == "endormi"){
        if(jet_malchance>50){
            affiche_menu(attaquant.nom+" dort",1000,imgMenu,jouer_sur_mac);
            return false;
        }
        affiche_menu(attaquant.nom+" se réveille",1000,imgMenu,jouer_sur_mac);
        defenseur.etat = "normal";
    }

    if(attaquant.etat == "paralyse"){
        if(jet_malchance>75){
            affiche_menu(attaquant.nom+" est paralysé et ne peut pas attaquer",1000,imgMenu,jouer_sur_mac);
            return false;
        }
    }

    // ECHEC DE L'ATTAQUE
    jet_malchance = (rand()%100)+1;
    if (jet_malchance > attaque.chance){ // L'attaque a attaque.chance % de chance de reussir
        affiche_menu("... mais échoue",1000,imgMenu,jouer_sur_mac);
        return false;
    }

    // ANIMATION
    animation(attaque.id,attaquant,defenseur,jouer_sur_mac);

    // CALCUL DES DEGATS
    int ancienne_vie = defenseur.vie;
    int efficace=0;      // 1 si l'attaque est super efficace, 2 si elle est 2 fois super efficace,etc...
    int pas_efficace=0;  // 1 si l'attaque n'est pas tres efficace, 2 si elle est 2 fois pas tres efficace,etc...

    if (attaque.degats != 0){
        int degats = ((attaquant.niveau*0.4+2)*attaquant.att*attaque.degats)/(50*defenseur.def)+2; // degats de l'attaque en prenant en compte les stats des pokemons
        if(attaquant.type == attaque.type)    // ex: un pokemon de type feu qui fait une attaque feu de
            degats = (3*degats)/2; // 40 degats fera en fait 60 degats (comme dans le vrai pokemon)
        for(int i=0;i<defenseur.type_super_eff.size();i++){
            efficace += defenseur.type_super_eff[i] == attaque.type;
        }
        for(int i=0;i<defenseur.type_pas_eff.size();i++){
            pas_efficace += defenseur.type_pas_eff[i] == attaque.type;
        }
        degats = puissance(2,efficace-pas_efficace)*degats;
        defenseur.vie = max(0,defenseur.vie-degats);
    }
    
    // AFFICHAGE DE LA VIE + EFFICACITE
    animation_vie(defenseur,ancienne_vie);
    affiche_stat(attaquant,defenseur,jouer_sur_mac);
    if(efficace-pas_efficace>0)
        affiche_menu("C'est très efficace !",1000,imgMenu,jouer_sur_mac);
    else if (efficace-pas_efficace<0)
        affiche_menu("Ce n'est pas très efficace !",1000,imgMenu,jouer_sur_mac);

    // ALTERATION DES STATS
    defenseur.att = max(1,(defenseur.att * attaque.coef_att_adv)/100);
    defenseur.def = max(1,(defenseur.def * attaque.coef_def_adv)/100);
    defenseur.vit = (defenseur.vit * attaque.coef_vit_adv)/100;
    attaquant.att = max(1,(attaquant.att * attaque.coef_att)/100);
    attaquant.def = max(1,(attaquant.def * attaque.coef_def)/100);
    attaquant.vit = (attaquant.vit * attaque.coef_vit)/100;

    // ALTERATION DE L'ETAT
    if (defenseur.etat == "normal"){
        jet_malchance = (rand()%100)+1;
        if (jet_malchance < attaque.coef_empoisonne){
            defenseur.etat = "empoisonne";
            affiche_menu(defenseur.nom+" est empoisonné",1000,imgMenu,jouer_sur_mac);
        }
        else if (jet_malchance < attaque.coef_endormi){
            defenseur.etat = "endormi";
            affiche_menu(defenseur.nom+" s'endort",1000,imgMenu,jouer_sur_mac);
        }
        else if (jet_malchance < attaque.coef_paralyse){
            defenseur.etat = "paralyse";
            defenseur.vit = defenseur.vit/4;
            affiche_menu(defenseur.nom+" est paralysé",1000,imgMenu,jouer_sur_mac);
        }
    }
    return true;
}
