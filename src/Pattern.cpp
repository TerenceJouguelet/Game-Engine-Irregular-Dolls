#include "Pattern.h"
#include "Foe.h"


void Pattern::assignAttributsWithTextFile(){
    string sheet_name; string destroy_effect_transf_preset; int color;
    this->file_assigner->assignSimpleValue("destroy_anim:", "sis", &sheet_name, &color, &destroy_effect_transf_preset);
    this->destroy_effect_transformation = new Transformator(destroy_effect_transf_preset, sheet_name, color);

    this->file_assigner->assignSimpleValue("death_sound:", "s", &this->death_sound_name);

    //cout << sheet_name << endl << destroy_effect_transf_preset << endl << this->destroy_effect_color << endl;

    this->file_assigner->assignSimpleValue("untouchable:", "b", &this->or_untouchable);

}

Pattern::Pattern(string id, string path_text_file):Handler(id, path_text_file),owner(nullptr), death_sound_name("None")
{
    //assignation des attribut en utilisant le fichier texte
    this->assignAttributsWithTextFile();
}

//ajoute un element dans la list des composants (méthode redéfini, on instancie cette fois un Projetlie [Bullet])
Movable* Pattern::addComponent(double oX, double oY, double angle_base, double speed_scale){

    //cout << oY << endl;
    Movable *M = new Bullet(oX, oY, angle_base, speed_scale, this);
    M->setPositionType(this->absolute_position);

    Bullet* B = dynamic_cast<Bullet*>(M);
    B->addDestroyAnimation(this->destroy_effect_transformation);
    B->setTouchableValue(this->or_untouchable);

    B->addDestructionSound(this->death_sound_name);
    this->components.push_back(B);

}


void Pattern::UpdateSequences(){
    Handler::UpdateSequences();
    this->destroy_effect_transformation->UpdateSequences();
}


void Pattern::checkElementsCollision(Collisionable *entity){
    int i = 0;
    list<Movable*>::iterator it = this->components.begin();

    while(it != this->components.end()){
        Bullet* B = dynamic_cast<Bullet*>(*it);
        //B->updateCara();
        //entity->updateCara();
        //cout << entity << endl;
        if(!B->isDestroy() && B->collisionTo(entity)){
            B->Deal(entity);
            entity->Deal(B);
        }
        ++it;
    }
}



void Pattern::checkCollisionWithOtherPattern(Pattern *other){
    list<Movable*>::iterator it = this->components.begin();

    while(it != this->components.end()){
        Bullet* B = dynamic_cast<Bullet*>(*it);
        if(!B->isDestroy()){
            other->checkElementsCollision(B);
        }
        ++it;
    }
}


void Pattern::destroyAll(){
    Sound(this->death_sound_name).playSound();
    list<Movable*>::iterator it = this->components.begin();
    while(it != this->components.end()){
        Bullet* B = dynamic_cast<Bullet*>(*it);
        if(!B->isDestroy()){
            B->Deal(nullptr);
        }
        ++it;
    }
}



bool Pattern::aliveStatue(Movable* entity){
    Bullet* B = dynamic_cast<Bullet*>(entity);
    /*if(this->sheets.front()->getSheetName() == "Little_Pill"){
        cout << !B->isActive() << endl;
    }*/
    if(B->isDestroy() && B->isDestroyAnimationFinish() && !B->isActive()){
        return false;
    }
    return true;
}



void Pattern::assignOwner(Foe* owner){
    this->owner = owner;
}

void Pattern::disconnectOwnerFromThisPattern(){
    if(this->owner != nullptr){
        this->owner->disconnectPattern(this->id);
    }
}

void Pattern::disconnectOwner(){
    this->owner = nullptr;
}


Pattern::~Pattern()
{
    delete this->destroy_effect_transformation;
}
