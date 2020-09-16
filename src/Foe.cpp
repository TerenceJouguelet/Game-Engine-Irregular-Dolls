#include "Foe.h"
#include "Sheet.h"
#include <string>
#include <stdexcept>
#include "Division.h"
#include "DivisionManager.h"

using namespace std;

//fonction pour charger les images dans la map


//---------------------------------------------------------------------------------------------------------------------------------------
//
//                                                             FOE
//
//---------------------------------------------------------------------------------------------------------------------------------------

Foe::Foe(double oX, double oY, double angle_base, double speed_scale, Movator *movator, float atk_delay)
                                    : Movable::Movable(oX, oY, angle_base, speed_scale, movator), /*pattern(nullptr),*/ has_aura(false){

    this->hitbox = this->current_sheet->createHitBoxCorresponding();
    this->setPvBySheet();
    this->atk_delay = Timer(atk_delay);
    this->atk_delay.launch();
    this->dead = false;
    this->can_attack = false;

}



void Foe::addAura(Transformator *transformator){
    this->has_aura = true;
    //this->aura = new Transformable(0, 0, sheet, color);
    this->aura = new Transformable(0, 0, transformator);
    this->aura->fabricateWith(transformator);
    this->aura->launch();
}


void Foe::addAura(string sheet_name, int color, string id){
    this->has_aura = true;
    this->aura = new AutoTransformable(0, 0, sheet_name, color, id, "data/text/Effect.txt");
    this->aura->launch();
}






void Foe::adaptOriginOfPattern(Pattern *pattern){
    pattern->adaptOrigin(this->X, this->Y);
}
void Foe::adaptOriginOfDiv(DivisionManager *division_manager){
    division_manager->adaptDivOrigin(this->X, this->Y);
}





void Foe::givePattern(string pattern_id, string pattern_text_path_path){
    Pattern *pattern = new Pattern(pattern_id, pattern_text_path_path);
    pattern->assignOwner(this);
    this->patterns[pattern_id] = pattern;
}

Pattern* Foe::getReferenceOfPattern(string id){
    return this->patterns.at(id);
}

void Foe::shoot(Player &player){
    if(this->canAttack() && !this->isDestroy()){
        for(map<string, Pattern*>::iterator it = this->patterns.begin() ; it != this->patterns.end() ; ++it){
            Pattern *pattern = it->second;
            if(pattern != nullptr){
                pattern->createElements(player);
            }
        }
    }
    if(!this->atk_delay.isFinished()){
        this->atk_delay.pass();
        if(this->atk_delay.isFinished()){
            this->can_attack = true;
        }
    }
}

bool Foe::canAttack(){
    return this->can_attack;
}

void Foe::disconnectPattern(string id){
    //this->pattern = nullptr;
    //this->patterns.erase(id);
    /*for(map<string, Pattern*>::iterator it = this->patterns.begin() ; it != this->patterns.end() ; ++it){
        cout << "F";
        Pattern *pattern = it->second;
        if(pattern != nullptr && pattern->id == id){
            it = this->patterns.erase(it);
        }
        cout << "E" << endl;
    }*/
    Pattern *pattern = this->patterns[id];
    if(pattern != nullptr){
        this->patterns.erase(id);
    }
}


void Foe::Move(Player *player){
    /*if(this->sheet_name == "Satori"){
        cout << "mukyu" << endl;
    }*/
    //cout << this->Y << endl;
    if(!this->isDestroy()){
        Movable::Move(player);
        for(map<string, Pattern*>::iterator it = this->patterns.begin() ; it != this->patterns.end() ; ++it){
            if(it->second != nullptr){
                this->adaptOriginOfPattern(it->second);
            }
        }
    }
    if(this->animation != nullptr){
        this->animation->changeStatut(this->Vect);
    }

    if(this->has_aura){
        this->aura->reposition(this->X, this->Y);
    }
}

