#include "Player.h"
#include "math.h"

#include <SDL2/SDL.h>
#include <iostream>
#include "System.h"
#include "Pattern.h"
#include "DivisionManager.h"

using namespace std;

//lorsque l'on réapparait, il y a un petit temps durant lequel le personnage remonte
#define TIME_RESPAWN 0.7
//vitesse de base du joueur
#define PLAYER_BASE_SPEED 5

#define N_BOMB 2
#define N_LIVES 4

#define BOMB_DURATION 9.0

Player::Player(string sheet_name, double X, double Y, int color):AutoTransformable(X, Y, sheet_name, color, "None"), death_time(1.0), ghost_time(4.0)
        , aura_1(X, Y, "Player_Aura_1", 0, "Rotation_Spawn_1"), aura_2(X, Y, "Player_Aura_2", 0, "Rotation_Spawn_2"), Transformator("None", sheet_name, color)
{
    this->speed = PLAYER_BASE_SPEED;
    this->hitbox = this->current_sheet->createHitBoxCorresponding();
    this->dead = false;
    this->ghost_mode = false;
    this->lives = N_LIVES;
    this->lost = false;
    this->focus = false;
    this->first_focus = true;

    this->is_shooting = false;
    this->main_shoot = new Pattern("marisa_shoot1", "data/text/player.txt");

    this->dolls = new DivisionManager("Dolls", "data/text/player.txt", 0.0);

    this->bomb = new Pattern("alice_bomb", "data/text/player.txt");
    this->n_bomb = N_BOMB;
    this->using_bomb = false;
    this->bomb_duration = Timer(BOMB_DURATION);

    this->addDestroyAnimation("Player_Death", 0, "Player_Extinction");
    this->death_circle = new Death_Circle();
    this->death_circle_sound.assignSound("Pulse_3");

    this->addDestructionSound("Player_Death");

    /*this->penetrable = true;
    this->untouchable = true;*/
}

void Player::updateCara(){
    this->hitbox->updateCara(this->X, this->Y, 0, 1);
}

//renvoie la la direction (angle) que doit avoir une objet de coordonnée X, Y pour atteindre le joueur
double Player::getDirAimed(double X, double Y){
    return (((this->X < X) ? M_PI : 0) + atan((this->Y - Y)/(this->X - X)))  * (180 / M_PI);
}

bool Player::isDestroy(){
    return this->dead;
}

void Player::Deal(Collisionable* target){
    if(!this->isDestroy() && !this->ghost_mode && !this->untouchable && !target->isPenetrable()){
        this->dead = true;
        this->lives--;
        this->death_time.launch();

        this->destroy();
        this->destroy_effect->reposition(this->X, this->Y);

        //this->focus = false;
        this->aura_1.stop();
        this->aura_2.stop();

        this->turnPenetrable();
    }
}

void Player::action(){

    const Uint8 *keystates = SDL_GetKeyboardState( NULL ); //tableau des évenements

    this->dolls->adaptDivOrigin(this->X, this->Y);
    this->dolls->createDivElements(*this);
    this->dolls->moveDivElements(*this);

    if(!this->isDestroy()){
        if(!this->ghost_mode || !this->ghost_time.isTimeUnder(TIME_RESPAWN)){

            //on déplace le personnage
            this->moveEvent(keystates);
            //on le replace si il sort de l'écran
            this->replaceIfOut();
            //on change le statut
            if(this->hasAnimSystem()){
                this->animation->changeStatut(keystates);
            }

            //possibilité de focus
            this->focusEvent(keystates);



            if(this->ghost_mode){
                this->ghost_time.pass();
                if(this->ghost_time.isFinished()){
                    this->ghost_mode = false;
                }
            }

        }
        else{
            this->Y -= 100/(timeToFrame(TIME_RESPAWN)); //on parcours 100 pixels de haut lorsque l'on réapparait
            this->ghost_time.pass();
        }
    }
    else{
        this->death_time.pass();
        if(this->death_time.isFinished()){
            this->dead = false;
            this->ghost_mode = true;
            this->ghost_time.launch();
            this->X = System::GAME_WIDTH/2;
            this->Y = System::GAME_HEIGHT + 50;

            if(this->lives <= 0){
                this->lost = true;
            }
            else{
                this->n_bomb = N_BOMB;

                this->death_circle->activate();
                this->death_circle_sound.playSound();
                this->turnUnPenetrable();   //les projectiles seront donc détruits, même en mode fantôme
            }

        }
    }


    //si on appuie sur W, on tire
    if(keystates[ SDL_SCANCODE_Z ]){
        this->is_shooting = true;
    }
    else{
        this->is_shooting = false;
    }


    //on met à jour la hitbox
    //this->updateCara();

    //on met à jour l'origine du pattern
    this->main_shoot->adaptOrigin(this->X, this->Y);


    //on met à jour la position des auras
    if(this->aura_1.isActive() && this->aura_2.isActive()){
        this->aura_1.reposition(this->X, this->Y);
        this->aura_2.reposition(this->X, this->Y);
    }


}

