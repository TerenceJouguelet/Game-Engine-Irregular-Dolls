#include "Collisionable.h"
#include "System.h"

Collisionable::Collisionable(){
    this->untouchable = false;
    this->penetrable = false;
}

Collisionable::Collisionable(string sheet_name){
    this->hitbox = Sheet::createHitBoxBySheetName(sheet_name);
    this->untouchable = false;
    this->penetrable = false;
}


Collisionable::Collisionable(Collisionable const& cpy): hitbox(0){
    if(cpy.hitbox != nullptr){
        hitbox = cpy.hitbox->getACopy();
    }
}

/*void Collisionable::turnUntouchable(){
    this->untouchable = true;
}*/

void Collisionable::setTouchableValue(bool value){
    this->untouchable = value;
}
void Collisionable::setPenetrableValue(bool value){
    this->penetrable = value;
}

void Collisionable::turnPenetrable(){this->penetrable = true;}      //STUPIDE
void Collisionable::turnUnPenetrable(){this->penetrable = false;}   //
bool Collisionable::isPenetrable(){return this->penetrable;}


bool Collisionable::collisionTo(Collisionable *other){
    this->updateCara();
    other->updateCara();
    /*Positionable *P1 = dynamic_cast<Positionable*>(this);
    Positionable *P2 = dynamic_cast<Positionable*>(other);*/
    bool retour = this->hitbox->overlap(other->hitbox);
    return retour;
}


void Collisionable::Deal(Collisionable *other){
    //par defaut, la collision ne faire rien
}


Collisionable::~Collisionable()
{
    delete this->hitbox;
}









Collisionor::Collisionor(){
    this->or_untouchable = false;
    this->or_penetrable = false;
}