void Foe::updateCara(){
    this->hitbox->updateCara(this->X, this->Y, this->angle.getValue(), this->scale.getValue());
}


void Foe::updateValues(){
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
        /*this->dead = true;
        if(this->pattern != nullptr){
            this->pattern->use();   //si il meurt, on dira que son pattern à été quand même utilisé
        }
        this->destroy();
        this->destroy_effect->reposition(this->X, this->Y);
        this->stop();   //on stop la tranformation, donc on affiche plus l'entité
        this->penetrable = true;*/
    }
}


bool Foe::isDestroy(){
    return this->dead;
}

void Foe::Deal(Collisionable* target){
    if(target == nullptr){
        if(!this->isDestroy()){
            this->dead = true;
            for(map<string, Pattern*>::iterator it = this->patterns.begin() ; it != this->patterns.end() ; ++it){
                if(it->second != nullptr){
                    it->second->use();   //si il meurt, on dira que son pattern à été quand même utilisé
                    it->second->disconnectOwner();
                }
            }
            this->destroy();
            this->destroy_effect->reposition(this->X, this->Y);
            this->stop();   //on stop la tranformation, donc on affiche plus l'entité
            this->penetrable = true;
        }
    }

    else if(target != nullptr && !this->untouchable && (!target->isPenetrable())){
        //un ennemi ne peut être touché que par un projectiles, si il est touché, il perd des pv
        Bullet *B = dynamic_cast<Bullet*>(target);
        this->pv -= B->getDamage();
        if(this->pv <= 0 && !this->isDestroy()){
            this->dead = true;
            for(map<string, Pattern*>::iterator it = this->patterns.begin() ; it != this->patterns.end() ; ++it){
                if(it->second != nullptr){
                    it->second->use();   //si il meurt, on dira que son pattern à été quand même utilisé
                    it->second->disconnectOwner();
                }
            }
            this->destroy();
            this->destroy_effect->reposition(this->X, this->Y);
            this->stop();   //on stop la tranformation, donc on affiche plus l'entité
            this->penetrable = true;
        }
    }

    /*
    if(target == nullptr){
        if(!this->isDestroy()){
            this->dead = true;
            if(this->pattern != nullptr){
                this->pattern->use();   //si il meurt, on dira que son pattern à été quand même utilisé
            }
            this->destroy();
            this->destroy_effect->reposition(this->X, this->Y);
            this->stop();   //on stop la tranformation, donc on affiche plus l'entité
            this->penetrable = true;
        }
    }
    */
}


//on redéfini la méthode pour l'animation puissent aussi se rejouer lorsque l'on change de keyframe
void Foe::adaptToKeyframe(Player *player){
    if(!this->isDestroy()){
        Movable::adaptToKeyframe(player);
        if(this->lambda.isKeyChanged() && this->theta.isKeyChanged()){
            delete this->hitbox;
            this->hitbox = this->current_sheet->createHitBoxCorresponding();
        }
    }
}

//les points de vie en fonction de l'ennemi
void Foe::setPvBySheet(){
    if(this->sheet_name == "Fairy_Maid"){
        this->pv = 70;
    }
    else if(this->sheet_name == "Fairy"){
        this->pv = 70;
    }
    else if(this->sheet_name == "Fairy_Sunflower"){
        this->pv = 300;
    }
    else if(this->sheet_name == "Fairy_Shield"){
        this->pv = 200;
    }
    else if(this->sheet_name == "Satori"){
        this->pv = 100;
    }
}


void Foe::drawFrontEffects(bool with_animation){
    if(this->isDestroy()){
        this->destroy_effect->Draw(with_animation);
    }
}

void Foe::drawBackEffects(bool with_animation){
    if(this->has_aura){
        this->aura->Draw(with_animation);
    }
}




