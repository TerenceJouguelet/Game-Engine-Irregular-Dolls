#include "Division.h"

void Division::assignAttributsWithTextFile(){
    string sheet_name; string destroy_effect_transf_preset; int color;

    this->file_assigner->assignSimpleValue("destroy_anim:", "sis", &sheet_name, &color, &destroy_effect_transf_preset);
    this->destroy_effect_transformation = new Transformator(destroy_effect_transf_preset, sheet_name, color);


    string aura_sheet_name = "None"; string trans_id; int aura_color;
    bool found = this->file_assigner->assignSimpleValue("aura:", "sis", &aura_sheet_name, &aura_color, &trans_id);
    this->no_aura = found ? false : true;

    if(found){
        this->aura_transformator = new Transformator("None", aura_sheet_name, aura_color);
    }

    string unique_id;
    found = this->file_assigner->assignSimpleValue("pattern:", "s", &unique_id);
    if(found){
        this->pattern_id.push_back(unique_id);
    }
    else{
        this->file_assigner->assignListofValue("pattern", &this->pattern_id);
    }


    this->pattern_text_path_path = "data/text/ecl.txt";
    this->file_assigner->assignSimpleValue("pattern_text_path_path:", "s", &this->pattern_text_path_path);


    //cout << this->id  << '\t' << "~" << this->pattern_id << endl;

    this->file_assigner->assignSimpleValue("atk_delay:", "f", &this->atk_delay);
    this->file_assigner->assignSimpleValue("death_sound:", "s", &this->death_sound_name);

    this->file_assigner->assignSimpleValue("untouchable:", "b", &this->or_untouchable);
    this->file_assigner->assignSimpleValue("penetrable:", "b", &this->or_penetrable);

}


Division::Division(string id, string path_text_file):Handler(id, path_text_file)
{
    //assignation des attribut en utilisant le fichier texte
    this->assignAttributsWithTextFile();
    this->new_bundle = false;
}

//ajoute un element dans la list des composants (méthode redéfini, on instancie cette fois un Ennemi [Foe])
Movable* Division::addComponent(double oX, double oY, double angle_base, double speed_scale){

    Movable *M = new Foe(oX, oY, angle_base, speed_scale, this, this->atk_delay);
    M->setPositionType(this->absolute_position);

    Foe *F = dynamic_cast<Foe*>(M);
    F->setTouchableValue(this->or_untouchable);
    F->setPenetrableValue(this->or_penetrable);

    for(int i = 0 ; i < this->pattern_id.size() ; i++){
        F->givePattern(this->pattern_id[i], this->pattern_text_path_path);
    }

    F->addDestroyAnimation(this->destroy_effect_transformation);

    F->addDestructionSound(this->death_sound_name);

    if(!this->no_aura){
        F->addAura(this->aura_transformator);
    }

    for(int i = 0 ; i < this->pattern_id.size() ; i++){
        Pattern *ref_pattern = dynamic_cast<Foe*>(F)->getReferenceOfPattern(this->pattern_id[i]);
        this->pattern_bundle.push_back(ref_pattern);
    }

    this->components.push_back(F);
    this->new_bundle = true;

}


void Division::shootTheBullets(Player &player){
    for(list<Movable*>::iterator it = this->components.begin() ; it != this->components.end() ; ++it){
        Foe *F = dynamic_cast<Foe*>(*it);

        //bool can_atk_before_shooting = F->canAttack();

        F->shoot(player);
        /*if(!can_atk_before_shooting && F->canAttack()){     //si l'ennemi vient d'acquerir la capacité d'attaquer, il faut gérer son pattern
            this->new_bundle = true;
        }*/
    }
}

bool Division::hasNewBundle(){
    return this->new_bundle;
}

list<Pattern*> Division::getPatternsBundle(){
    list<Pattern*> bundle = this->pattern_bundle;
    this->pattern_bundle.clear();
    this->new_bundle = false;
    return bundle;
}


list<Movable*>* Division::getRefOfComponents(){
    return &this->components;
}

/*
void Division::checkElementsCollision(Player &player){
    int i = 0;
    list<Movable*>::iterator it = this->components.begin();

    while(it != this->components.end()){

        Foe* F = dynamic_cast<Foe*>(*it);
        B->updateCara();
        if(!player.isDead() && B->collisionTo(player)){
            player.Kill();
            delete *it;
            it = this->components.erase(it);
            if(this->isEmpty()){
                break;
            }
        }
        else{
            ++it;
        }
    }
}
*/


void Division::destroyAll(){
    //Sound(this->death_sound_name).playSound();
    list<Movable*>::iterator it = this->components.begin();
    while(it != this->components.end()){
        Foe* F = dynamic_cast<Foe*>(*it);
        if(!F->isDestroy()){
            F->Deal(nullptr);
        }
        ++it;
    }
}



void Division::checkCollisionWithPlayerShoot(Player &player){
    for(list<Movable*>::iterator it = this->components.begin() ; it != this->components.end() ; ++it){
        Foe *F = dynamic_cast<Foe*>(*it);
        player.checkShootCollision(F);
        player.checkBombCollision(F);
    }
}




bool Division::aliveStatue(Movable* entity){
    Foe* F = dynamic_cast<Foe*>(entity);
    if(F->isDestroy() && F->isDestroyAnimationFinish() && !F->isActive()){
        return false;
    }
    return true;
}

Division::~Division()
{
    /*while(!this->sheets.empty()){
        delete this->sheets.front();
        this->sheets.erase(this->sheets.begin());
    }*/
    delete this->destroy_effect_transformation;

    if(!this->no_aura){
        delete this->aura_transformator;
    }

    /*for(list<Movable*>::iterator it = this->components.begin(); it != this->components.end(); it++){
        delete *it;
        it = this->components.erase(it);
    }*/
}
