#include "pokemon.h"

void InitRandom()
{
    srand((unsigned int)time(0));
    rand();
}

Pokemon::Pokemon(){
    type = "normal";
    etat = "normal";
    xp = 0;
    bonus_att = 1;
    residu_bonus_att = 0;
    bonus_def = 1;
    residu_bonus_def = 0;
    bonus_vit = 1;
    residu_bonus_vit = 0;
    bonus_vie = 2;
    residu_bonus_vie = 0;
    niveau = 1;
    vitesse_croissance = "normal";
    taux = 255;
}

Pokemon::Pokemon(NativeBitmap b0, NativeBitmap b1, string nom0, int id0, string type0, int vie_max0, int att_normal0,
                 int def_normal0, int vit_normal0, int attaques0[nbre_attaques][2], int niveau0,
                 int bonus_vie0, int bonus_att0, int bonus_def0, int bonus_vit0
                 , vector<string> &type_super_eff0, vector<string> &type_pas_eff0, string vitesse_croissance0
                 ,int taux0){
    sprite = b0;
    sprite_back = b1;
    nom = nom0;
    id = id0;
    type = type0;
    type_super_eff = type_super_eff0;
    type_pas_eff = type_pas_eff0;
    vie_max = vie_max0;
    att_normal = att_normal0;
    def_normal = def_normal0;
    vit_normal = vit_normal0;
    taux = taux0;
    vie = vie_max;
    att = att_normal;
    def = def_normal;
    vit = vit_normal;
    etat = "normal";
    for(int i=0;i<nbre_attaques;i++){
        for(int j=0;j<2;j++){
            attaques[i][j]=attaques0[i][j];
        }
    }
    niveau = niveau0;
    xp = 0;
    vitesse_croissance = vitesse_croissance0;
    bonus_att = bonus_att0;
    residu_bonus_att = 0;
    bonus_def = bonus_def0;
    residu_bonus_def = 0;
    bonus_vie = bonus_vie0;
    residu_bonus_vie = 0;
    bonus_vit = bonus_vit0;
    residu_bonus_vit = 0;
}

int Pokemon::xp_niveau(int niv){
    if (vitesse_croissance == "rapide")
        return (4*niv*niv*niv)/5;
    else if (vitesse_croissance == "normal")
        return (niv*niv*niv);
    else if (vitesse_croissance == "parabolique")
        return (6*niv*niv*niv-75*niv*niv+500*niv-700)/5;
    else if (vitesse_croissance == "lent")
        return (5*niv*niv*niv)/4;
    return 10000;
}

bool Pokemon::Monter_niveau(){
	bool lvlUp = false;
    while(xp>=xp_niveau(niveau+1)){
        niveau ++;
		lvlUp = true;

        vie_max += (bonus_vie+residu_bonus_vie)/100;
        vie += (bonus_vie+residu_bonus_vie)/100;
        residu_bonus_vie = (bonus_vie+residu_bonus_vie)%100;

        att_normal += (bonus_att+residu_bonus_att)/100;
        residu_bonus_att = (bonus_att+residu_bonus_att)%100;

        def_normal += (bonus_def+residu_bonus_def)/100;
        residu_bonus_def = (bonus_def+residu_bonus_def)%100;

        vit_normal += (bonus_vit+residu_bonus_vit)/100;
        residu_bonus_vit = (bonus_vit+residu_bonus_vit)%100;

        // ---------------> Affichage sympa à faire
        cout<<"niveau :"<< niveau<<endl;
        cout<<"xp :"<<xp<<endl;
        cout<<"vie_max :"<<vie_max<<endl;
        cout<<"vie :"<<vie<<endl;
        cout<<"attaque :"<<att_normal<<endl;
        cout<<"defense :"<<def_normal<<endl;
        cout<<"vitesse :"<<vit_normal<<endl;
        cout<<"xp pour niveau suivant :"<<xp_niveau(niveau+1)-xp<<endl;
    }
    return lvlUp;
}
void Pokemon::Initialisation(){
    // la vie n'est pas restaurée
    att = att_normal;
    def = def_normal;
    vit = vit_normal;
}

void Pokemon::Soin(){
    vie = vie_max;
    etat = "normal";
}

