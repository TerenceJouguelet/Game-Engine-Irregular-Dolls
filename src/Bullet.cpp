#include "Bullet.h"
#include "Sheet.h"
#include <stdexcept>
#include <iostream>
#include "Movable.h"
#include "Handler.h"
#include "Pattern.h"

using namespace std;




Bullet::Bullet(double oX, double oY, double angle_base, double speed_scale, Movator *movator)
                                    : Movable::Movable(oX, oY, angle_base, speed_scale, movator){

    this->hitbox = this->current_sheet->createHitBoxCorresponding();
    this->setDamageBySheet();
    this->dead = false;

}


int Bullet::getDamage(){
    return this->damage;
}


//on redéfini la méthode pour l'animation puissent aussi se rejouer lorsque l'on change de keyframe
void Bullet::adaptToKeyframe(Player *player){
    if(!this->isDestroy()){
        Movable::adaptToKeyframe(player);
        if(this->lambda.isKeyChanged() && this->theta.isKeyChanged()){
            delete this->hitbox;
            this->hitbox = this->current_sheet->createHitBoxCorresponding();
        }
    }
}

void Bullet::updateCara(){
    this->hitbox->updateCara(this->X, this->Y, this->angle.getValue(), this->scale.getValue());
}


void Bullet::updateValues(){
    Movable::updateValues();

    if(this->absolute_position){
        this->adaptAngle(this->theta.getValue() + this->base_angle + 90);
    }
    else{
        if(*this->aimed == false){
            this->adaptAngle(this->theta.getValue() + this->base_angle - 180);
        }
        else{
            this->adaptAngle(this->theta.getValue() + this->base_angle + 90);
        }
    }



    if(this->isTrajectoryEnd()){
        this->Deal(nullptr);    //destruction de l'entité
    }
}

bool Bullet::isDestroy(){
    return this->dead;
}

void Bullet::Deal(Collisionable* target){
    //Destructible *entity = dynamic_cast<Destructible*>(target);
    if(target == nullptr || (!this->untouchable && !target->isPenetrable())){
        this->dead = true;
        this->destroy();
        this->destroy_effect->reposition(this->hitbox->getXImpact(), this->hitbox->getYImpact());
        //if(this->sheet_name != "shoot_1"){   //petite exception
            //this->cancel();
            this->stopMovement();
        //}
        //else{
            this->stop();
        //}

    }
}


//si se sont des tirs de joueur, ces dernier auront une certaine puissance correspondant au dégat qu'ils causeront au PV de l'ennemi
void Bullet::setDamageBySheet(){
    this->damage = 0;
    if(this->sheet_name == "shoot_1"){
        if(this->color == 5){
            damage = 10;
        }
        else{
            damage = 2;
        }
    }
    if(this->sheet_name == "Bomb_Sphere"){
        damage = 4;
    }
}


void Bullet::drawFrontEffects(bool with_animation){
    if(this->isDestroy()){
        this->destroy_effect->Draw(with_animation);
    }
}


Bullet::~Bullet()
{

}
