#include "AnimSystem.h"
#include <iostream>
#include "System.h"
#include <vector>

#define IDLE 0
#define DIR_LEFT 1
#define LEFT 2
#define DIR_RIGHT 3
#define RIGHT 4



using namespace std;

AnimSystem::AnimSystem(vector<int> &statuts, double delay, bool loop)
{
    this->statuts = statuts;
    this->statut = IDLE;
    this->frame = 0;
    this->delay = timeToFrame(delay);
    this->i_delay = 0;
    this->reverse = false;
    this->loop = loop;
    this->from_right = false;
    this->finish = false;
}


/*//on ne prend pas en compte pour l'instant le cas le l'animation se jou à l'envers*/
bool AnimSystem::isFinised(){
    /*if(!this->loop && this->frame == this->statuts[statut] - 1){
        return true;
    }
    else{
        return false;
    }*/
    return this->finish;
}


//pour lancer l'animation, c'est à dire la remettre à la première frame
void AnimSystem::lauch(){
    this->frame = 0;
    this->finish = false;
}


//Surcharge: pour le joueur
void AnimSystem::changeStatut(const Uint8 *keystates){
    //-----------------------------------------------------------------------------------
    // Animations qui seront jouées, dépendentes des évenements provenant du clavier
    //-----------------------------------------------------------------------------------
    int prec_statut = this->statut;

    if( keystates[ SDL_SCANCODE_LEFT ] || (keystates[ SDL_SCANCODE_LEFT ] && keystates[ SDL_SCANCODE_UP ])
                               || (keystates[ SDL_SCANCODE_LEFT ] && keystates[ SDL_SCANCODE_DOWN ]) )
    {
        if(this->statut != LEFT){
            this->statut = DIR_LEFT;
        }

    }

    else if(keystates[ SDL_SCANCODE_RIGHT ] || (keystates[ SDL_SCANCODE_RIGHT ] && keystates[ SDL_SCANCODE_UP ])
                                    || (keystates[ SDL_SCANCODE_RIGHT ] && keystates[ SDL_SCANCODE_DOWN ]) )
    {
        if(this->statut != RIGHT){
            this->statut = DIR_RIGHT;
        }

    }

    else if(keystates[ SDL_SCANCODE_UP ])
    {
        this->statut = IDLE;
    }

    else if(keystates[ SDL_SCANCODE_DOWN ])
    {
        this->statut = IDLE;
    }
    else
    {
        this->statut = IDLE;
    }


    if(this->statut != prec_statut){
        this->frame = 0;
    }

    //si les animation de transition sont terminé
    if(this->statut == DIR_RIGHT && this->frame == this->statuts[DIR_RIGHT] - 1){
        this->statut = RIGHT;
        this->frame = 0;
    }
    else if(this->statut == DIR_LEFT && this->frame == this->statuts[DIR_LEFT] - 1){
        this->statut = LEFT;
        this->frame = 0;
    }


}


//Surcharge: pour les ennemis
void AnimSystem::changeStatut(Vector2D &V){

    if(this->statuts.size() == 1){
        this->statut = 0;
    }
    else{
        int prec_statut = this->statut;

        V.theta = (360 + (int)V.theta)%360;

        if( /*(V.theta >= 85 && V.theta <= 95)*/false || (V.theta >= 265 && V.theta <= 275) || V.lambda < 0.1){   //si l'entité va en bas ou en haut ou est très ralentie
            if(this->statut != IDLE){
                if(this->statut == RIGHT){
                    this->statut = DIR_RIGHT;
                    this->reverse = true;
                }
                if(this->statut == LEFT){
                    this->statut = DIR_LEFT;
                    this->reverse = true;
                }
            }
        }

        else if(V.theta > 90 && V.theta <= 270){ //si l'entité va à gauche
            if(this->statut != LEFT){
                this->statut = DIR_LEFT;
                this->reverse = false;
            }

        }
        else if(V.theta > 270 || V.theta < 90){   //si l'entité va à droite
            if(this->statut != RIGHT){
                this->statut = DIR_RIGHT;
                this->reverse = false;
            }

        }
        else{   //par defaut
            if(this->statut != IDLE){
                if(this->statut == RIGHT){
                    this->statut = DIR_RIGHT;
                    this->reverse = true;
                }
                if(this->statut == LEFT){
                    this->statut = DIR_LEFT;
                    this->reverse = true;
                }
            }

        }


        if(this->statut != prec_statut){
            if(!reverse){
                this->frame = 0;
            }
            else{
                this->frame = this->statuts[this->statut] - 1;
            }
        }

        //cout << this->reverse << endl;

        //si les animation de transition sont terminé
        if(this->statut == DIR_RIGHT && !this->reverse && this->frame == this->statuts[DIR_RIGHT] - 1){
            this->statut = RIGHT;
            this->frame = 0;
            this->reverse = false;
        }
        else if(this->statut == DIR_LEFT && !this->reverse && this->frame == this->statuts[DIR_LEFT] - 1){
            this->statut = LEFT;
            this->frame = 0;
            this->reverse = false;
        }
        if(this->statut == DIR_RIGHT && this->reverse && this->frame == 0){
            this->from_right = false;

            this->statut = IDLE;
            this->frame = 0;
            this->reverse = false;
        }
        else if(this->statut == DIR_LEFT && this->reverse && this->frame == 0){
            this->from_right = true;

            this->statut = IDLE;
            this->frame = 0;
            this->reverse = false;
        }

        if(this->statut == IDLE && this->from_right){
            this->from_right = true;
        }
    }
}

bool AnimSystem::moveFromRight(){
    return this->from_right;
}



int AnimSystem::nextFrame(){
    if(!this->isFinised()){
        if(!this->reverse){
            if( (this->i_delay++)%this->delay == 0 ){
                this->frame = (this->frame + 1)%this->statuts[statut];
                if(!this->loop && this->frame == 0){
                    this->finish = true;
                    //this->frame--;
                }
                return this->frame;
            }
            return this->frame;
        }
        else{
            if( (this->i_delay++)%this->delay == 0 ){
                this->frame = this->frame == 0 ? 0 : (this->frame - 1);
                if(!this->loop && this->frame == 0){
                    this->finish = true;
                }
                return this->frame;
            }
            return this->frame;
        }
    }

}

int AnimSystem::getStatut(){
    return this->statut;
}

AnimSystem::~AnimSystem(){
}