Foe::~Foe(){
    for(map<string, Pattern*>::iterator it = this->patterns.begin() ; it != this->patterns.end() ; ++it){
        if(it->second != nullptr){
            it->second->use();
            it->second->disconnectOwner();
        }
    }
    if(this->has_aura){
        delete this->aura;
    }
}






//---------------------------------------------------------------------------------------------------------------------------------------
//
//                                                              BOSS
//
//---------------------------------------------------------------------------------------------------------------------------------------

Boss::Boss(double oX, double oY, string id, string path_text_file):
                                                Movator(id, path_text_file)
                                                ,Foe(oX, oY, 0, 1, this, 10.0){

    this->addDestroyAnimation("Shock_Wave", 1, "None");
    this->addDestructionSound("Enemy_Death");

    this->addAura("Foe_Aura", 0, "Satori_Aura");

    this->ennemy_indicator = new AutoMovable(0, 0, "Indicator", "data/text/Satori.txt");
    this->blink_time = 0;
    this->in_transition = false;
}





void Boss::changeTrajectory(string id, string path_text_file){
    this->blink_time = 0;
    //ce que font les constructeurs

    delete this->file_assigner;
    this->file_assigner = new AssignFileSystem(path_text_file, id, "@", "#");

    Movator::assignAttributsWithTextFile();

    this->fabricateWith(this);

    //cout << "mukyu" << endl;*/
}


void Boss::moveToCenter(){
    this->in_transition = true;
    //(KEY) distance, duree, angle, fct_dir, inf_bound_dir, sup_bound_dir, amp_dir, fct_speed, speed_last, infinite, wait_val
    this->thetaS.eraseAllKeyFrame();
    this->lambdaS.eraseAllKeyFrame();

    double distance = sqrt( (System::X_GAME_MIDDLE - this->X)*(System::X_GAME_MIDDLE - this->X) + (170 - this->Y)*(170 - this->Y) );
    double center_angle = (((System::X_GAME_MIDDLE < this->X) ? M_PI : 0) + atan((170 - this->Y)/(System::X_GAME_MIDDLE - this->X)))  * (180 / M_PI);
    Keyframe Key = Keyframe(distance, 2.0, center_angle, "line", 0.0, 1.0, 1.0, "dec", -1.0, false , 0.0, false);
    this->thetaS.changeType("theta");
    this->lambdaS.changeType("lambda");
    this->thetaS.addKeyframe(Key);
    this->lambdaS.addKeyframe(Key);

    this->lambda = VarStat(this->lambdaS.getIterSequence(), false);
    this->theta = VarStat(this->thetaS.getIterSequence(), false);
}



void Boss::setPhasePv(int pv){
    this->in_transition = false;
    this->pv = pv;
}

int* Boss::getRefofPv(){
    return &this->pv;
}


void Boss::Deal(Collisionable* target){
    if(target != nullptr && !this->untouchable && (!target->isPenetrable())){
        //un ennemi ne peut être touché que par un projectiles, si il est touché, il perd de pv
        Bullet *B = dynamic_cast<Bullet*>(target);
        this->pv -= B->getDamage();
    }
}


void Boss::Move(Player *player){
    this->UpdateSequences();
    this->ennemy_indicator->adaptOrigin(this->X, 564);
    this->ennemy_indicator->Move(nullptr);
    Foe::Move(player);
}


void Boss::Draw(bool with_animation){
    Foe::Draw(with_animation);
}

void Boss::drawIndicator(bool with_animation){
    if(!this->in_transition && this->pv < 500){
        if(this->blink_time%10 < 5){
            this->ennemy_indicator->Draw(with_animation);
        }
    }
    else if(!this->in_transition && this->pv < 700){
        if(this->blink_time%15 < 10){
            this->ennemy_indicator->Draw(with_animation);
        }
    }
    else{
        this->ennemy_indicator->Draw(with_animation);
    }
    this->blink_time = (this->blink_time + 1)%60;
}

Boss::~Boss(){
    delete this->ennemy_indicator;
}



