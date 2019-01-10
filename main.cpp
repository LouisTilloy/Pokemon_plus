#include <fstream>
#include <sstream>
#include <iostream>
#include "combat.h"

// LEGENDE : les commentaires précédés de --> désigne des fonctions à implémenter
// C'est comme cela que nous nous sommes organisé pour savoir quoi faire.

const int CARTE=0;
const int COMBAT=1;
const int FINANCE=2;
const int TITRE=3;

// pas de l'animation changement de terrain
const int etapesAnimation=30;

int main() {
    openWindow(largeurTerrain*largeurTile, largeurTerrain*largeurTile);
    bool jouer_sur_mac = true; // initialisation variable compatibilité

    int DISPLAY=TITRE;

    InitRandom(); // initialisation de la graine aléatoire

    // ----------------------- INITIALISATION (chargement images + textes + création d'objets)

    // ------------ INITIALISATION ECRAN TITRE
    NativeBitmap titreImg = initialiserImg("titre");


    // ------------ INITIALISATION CARTE
    int terrainActuel=0; // variable désignant le terrain à afficher parmi tous les .txt

    // Table d'images de tiles
    NativeBitmap tableImg[nTile]; // tableau d'images bitmap des cases pour générer les terrains
    for(int i=0;i<nTile;i++) {tableImg[i]=initialiserImg(intToString(i));}
    Image<AlphaColor> tableAlphaImg[nAlphaTile]; // tableau de cases transparentes pour premier plan d'affichage (herbes, etc.)
    load(tableAlphaImg[0], srcPath("img/2_alpha.png")); // à généraliser, pour l'instant il n'y en a besoin que d'une

    // Lecture des fichier texte et génération des objets carte
    carte terrain[largeurCarte]; // tableau d'objets
    for(int i=0;i<largeurCarte;i++) {
        cout << "Ouverture du fichier " << i << ".txt : ";
        string nomFil = srcPath("carte/")+intToString(i)+".txt";
        ifstream fil(nomFil.c_str());
        if(fil) {
            cout << "1\n"; // ouverture ok
            string line;
            for(int k=0;getline(fil,line);k++) {
                for(int j=0;j<largeurTerrain;j++) {terrain[i].t[k][j] = line[j]-'0';} // conversion anscii -> int
            }
            fil.close();
        }
        else {cout << "0\n";}
    }

    // Capture des rendus globaux de terrain pour accelérer l'affichage
    for (int i=0;i<largeurCarte;i++) {
        affiche(terrain[i], tableImg); // second-plan
        affiche(terrain[i],tableAlphaImg); // premier-plan
        int w,h;
        byte *rCapture,*gCapture,*bCapture;
        captureWindow(rCapture,gCapture,bCapture,w,h);
        terrain[i].img = NativeBitmap(w,h);
        terrain[i].img.setColorImage(0,0,rCapture,gCapture,bCapture,w,h);
    }


    // ------------ INITIALISATION MENU
    Image<AlphaColor> imgMenu;
    load(imgMenu, srcPath("img/panneau.png"));
    Image<AlphaColor> imgMenuListe;
    load(imgMenuListe, srcPath("img/panneau2.png"));
    
    
    //-------------- INITIALISATION POOKIEBALL + EXPLOSIONS
    const int nPookieball = 6;
    Image<AlphaColor> ImagesPookieball[nPookieball];
    for (int i=0;i<nPookieball;i++)
         load(ImagesPookieball[i],srcPath("img/pokeball"+intToString(i)+".png"));
    const int nExplosions = 4;
    Image<AlphaColor> ImagesExplosions[nExplosions];
    for (int i=0;i<nExplosions;i++)
        load(ImagesExplosions[i],srcPath("img/explosion"+intToString(i)+".png"));


    // ------------ INITIALISATION POKEMONS
    vector<Pokemon> liste_pokemons;

    // lecture d'un fichier texte contenant les infos sur les pokemons
    string nomFichierBDD = srcPath("carte/bdd.txt");
    ifstream bddPokemon(nomFichierBDD.c_str());

    if(bddPokemon) {
        string line;
        // Remplissage du vector tant qu'on peut extraire des lignes
        while(getline(bddPokemon,line)) {
            stringstream lineSS(line);
            // Lecture des caractéristiques principales
            string b0;
            string b1;
            string nom0;
            int id0;
            string type0;
            int vie_max0;
            int att_normal0;
            int def_normal0;
            int vit_normal0;
            int attaques0[nbre_attaques][2];
            int niveau0;
            int bonus_vie0;
            int bonus_att0;
            int bonus_def0;
            int bonus_vit0;
            vector<string> type_super_eff0;
            vector<string> type_pas_eff0;
            string vitesse_croissance0;
            int taux0;
            lineSS >> b0 >> b1 >> nom0 >> id0 >> type0 >> vie_max0 >> att_normal0 >> def_normal0 >> vit_normal0 >> niveau0 >> bonus_vie0 >> bonus_att0 >> bonus_def0 >> bonus_vit0 >> vitesse_croissance0 >> taux0;

            // Attaques
            getline(bddPokemon,line);
            stringstream lineAtt(line);
            for (int i=0;i<nbre_attaques;i++)
                lineAtt >> attaques0[i][0] >> attaques0[i][1];

            // Type pas eff
            getline(bddPokemon,line);
            stringstream lineTypePasEff(line);
            string attaques0TmpStr;
            while(getline(lineTypePasEff,attaques0TmpStr,' '))
                type_pas_eff0.push_back(attaques0TmpStr);

            // Type eff
            getline(bddPokemon,line);
            stringstream lineTypeEff(line);
            while(getline(lineTypeEff,attaques0TmpStr,' '))
                type_super_eff0.push_back(attaques0TmpStr);

            // Appel au constructeur
            liste_pokemons.push_back(Pokemon(initialiserImg(b0), initialiserImg(b1), nom0, id0, type0, vie_max0, att_normal0, def_normal0,
                                             vit_normal0, attaques0, niveau0, bonus_vie0, bonus_att0, bonus_def0, bonus_vit0, type_super_eff0,
                                             type_pas_eff0, vitesse_croissance0, taux0));
        }
        bddPokemon.close();
    }


    // ------------ INITIALISATION HEROS
    heros sacha;
    for(int i=0;i<8;i++) {load(sacha.img[i],srcPath("img/ash"+intToString(i)+".png"));}
    sacha.nom = "Sacha";

    // des objets et statistiques évoluées de bases pour la démo, en principe on commence avec rien
    sacha.pokemons.push_back(liste_pokemons[0]);
    sacha.pokemons.push_back(liste_pokemons[0]);
    sacha.pokemons[0].SetNiveau(25);
    sacha.pokemons[1].SetNiveau(10);
    sacha.objets.add("Pookieball",10);
    sacha.objets.add("Kro",3);
    sacha.objets.add("Straffe",1);
    sacha.objets.add("Cidre",12);

    // ------------ INITIALISATION PNJ (personnages non-joueurs)
    // À généraliser avec lecture d'un fichier texte pour les versions future
    vector <heros> liste_pnj;

    heros OLIVIA;
    OLIVIA.nom = "Olivia";
    for(int i=0;i<8;i++) {load(OLIVIA.img[i],srcPath("img/ash"+intToString(i)+".png"));}
    load(OLIVIA.img[3],srcPath("img/olivia3.png"));
    OLIVIA.dialogue.push_back("J'aime la nature");
    OLIVIA.dialogue.push_back("Et toi ?");
    OLIVIA.dialogue.push_back("...");
    OLIVIA.dialogue.push_back("On dirait que ce jeu");
    OLIVIA.dialogue.push_back("n'implémente pas de réponses...");
    OLIVIA.x = 17;
    OLIVIA.y = 3;
    liste_pnj.push_back(OLIVIA);

    heros JON;
    JON.nom = "Jon";
    for(int i=0;i<8;i++) {load(JON.img[i],srcPath("img/ash"+intToString(i)+".png"));}
    load(JON.img[3],srcPath("img/jon3.png"));
    JON.dialogue.push_back("Je protège ce panneau");
    JON.dialogue.push_back("...");
    JON.dialogue.push_back("Ça valait pas la peine de venir.");
    JON.x = 15;
    JON.y = 17;
    liste_pnj.push_back(JON);

    heros PAT;
    PAT.nom = "Pat";
    for(int i=0;i<8;i++) {load(PAT.img[i],srcPath("img/ash"+intToString(i)+".png"));}
    load(PAT.img[3],srcPath("img/pat3.png"));
    PAT.dialogue.push_back("Hihihi");
    PAT.x = 5;
    PAT.y = 12;
    liste_pnj.push_back(PAT);

    terrain[0].listePnj.push_back(0); // ajout de OLIVIA au premier terrain, elle est repérée par son id 0
    terrain[1].listePnj.push_back(1);
    terrain[3].listePnj.push_back(2);


    // ----------------------- BOUCLE DE JEU
    terrain[terrainActuel].affiche();
    bool play=true;
    bool rencontrePokemonSauvage=true;


    while(play) {
        if (DISPLAY==CARTE) {
            // ------------------------------------------- DEPLACEMENT DANS LA CARTE
            while(DISPLAY==CARTE) {
                // Affichage
                noRefreshBegin();
                terrain[terrainActuel].affiche(); // Affichage du fond statique du terrain
                sacha.affiche(); // Affichage sacha
                for (int k=0; k < terrain[terrainActuel].listePnj.size() ; k++)
                    liste_pnj[terrain[terrainActuel].listePnj[k]].affiche(); // Affichage pnj
                affiche_tile(terrain[terrainActuel], tableAlphaImg, sacha.x, sacha.y); // Affichage éléments transparents
                noRefreshEnd();

                // Gestion du clavier
                int clav = Clavier();
                if(clav>=16777234) {
                    // Déplacement touches directionnelles
                    clav = (clav - 16777234)%4;
                    sacha.sens = clav; // on oriente le regard du personnage vers la direction pointée quoiqu'il arrive

                    // Collision avec les autres personnages
                    bool nonCollisionPnj = true;
                    for (int k=0; k < terrain[terrainActuel].listePnj.size() ; k++)
                        nonCollisionPnj = nonCollisionPnj && !(sacha.collision(liste_pnj[terrain[terrainActuel].listePnj[k]]));

                    if (!sacha.collision(terrain[terrainActuel].t) && nonCollisionPnj) {
                        sacha.avancer(clav, terrain[terrainActuel], tableImg, tableAlphaImg);

                        // gestion de l'apparition de pokémon dans les hautes-herbes
                        if(terrain[terrainActuel].t[sacha.y][sacha.x]==2) {
                            int r=rand()%6; // lancé de dé et apparition pokémons
                            if(r==0) {
                                // Fondu au noir et passage à l'écran de combat
                                didididididi(); // animation
                                fillRect(0,0,largeurTerrain*largeurTile,largeurTerrain*largeurTile, WHITE);
                                DISPLAY=COMBAT;
                            }
                        }
                    }
                }
                else if(clav=='a') {
                    // Touche A => Actions

                    // interactions avec le terrain
                    if(terrain[terrainActuel].t[sacha.nextY()][sacha.nextX()]==3) {
                        // Lecture d'un panneau -----> relier à des textes...
                        affiche_menu("ATTENTION AUX HAUTES HERBES !", imgMenu,jouer_sur_mac);
                    }
                    // ------> else if ... pour les autres types d'éléments du terrain

                    // interactions avec les personnages
                    for (int k=0; k < terrain[terrainActuel].listePnj.size() ; k++)
                        if (sacha.collision(liste_pnj[terrain[terrainActuel].listePnj[k]]))
                            for (int l=0;l<liste_pnj[terrain[terrainActuel].listePnj[k]].dialogue.size();l++)
                                affiche_menu(liste_pnj[terrain[terrainActuel].listePnj[k]].dialogue[l],imgMenu,jouer_sur_mac);

                    // -----------> else if, récupérer objet, etc., activer interrupteur
                }


                // Changement de terrain si le héros sors de l'écran
                if(sacha.x>=largeurTerrain) {
                    // personnage trop à droite
                    sacha.x=0;
                    // animation slide
                    for(int t=0;t<etapesAnimation;t++) {
                        noRefreshBegin();
                        terrain[terrainActuel].affiche(-largeurTile*largeurTerrain*t/etapesAnimation);
                        terrain[terrainActuel+1].affiche(largeurTile*largeurTerrain*(etapesAnimation-t)/etapesAnimation);
                        sacha.affiche(largeurTile*largeurTerrain*(etapesAnimation-t)/etapesAnimation);
                        noRefreshEnd();
                        milliSleep(7);
                    }
                    terrainActuel++;
                }
                else if(sacha.x<0) {
                    // personnage trop à gauche
                    sacha.x = largeurTerrain-1;
                    // animation slide
                    for(int t=0;t<etapesAnimation;t++) {
                        noRefreshBegin();
                        terrain[terrainActuel].affiche(largeurTile*largeurTerrain*t/etapesAnimation);
                        terrain[terrainActuel-1].affiche(-largeurTile*largeurTerrain*(etapesAnimation-t)/etapesAnimation);
                        sacha.affiche(largeurTile*largeurTerrain*t/etapesAnimation-largeurTile);
                        noRefreshEnd();
                        milliSleep(7);
                    }
                    terrainActuel--;
                }

                // Fréquence d'affichage : 20 images par secondes
                milliSleep(50);
            }

        }
        else if(DISPLAY==COMBAT) {
            // --------------------------------------------------- COMBAT POKEMON

            // Quelques variables locales utiles au combat
            int indice_sacha = sacha.indice_premier(); // selecteur pokemon actuel
            int indice_dresseur = 0;
            Pokemon gentil = sacha.pokemons[indice_sacha];
            gentil.position = true; // gentil est au premier plan
            Pokemon mechant;
            mechant.position = false; // mechant est au second plan
            int nbre_pokemons_sacha = sacha.pokemons.size() - indice_sacha; // nb pokemon total
            int nbre_pokemons_dresseur=1; // par défaut, pour pokémon sauvage

            // ---> pour la démo, toujours le même combat (MAGICARPE niveau 30), ensuite, il faudra une liste des pokémons qui apparaissent par zones
            heros dresseur = sacha;
            //Pokemon Sauvage = liste_pokemons[rand()%nbre_pokemons];
            Pokemon Sauvage = liste_pokemons[1];
            Sauvage.SetNiveau(50);

            if (rencontrePokemonSauvage){
                // pokémon sauvage
                mechant = Sauvage;
                nbre_pokemons_dresseur = 1;
                affiche_menu("Un "+mechant.nom+" sauvage apparaît !",imgMenu,jouer_sur_mac);
            }
            else{
                // dresseur adversaire, possède plusieurs pokémons
                nbre_pokemons_dresseur = dresseur.pokemons.size();
                mechant = dresseur.pokemons[indice_dresseur];
                affiche_menu(dresseur.nom+" veut se battre !",imgMenu,jouer_sur_mac);
            }

            bool capture = false; // phase de capture du pokémon

            while(nbre_pokemons_sacha > 0 && nbre_pokemons_dresseur > 0 && !capture){
                // Affichage vie, nom, etc. des deux adversaires
                affiche_stat(gentil,mechant,jouer_sur_mac);

                // Affichage menu de choix principal
                int choix = affiche_menu("ATTAQUE","POOKIEMON","SACOCHE","FUITE",imgMenu,jouer_sur_mac);

                int choix_attaque_gentil;
                int choix_attaque_mechant;
                // ----------------> rajouter mémoire de la dernière attaque

                if (choix==0)
                    choix_attaque_gentil = affiche_menu(gentil.attaques, gentil.niveau,imgMenuListe,jouer_sur_mac);
                else if(choix==1) {
                    // CHOIX POOKIEMON
                    gentil.Initialisation(); // sortir le pokémon du combat et réinitialiser ses variables de combat
                    sacha.pokemons[indice_sacha] = gentil;
                    indice_sacha = choix_pokemon(sacha,jouer_sur_mac);
                    // -------------> ANIMATION CHANGEMENT POOKIEMON (ouverture de la pokeball)
                    gentil = sacha.pokemons[indice_sacha];
                    gentil.position = true;
                    affiche_stat(gentil,mechant,jouer_sur_mac);
                    affiche_menu("Go "+gentil.nom+" !",1000,imgMenu,jouer_sur_mac);
                }
                else if(choix==3) {
                    // FUITE
                    string textFuite[] = {"Vous fuyez comme un lâche !",
                                          "Sérieusement ?",
                                          "Vous fuyez, c'est pas très pro..."};
                    // ------------> RAJOUTER UNE PROBA DE REUSSITE, pour la démo, on peut fuir tout le temps
                    affiche_menu(textFuite[rand()%3],1000,imgMenu,jouer_sur_mac);
                    milliSleep(500); // --------------------------> animation foudu blanc
                    gentil.Initialisation(); // sortir le pokémon du combat
                    sacha.pokemons[indice_sacha] = gentil;
                    DISPLAY=CARTE;
                    choix = 4;
                    break;
                }

                // Choix d'attaque de l'adversaire
                // ---------> IA un peu plus performante ?
                choix_attaque_mechant = rand()%(mechant.NombreAttaques());

                // Attaque
                if (gentil.vit >= mechant.vit || choix!=0){
                    // tour du joueur
                    if (choix==0){
                        // ATTAQUE
                        affiche_menu(gentil.nom+" attaque "+liste_attaques[gentil.attaques[choix_attaque_gentil][1]].nom, 500, imgMenu,jouer_sur_mac);
                        affiche_stat(gentil,mechant,jouer_sur_mac);
                        attaque(gentil,mechant,gentil.attaques[choix_attaque_gentil][1],imgMenu,jouer_sur_mac);
                    }
                    else if (choix==2) {
                        // SACOCHE
                        choix = affiche_menu(sacha.objets,imgMenuListe,jouer_sur_mac);
                        if (sacha.objets[choix].disponible()){
                            affiche_menu(sacha.nom + " utilise "+sacha.objets[choix].nom,500,imgMenu,jouer_sur_mac);

                            if (sacha.objets[choix].isBall) {
                                if(rencontrePokemonSauvage){
                                    // Pookieball
                                    sacha.objets[choix].utiliser(); // décrémente quantité
                                    capture = Capture_pokemon(ImagesPookieball,ImagesExplosions,imgMenu,gentil,mechant,jouer_sur_mac);
                                }
                                else {
                                        affiche_menu(" Prof chen va te mettre une quenelle si tu fais ça !",700,imgMenu, jouer_sur_mac);
                                    }
                            }
                            else {
                                // ----> autres objets, peuvent servir à soigner, etc.
                            }
                        }
                        else {
                            affiche_menu("Vous ne disposez plus de "+sacha.objets[choix].nom,800,imgMenu,jouer_sur_mac);
                        }
                    }

                    // tour de l'adverse
                    if (!capture && !mechant.Mort()) {
                        affiche_menu(mechant.nom+" attaque "+liste_attaques[mechant.attaques[choix_attaque_mechant][1]].nom, 500, imgMenu,jouer_sur_mac);
                        affiche_stat(gentil,mechant,jouer_sur_mac);
                        attaque(mechant,gentil,mechant.attaques[choix_attaque_mechant][1],imgMenu,jouer_sur_mac);
                    }
                }
                else{
                    // tour du joueur
                    affiche_menu(mechant.nom+" attaque "+liste_attaques[mechant.attaques[choix_attaque_mechant][1]].nom, 500, imgMenu,jouer_sur_mac);
                    affiche_stat(gentil,mechant,jouer_sur_mac);
                    attaque(mechant,gentil,mechant.attaques[choix_attaque_mechant][1], imgMenu,jouer_sur_mac);
                    // tour de l'adverse
                    if(!gentil.Mort() && choix==0) {
                        affiche_menu(gentil.nom+" attaque "+liste_attaques[gentil.attaques[choix_attaque_gentil][1]].nom, 500, imgMenu,jouer_sur_mac);
                        attaque(gentil,mechant,gentil.attaques[choix_attaque_gentil][1], imgMenu,jouer_sur_mac);
                    }
                }

                // CAPTURE DU POKEMON !
                if (capture){
                    mechant.Initialisation();
                    gentil.Initialisation();
                    sacha.pokemons[indice_sacha] = gentil;
                    if(sacha.pokemons.size()<6) {
                        sacha.pokemons.push_back(mechant);
                        affiche_menu("Son profil est ajouté au Pookiedex™...", 1000, imgMenu, jouer_sur_mac); // lol
                    }
                    else {
                        affiche_menu("Plus de place pour "+mechant.nom+"...", 1000, imgMenu, jouer_sur_mac);
                        // ----------------------> PROPOSER DE TRANSFERER DANS UN VECTOR DE STOCKAGE, ou revendre le pookiemon ?
                    }
                }
                else{
                    // POISON
                    if(gentil.etat == "empoisonne" && !gentil.Mort()){
                        gentil.vie =  max(0,(gentil.vie*100-gentil.vie_max*10)/100);
                        affiche_menu(gentil.nom+" souffre du poison",1000,imgMenu,jouer_sur_mac);
                    }
                    if(mechant.etat == "empoisonne" && !mechant.Mort()){
                        mechant.vie =  max(0,(mechant.vie*100-mechant.vie_max*10)/100);
                        affiche_menu(mechant.nom+" souffre du poison",1000,imgMenu,jouer_sur_mac);
                    }

                    // MORT D'UN DES DEUX POKEMONS + CHOIX DU POKEMON SUIVANT
                    if (mechant.Mort()){
                        affiche_menu(mechant.nom+" est K.O.", 1000, imgMenu,jouer_sur_mac);
                        affiche_menu(gentil.nom+" a gagné "+ intToString(mechant.xp_donne())+" XP", 1000, imgMenu,jouer_sur_mac);
                        gentil.xp += mechant.xp_donne();
                        if(gentil.Monter_niveau()){ // Test si le pokemon monte un niveau
                            affiche_stat(gentil,mechant,jouer_sur_mac);
                            affiche_menu(gentil.nom+" monte au niveau "+intToString(gentil.niveau),imgMenu,jouer_sur_mac);                         
                        }
                        nbre_pokemons_dresseur-- ; // Reactualise le nombre de pokemons vivants
                        indice_dresseur++; // Incremente l'indice du dresseur pour changer son pokemon
                        if(nbre_pokemons_dresseur>0){
                            mechant = dresseur.pokemons[indice_dresseur];
                            mechant.position = false;
                        }
                        else{
                            gentil.Initialisation(); // Regain de stats
                            sacha.pokemons[indice_sacha] = gentil;
                            if (!rencontrePokemonSauvage){
                                affiche_menu(dresseur.nom + " vous donne "+intToString(dresseur.argent)+" bières", 1000, imgMenu,jouer_sur_mac);
                                sacha.argent += dresseur.argent;
                            }
                        }
                    }
                    if (gentil.Mort()){
                        affiche_menu(gentil.nom+" est K.O.", imgMenu,jouer_sur_mac);
                        // -----> appeler un autre pookiemon ou fuir ?
                        gentil.Initialisation();
                        sacha.pokemons[indice_sacha] = gentil;
                        nbre_pokemons_sacha-- ;
                       
                        if(nbre_pokemons_sacha>0){
                            indice_sacha=choix_pokemon(sacha,jouer_sur_mac);
                            // -------------------> ANIMATION CHANGEMENT POOKIEMON
                            affiche_stat(gentil,mechant,jouer_sur_mac);
                            gentil = sacha.pokemons[indice_sacha];
                            gentil.position = true;
                            affiche_menu("Go "+gentil.nom+" !",1000,imgMenu,jouer_sur_mac);
                        }
                        else{
                            // ---> GAME OVER à faire
                            cout<<"PERDU"<<endl;
                        }
                    }
                }
            }
            // Retour à la carte
            DISPLAY=CARTE;

        }
        else if(DISPLAY==FINANCE) {
            // Rien pour le moment...
            /*

              Le but du jeu dans les prochaines versions, est de permettre de revendre les bières gagnées pendant les combats
              dans les maisons que l'on apperçoit sur la carte (dans lesquelles on ne peut pa encore rentrer).

              Les cours de revente évolueront périodiquement, ce qui apportera une dimension d'économie en phase avec l'esprit des Ponts.

              Pour exemple, le système de vente et d'évolution des cours devrait fortement ressembler à celui des jeux de simulation
              comme Frontier, un des premiers jeu de simulation spatiale.

            */
        }
        else if(DISPLAY==TITRE) {
            // --------------------------------------------------- ECRAN DE DEMARRAGE
            putNativeBitmap(0,0,titreImg);

            while (Clavier() != 'a')
                milliSleep(100);

            flushEvents();

            // Grossissement des polices si on joue sur Mac
            string config[] = {"Jouer sur Windows","Jouer sur Mac OS"};
            jouer_sur_mac = affiche_menu(config,2,imgMenuListe,false);

            DISPLAY = CARTE;
        }
    }

    endGraphics();
	return 0;
}