void Player::shoot(){

    const Uint8 *keystates = SDL_GetKeyboardState( NULL ); //tableau des évenements


    if(!this->using_bomb && this->n_bomb > 0 && !this->isDestroy()){
        if(keystates[ SDL_SCANCODE_X ]){
            this->using_bomb = true;
            this->bomb_duration.launch();
            this->n_bomb--;
            this->bomb->resetTime();
        }
    }



    if(!this->isDestroy() && this->is_shooting){
        this->main_shoot->createElements(*this);
        this->dolls->shootTheBulletsOfDiv(*this);
    }


}

void Player::moveShoot(){
    this->main_shoot->moveElements(*this);
    this->dolls->movePatterns(*this);

    if(this->using_bomb){
        this->bomb->createElements(*this);
        this->bomb->moveElements(*this);
        this->bomb_duration.pass();
        if(this->bomb_duration.isFinished()){
            this->using_bomb = false;
        }
    }
    this->bomb->adaptOrigin(this->X, this->Y);
}


bool Player::Lost(){
    return this->lost;
}



int* Player::getRefOfLives(){
    return &this->lives;
}

int* Player::getRefOfBombs(){
    return &this->n_bomb;
}




void Player::checkShootCollision(Collisionable *target_foe){
   this->main_shoot->checkElementsCollision(target_foe);
   this->dolls->checkPatternsCollision(target_foe, nullptr);
}

void Player::checkBombCollision(Collisionable *target_foe){
    if(this->using_bomb){
        if(!target_foe->isDestroy()){
            this->bomb->checkElementsCollision(target_foe);
        }
   }
}

void Player::checkCollisionWithDeathCircle(Pattern *target_pattern){
    if(this->death_circle->isActivate()){
        target_pattern->checkElementsCollision(death_circle);
        //cout << death_circle.hitbox-> << endl;
        this->death_circle->pass();
    }
}


void Player::checkCollisionWithBomb(Pattern *target_pattern){
    if(this->using_bomb){
        target_pattern->checkCollisionWithOtherPattern(this->bomb);
    }
}


void Player::affectOriginOf(Handler *handler){
    handler->adaptOrigin(this->X, this->Y);
}




void Player::drawFrontEffects(bool with_animation){
    if(this->isDestroy()){
        this->destroy_effect->Draw(with_animation);
    }
}




void Player::Draw(bool with_animation){

    //On ne dessine pas le personnage si l'on est mort. En ghost mode, on dessine le personnage toutes les 5 frames pour avoir un effet de clignotement
    if(!this->isDestroy() && this->ghost_time.isTicksMultipleBy(5)){
        Drawable::Draw(with_animation);
    }

    if(this->aura_1.isActive() && this->aura_2.isActive()){
        //on modifie les données de transformation
        /*this->aura_1.updateTransformSequence();
        this->aura_2.updateTransformSequence();*/

        //this->aura_1.updateTransformValue();
        //this->aura_2.updateTransformValue();
    }

    this->aura_1.Draw(with_animation);
    this->aura_2.Draw(with_animation);

    this->dolls->drawDivAndPatterns(with_animation);
    this->main_shoot->drawElements(with_animation);
    if(this->using_bomb){
        this->bomb->drawElements(with_animation);
    }
    this->death_circle->Draw(with_animation);

    this->drawFrontEffects(with_animation);

}