void Pokemon::operator=(Pokemon &pokemon){
    sprite = pokemon.sprite;
    sprite_back = pokemon.sprite_back;
    nom = pokemon.nom;
    id = pokemon.id;
    type = pokemon.type;
    type_super_eff = pokemon.type_super_eff;
    type_pas_eff = pokemon.type_pas_eff;
    vie_max = pokemon.vie_max;
    att_normal = pokemon.att_normal;
    def_normal = pokemon.def_normal;
    vit_normal = pokemon.vit_normal;
    taux = pokemon.taux;
    vie = pokemon.vie;
    att = pokemon.att;
    def = pokemon.def;
    vit = pokemon.vit;
    etat = pokemon.etat;
    for(int i=0;i<nbre_attaques;i++){
        for(int j=0;j<2;j++){
            attaques[i][j]=pokemon.attaques[i][j];
        }
    }
    position = pokemon.position;
    niveau = pokemon.niveau;
    xp = pokemon.xp;
    vitesse_croissance = pokemon.vitesse_croissance;
    bonus_vie = pokemon.bonus_vie;
    residu_bonus_vie = pokemon.residu_bonus_vie;
    bonus_att = pokemon.bonus_att;
    residu_bonus_att = pokemon.residu_bonus_att;
    bonus_def = pokemon.bonus_def;
    residu_bonus_def = pokemon.residu_bonus_def;
    bonus_vit = pokemon.bonus_vit;
    residu_bonus_vit = pokemon.residu_bonus_vit;
}


// bug sur certains système ->
void Pokemon::operator=(const Pokemon &pokemon){
    sprite = pokemon.sprite;
    sprite_back = pokemon.sprite_back;
    nom = pokemon.nom;
    id = pokemon.id;
    type = pokemon.type;
    type_super_eff = pokemon.type_super_eff;
    type_pas_eff = pokemon.type_pas_eff;
    vie_max = pokemon.vie_max;
    att_normal = pokemon.att_normal;
    def_normal = pokemon.def_normal;
    vit_normal = pokemon.vit_normal;
    taux = pokemon.taux;
    vie = pokemon.vie;
    att = pokemon.att;
    def = pokemon.def;
    vit = pokemon.vit;
    etat = pokemon.etat;
    for(int i=0;i<nbre_attaques;i++){
        for(int j=0;j<2;j++){
            attaques[i][j]=pokemon.attaques[i][j];
        }
    }
    position = pokemon.position;
    niveau = pokemon.niveau;
    xp = pokemon.xp;
    vitesse_croissance = pokemon.vitesse_croissance;
    bonus_vie = pokemon.bonus_vie;
    residu_bonus_vie = pokemon.residu_bonus_vie;
    bonus_att = pokemon.bonus_att;
    residu_bonus_att = pokemon.residu_bonus_att;
    bonus_def = pokemon.bonus_def;
    residu_bonus_def = pokemon.residu_bonus_def;
    bonus_vit = pokemon.bonus_vit;
    residu_bonus_vit = pokemon.residu_bonus_vit;
}


bool Pokemon::Mort(){
    return (vie<=0);
}

int Pokemon::NombreAttaques(){
    int nbre=0;
    for(int i=0;i<nbre_attaques;i++){
        nbre += (attaques[i][0] <= niveau);
    }
    return nbre;
}

int Pokemon::xp_donne(){
    return 4*sqrt((niveau * (def_normal + vit_normal + att_normal)));
}

void Pokemon::SetNiveau(int niv){
    niveau = niv;
    xp = xp_niveau(niveau);
    att_normal += (niveau*bonus_att)/100;
    att = att_normal;
    vit_normal += (niveau*bonus_vit)/100;
    vit = vit_normal;
    vie_max += (niveau*bonus_vie)/100;
    vie = vie_max;
    def_normal += (niveau*bonus_def)/100;
    def = def_normal;
}

Attaque::Attaque(int id0, string nom0, int degats0, int chance0, string type0, int animation0,
                 int coef_att_adv0, int coef_def_adv0, int coef_vit_adv0, int coef_att0,
                 int coef_def0, int coef_vit0, int coef_empoisonne0, int coef_endormi0, int coef_paralyse0){
    id = id0;
    nom = nom0;
    degats = degats0;
    chance = chance0;
    type = type0;
    animation = animation0;
    coef_att_adv = coef_att_adv0;
    coef_def_adv = coef_def_adv0;
    coef_vit_adv = coef_vit_adv0;
    coef_att = coef_att0;
    coef_def = coef_def0;
    coef_vit = coef_vit0;
    coef_empoisonne = coef_empoisonne0;
    coef_endormi = coef_endormi0;
    coef_paralyse = coef_paralyse0;
}

void Attaque::operator=(Attaque attaque){
    id = attaque.id;
    nom = attaque.nom;
    degats = attaque.degats;
    chance = attaque.chance;
    type = attaque.type;
    animation = attaque.animation;
    coef_att_adv = attaque.coef_att_adv;
    coef_def_adv = attaque.coef_def_adv;
    coef_vit_adv = attaque.coef_vit_adv;
    coef_att = attaque.coef_att;
    coef_def = attaque.coef_def;
    coef_vit = attaque.coef_vit;
    coef_empoisonne = attaque.coef_empoisonne;
    coef_endormi = attaque.coef_endormi;
    coef_paralyse = attaque.coef_paralyse;
}
