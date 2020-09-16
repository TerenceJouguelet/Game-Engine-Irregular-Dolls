#include "Destructible.h"
#include "Transformable.h"


Destructible::Destructible():destroy_effect(nullptr){
    /*//on lance la transformation bien qu'il n'y en ai pas pour l'instant (pour quelle soit active et donc pour pouvoir dessiner l'entité)*/
}

void Destructible::addDestroyAnimation(Transformator *transformator){
    //this->destroy_effect = new Transformable(0, 0, sheet, color);
    this->destroy_effect = new Transformable(0, 0, transformator);
    //this->destroy_effect->fabricateWith(transformator);

    //petite exception pour les projectiles de Alice
    if(transformator->id == "marisa_shoot1"){
        //this->destroy_effect->setSystVar(VarStat(1.0, 1.2), VarStat(-60, -120), VarStat(190), 2.0);
        //this->destroy_effect->setInitDrawOffset(0, -10);
    }
}


void Destructible::addDestroyAnimation(string sheet_name, int color, string preset){
    this->destroy_effect = new AutoTransformable(0, 0, sheet_name, color, preset);
}


void Destructible::addDestructionSound(string sound_name){
    this->destroy_sound.assignSound(sound_name);
}

void Destructible::destroy(){
    this->destroy_effect->playAnimation();
    this->destroy_effect->launch();
    this->destroy_sound.playSound();
}


bool Destructible::isDestroyAnimationFinish(){
    /*if(this->destroy_effect->sheet_name == "Bullet_Explosion"){
        cout << this->destroy_effect->isAnimationFinish() << '\t' << this->destroy_effect->isTransformationFinish() << endl;
    }*/
    return this->destroy_effect->isAnimationFinish() && this->destroy_effect->isTransformationFinish();
}


Destructible::~Destructible(){
    delete this->destroy_effect;
}