//pour déplcer le personnage selon les touches saisis
void Player::moveEvent(const Uint8 *keystates){
    int speed_diag = round(sqrt(pow(this->speed,2)/2));  //speed_diag défini avec pythagore, pour ne pas se déplacer trop vite en diagonale

    //-------------------------------------------------
    // On gère les évenements de déplacements
    //-------------------------------------------------
    if(keystates[ SDL_SCANCODE_UP ])    //si on appuie sur la fleche du haut...
    {
        if(keystates[ SDL_SCANCODE_DOWN ]) // ...et aussi sur la fleche du bas...
        {
         // (on ne fait rien car les directions sont opposées)
        }
        else if(keystates[ SDL_SCANCODE_RIGHT ]) // ...ou alors sur la fleche de droite...
        {
            this->X += speed_diag;
        }
        else if(keystates[ SDL_SCANCODE_LEFT ]) //...ou alors sur la flèche de gauche...
        {
            this->X -= speed_diag;
        }
        else
        {
            this->Y -= this->speed;  //...ou si on appuie SEULEMENT sur la flèche du haut
        }
    }

    //etc...

    if(keystates[ SDL_SCANCODE_RIGHT ])
    {
        if(keystates[ SDL_SCANCODE_LEFT ])
        {

        }
        else if (keystates[ SDL_SCANCODE_UP ])
        {
            this->Y -= speed_diag;
        }
        else if(keystates[ SDL_SCANCODE_DOWN ])
        {
            this->Y += speed_diag;
        }
        else
        {
            this->X += this->speed;
        }
    }

    if(keystates[ SDL_SCANCODE_DOWN ])
    {
        if (keystates[ SDL_SCANCODE_UP ])
        {

        }
        else if (keystates[ SDL_SCANCODE_RIGHT ])
        {
            this->X += speed_diag;
        }
        else if(keystates[ SDL_SCANCODE_LEFT ])
        {
            this->X -= speed_diag;
        }
        else
        {
        this->Y += this->speed;
        }
    }

    if(keystates[ SDL_SCANCODE_LEFT ])
    {
        if (keystates[ SDL_SCANCODE_RIGHT ])
        {

        }
        else if (keystates[ SDL_SCANCODE_UP ])
        {
            this->Y -= speed_diag;
        }
        else if(keystates[ SDL_SCANCODE_DOWN ])
        {
            this->Y += speed_diag;
        }
        else
        {
            this->X -= this->speed;
        }
    }

}

void Player::focusEvent(const Uint8 *keystates){
    //on passe en mode focus en maintenant shift
    if(keystates[ SDL_SCANCODE_LSHIFT ]){
        if(!this->focus){
            this->aura_1.launch();
            this->aura_2.launch();

            if(!this->first_focus){
                this->dolls->reverseTrajectoryOfElements();
            }
            this->first_focus = false;
            this->dolls->changeMoveSegmentOfElements(1);
        }
        this->focus = true;
        this->speed = 2;
    }
    else{
        if(this->focus){
            this->aura_1.cancel();
            this->aura_2.cancel();

            this->dolls->reverseTrajectoryOfElements();
            this->dolls->changeMoveSegmentOfElements(1);
        }
        this->focus = false;
        this->speed = PLAYER_BASE_SPEED;
    }
}


//pour replacer le personnage correctement si il sort de l'écran
void Player::replaceIfOut(){
    this->newBoundingBoxSize(); //on a besoin de la bounding box pour calculer la collision exact sur le bord du mur
    System::replaceIfOut(&this->X, &this->Y, this->width_BB, this->height_BB);
}

Player::~Player()
{
    delete this->main_shoot;
    delete this->dolls;
    delete this->death_circle;
    delete this->bomb;
}
