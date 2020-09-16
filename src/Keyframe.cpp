#include "Keyframe.h"
//#include <cmath>
#include <iostream>
//#include <math.h>

#include "System.h"

#define BORNE_GAUSS 2.3
#define BORNE_CARRE 2
#define M_PI 3.14159265359

using namespace std;

Keyframe::Keyframe(){

}

bool Keyframe::isInfinite(){
    return this->infinite;
}

bool Keyframe::isLooping(){
    return this->loop;
}

//Keyframe utilisant des fonctions mathématiques
Keyframe::Keyframe(double distance, double duree, double angle, string fct_dir, float inf_bound_dir,
                              float sup_bound_dir, float amp_dir, string fct_speed, float const_speed_value, bool infinite, float wait_val, bool loop)
{
    this->loop = loop;

    this->distance = distance;
    this->time = timeToFrame(duree);
    this->angle = angle;
    this->fct_dir = fct_dir;
    this->inf_bound_dir = inf_bound_dir;
    this->sup_bound_dir = sup_bound_dir;
    this->amp_dir = amp_dir;
    this->fct_speed = fct_speed;
    this->const_speed_value = const_speed_value;
    this->timer = Timer(wait_val);

    this->finish = false;
    this->sum_lambda = 0;
    this->i = 0;
    this->oX = 0;
    this->oY = 0;

    this->const_speed = false;
    if(fct_speed == "linear" || const_speed_value > 0){   //ne peut être infini que si la vitesse est constante ou est destiné à être constante (const_speed > 0)
        this->infinite = infinite;
    }
    else{
        this->infinite = false;
    }
    //---------------------------------------------------------
    //
    //                  FONCTION DE VITESSE
    //
    //---------------------------------------------------------

    //----------------------------------------------
    //         FONCTION GAUSSIENNE COMPLETE
    //----------------------------------------------
    if(fct_speed == "smooth"){

        this->fct_speed_step = ((BORNE_GAUSS*2)/this->time);     //On a "nb_frame" valeurs équidistantes dans l'intervale [-BORNE_GAUSS, BORNE_GAUSS]
        this->scale_factor = (distance/sqrt(M_PI))*this->fct_speed_step ; // sqrt(M_PI) est l'integrale de la fonction gaussienne
        this->bound_a = -BORNE_GAUSS;      //on va parcourir l'intervalle [-BORNE_GAUSS , BORNE_GAUSS]
        this->x_fct_speed = -BORNE_GAUSS;
        this->bound_b = BORNE_GAUSS;
    }

    //----------------------------------------------
    //         FONCTION GAUSSIENNE COUPE
    //  La fonction gaussienne mais à partir de 0
    //----------------------------------------------
    if(fct_speed == "dec"){
        this->fct_speed_step = (BORNE_GAUSS/(this->time));     //On "nb_frame" valeurs équidistantes dans l'intervale [0 , BORNE_GAUSS]
        this->scale_factor = this->fct_speed_step*(distance/(sqrt(M_PI)/2));
        this->bound_a = 0.0;      //on va parcourir l'intervalle [0 , BORNE_GAUSS]
        this->x_fct_speed = 0;
        this->bound_b = BORNE_GAUSS;
    }

    //----------------------------------------------
    //         FONCTION GAUSSIENNE CARRE
    //----------------------------------------------
    if(fct_speed == "square"){
        this->fct_speed_step = (2*BORNE_CARRE/this->time);
        double integral = (pow(BORNE_CARRE,3)/3) - (pow(-BORNE_CARRE,3)/3);
        this->scale_factor = (distance/integral)*this->fct_speed_step;
        this->bound_a = -BORNE_CARRE;
        this->x_fct_speed = -BORNE_CARRE;
        this->bound_b = BORNE_CARRE;
    }

    //--------------------------------------------------------
    //                  FONCTION CONSTANTE
    // On supposera que l'ont sera dans l'intervalle [0 , 1]
    //--------------------------------------------------------
    if(fct_speed == "linear"){
        if(this->const_speed_value < 0){
            this->const_speed_value = this->distance/this->time;
        }
        double integral = 1;
        this->fct_speed_step = (1/this->time);
        this->scale_factor = this->fct_speed_step*(distance/integral);
        this->bound_a = 0;
        this->x_fct_speed = 0;
        this->bound_b = 1;
    }

    //---------------------------------------------------------
    //
    //                  FONCTION D'ORIENTATION
    //
    //---------------------------------------------------------

    if(fct_dir == "sinus"){
        this->x_fct_dir = inf_bound_dir;//borne_inf_orientation;
    }

    if(fct_dir == "square"){
        this->x_fct_dir = inf_bound_dir;
    }

}

//-----------OBSLOLETE-----------
//reservé au keyframes gerant les positions
//la position d'origine de la keyframe N est la dernière position de la keyframe N-1
void Keyframe::newOrigin(Keyframe *previous_key){
    /*double dist = previous_key->distance;
    double angle = previous_key->angle;

    this->oX = dist*cos(degToRad(angle)) + previous_key->oX;
    this->oY = dist*sin(degToRad(angle)) + previous_key->oY;*/
}


//Renvoie vrai si l'on a attendu le temps nécessaire.
//dans ce cas en particulier, ce résultat interne du timer doit être connu à l'exterieur de la keyframe
//pour connaître le moment on l'on peut passer à la keyframe suivante
bool Keyframe::wait(){
    this->timer.pass();
    return this->timer.isFinished();
}



bool Keyframe::isSpeedConstante(){
    return this->const_speed;
}


//----------------------------------------------------------------------------------------------------------------------
//                                          nextSpeed
//
// Cette fonction renvoie la nouvelle vitesse de l'objet (lambda) en fonction de la Trajectoire passé en argument
// Elle dépend principalement de la valeur de x_courant_vitesse
//----------------------------------------------------------------------------------------------------------------------
double Keyframe::nextSpeed(){
    double lambda;
    double next_lambda;
    double true_lambda;
    if(!this->finish){
        //----------------------------------------------------
        //       FONCTION GAUSSIENNE (COMPLETE OU COUPE)
        //----------------------------------------------------
        if(this->fct_speed == "smooth" || this->fct_speed == "dec"){
            lambda = exp(-pow(this->x_fct_speed, 2))*this->scale_factor; //de base : l'integrale est en unité d'aire, on doit donc faire "*T.facteur_echelle * T.pas"
        }

        //-------------------------------
        //        FONCTION CARRE
        //-------------------------------
        if(this->fct_speed == "square"){
            lambda = pow(this->x_fct_speed, 2) * this->scale_factor;
        }

        //-------------------------------
        //      FONCTION CONSTANTE
        //-------------------------------
        if(this->fct_speed == "linear"){
            lambda = this->const_speed_value * this->scale_factor;
        }



        this->sum_lambda += lambda;


        //------------------------------------------------------
        // Le vrai lambda selon la fonction d'orientation
        // cf. compte rendu
        //------------------------------------------------------
        if(this->fct_dir == "line"){
            next_lambda = lambda;
        }
        //Si la fonction d'orientation, n'est pas LIGNE, il y a un décalage a effectuer pour que le deplacement se fasse bien sur la distance voulue
        if(this->fct_dir == "sinus"){
            next_lambda = lambda/(cos( atan(this->amp_dir*cos(this->x_fct_dir))));
            //next_lambda = sqrt(pow(lambda, 2) + pow(sin(this->x_fct_dir + lambda) - sin(this->x_fct_dir), 2));
        }

        if(this->fct_dir == "square"){
            next_lambda = lambda/(cos( atan(this->amp_dir*2*this->x_fct_dir) ));
            //next_lambda = sqrt(pow(lambda, 2) + pow(pow(this->x_fct_dir + lambda, 2) - pow(this->x_fct_dir, 2), 2));
            //next_lambda = sqrt(pow(lambda, 2) + pow(this->amp_dir*2*this->x_fct_dir sin(this->x_fct_dir + lambda) - sin(this->x_fct_dir), 2));
        }

        //Le pas dépend de la fonction de vitesse
        float fct_dir_step;
        float integral;
        float abs_length = fabs(this->inf_bound_dir - this->sup_bound_dir);     //longeur formée du segment formé par
                                                                                                    //les deux bornes sur l'abscisse de la fonction d'orientation
        if(this->fct_speed == "smooth"){
            integral = sqrt(M_PI);
            fct_dir_step = (exp(-pow(this->x_fct_speed, 2)))*this->fct_speed_step*( abs_length/integral);
        }
        else if(this->fct_speed == "dec"){
            integral = sqrt(M_PI)/2;
            fct_dir_step = (exp(-pow(this->x_fct_speed, 2)))*this->fct_speed_step*(abs_length/integral);
        }
        else if(this->fct_speed == "square"){
            integral = (pow(BORNE_CARRE,3)/3) - (pow(-BORNE_CARRE,3)/3);
            fct_dir_step = (pow(this->x_fct_speed, 2))*this->fct_speed_step*(abs_length/integral);
        }
        else if(this->fct_speed == "linear"){
            if(this->x_fct_dir < this->sup_bound_dir){    //une fois que l'on est arrivé au bout de la courbe, le projectile continu en ligne droite
               fct_dir_step = this->const_speed_value*this->fct_speed_step*(abs_length/(1*this->const_speed_value));
            }
        }


        this->x_fct_dir += fct_dir_step;

        //Testes pour savoir si la vitesse devient constante
        if(this->fct_speed == "smooth"){
            if(this->const_speed_value > 0 && next_lambda >= this->const_speed_value){    //(dans la première moitié de la courbe:) lorsque la vitesse atteint à this->const_speed...
                this->const_speed = true;
                return this->const_speed_value;          //on continue tout droit
            }
        }
        if(this->fct_speed == "dec"){
            if(this->const_speed_value > 0 && next_lambda < this->const_speed_value){    //(dans la deuxième moitié de la courbe:) lorsque la vitesse atteint à this->const_speed...
                this->const_speed = true;
                return this->const_speed_value;          //on continue tout droit
            }
        }
        if(this->fct_speed == "square"){
            if(this->const_speed_value > 0 && this->x_fct_speed > 0 && next_lambda > this->const_speed_value){
                this->const_speed = true;
                return this->const_speed_value; //on continue tout droit
            }
        }


        this->x_fct_speed += this->fct_speed_step;
        this->i++;


        //si le trajectoire vient de ce terminer, on lance le timer
        if(this->i >= this->time && !this->infinite){
            this->finish = true;
            this->timer.launch();
            return 0;
        }

        //cout << this->sum_lambda << endl;
        next_lambda = next_lambda < 0.05 ? 0 : next_lambda;
        return next_lambda;
    }
    return 0;
}



//----------------------------------------------------------------------------------------------------------------------
//                                          nextDirection
//
// Cette fonction renvoie la nouvelle orientation de l'objet en fonction de la Trajectoire passé en argument
//----------------------------------------------------------------------------------------------------------------------
double Keyframe::nextDirection(){
    if(!this->finish){
        double theta;
        if(this->fct_dir == "sinus"){
            theta = this->angle + radToDeg(atan(this->amp_dir*cos(this->x_fct_dir)));

            /*int lambda = exp(-pow(this->x_fct_speed - this->fct_speed_step, 2)) * T->facteur_echelle;
            theta = this->angle + radToDeg(atan2(this->amp_dir*sin(this->x_fct_dir + lambda) - this->amp_dir*sin(this->x_fct_dir), lambda));*/
        }
        else if(this->fct_dir == "square"){
            theta = this->angle + radToDeg(atan(this->amp_dir*2*this->x_fct_dir));
        }
        else{
            theta = this->angle;
        }

        //Le pas dépend de la fonction de vitesse
        float fct_dir_step;
        float integral;
        float abs_length = fabs(this->inf_bound_dir - this->sup_bound_dir);     //longeur formée du segment formé par
                                                                                                    //les deux bornes sur l'abscisse de la fonction d'orientation
        if(this->fct_speed == "smooth"){
            integral = sqrt(M_PI);
            fct_dir_step = (exp(-pow(this->x_fct_speed, 2)))*this->fct_speed_step*(abs_length/integral);
        }
        else if(this->fct_speed == "dec"){
            integral = sqrt(M_PI)/2;
            fct_dir_step = (exp(-pow(this->x_fct_speed, 2)))*this->fct_speed_step*(abs_length/integral);
        }
        else if(this->fct_speed == "square"){
            integral = (pow(BORNE_CARRE,3)/3) - (pow(-BORNE_CARRE,3)/3);
            fct_dir_step = (pow(this->x_fct_speed, 2))*this->fct_speed_step*(abs_length/integral);
        }
        else if(this->fct_speed == "linear"){
            if(this->x_fct_dir < this->sup_bound_dir){    //une fois que l'on est arrivé au bout de la courbe, le projectile continu en ligne droite
               fct_dir_step = this->const_speed_value*this->fct_speed_step*(abs_length/(1*this->const_speed_value));
            }
        }

        this->x_fct_dir += fct_dir_step;

        this->x_fct_speed += this->fct_speed_step;

        this->i++;


        //si le trajectoire vient de ce terminer, on lance le timer
        if(this->i >= this->time && !this->infinite){
            this->last_theta = theta;
            this->finish = true;
            this->timer.launch();
        }

        //printf("%lf\n", theta);
        return theta;
    }
    return this->last_theta;
}


bool Keyframe::isFinished(){
    return this->finish;
}



//Keyframe utilisant une courbe de bezier
Keyframe::Keyframe(double debut, double fin, double duree, double x1, double y1, double x2, double y2, float wait_val, bool loop)
{
    this->loop = loop;
    this->infinite = false;

    this->debut = debut;
    this->fin = fin;
    this->distance = fin - debut;
    this->pas = 1.0/(timeToFrame(duree));
    this->x_bez = 0;
    this->finish = false;
    this->timer = Timer(wait_val);

    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;

    //cout << debut << endl;
}

Keyframe::Keyframe(double debut, double fin, double duree, string preset, float wait_val, bool loop):
     Keyframe(debut, fin, duree, 0, 0, 0, 0, wait_val, loop)
{
    //cout << debut << endl;
    if(preset == "linear"){
        this->x1 = 0;
        this->y1 = 0;
        this->x2 = 1;
        this->y2 = 1;
    }
    if(preset == "ease"){
        this->x1 = 0.25;
        this->y1 = 0.1;
        this->x2 = 0.25;
        this->y2 = 1;
    }
    if(preset == "ease_in"){
        this->x1 = 0.42;
        this->y1 = 0;
        this->x2 = 1;
        this->y2 = 1;
    }
    if(preset == "ease_out"){
        this->x1 = 0;
        this->y1 = 0;
        this->x2 = 0.58;
        this->y2 = 1;
    }
    if(preset == "ease_in_out"){
        this->x1 = 0.48;
        this->y1 = 0;
        this->x2 = 0.58;
        this->y2 = 1;
    }
    if(preset == "ease_sym"){
        this->x1 = 0.5;
        this->y1 = 0;
        this->x2 = 0.5;
        this->y2 = 1;
    }
}

double Keyframe::nextBezier(){
    double t;
    //double x_bez = this->x_bez;
    //cout << this->finish << endl;
    if(!this->finish){
        double lower = 0;
        double upper = 1;
        t = (upper + lower) / 2.0;



        float xTolerance = 0.001;

        //x initial
        double x1 = this->x1;
        double x2 = this->x2;

        double x_en_cours = 3*x1*t*pow(1 - t, 2) + 3*x2*pow(t, 2)*(1 - t) + pow(t, 3);

        //dichotomie pour trouver le bon t
        while(fabs(this->x_bez - x_en_cours) > xTolerance){
            if(this->x_bez > x_en_cours){
                lower = t;
            }
            else{
                upper = t;
            }

            t = (upper + lower) / 2;
            x_en_cours = 3*this->x1*t*pow(1 - t, 2) + 3*this->x2*pow(t, 2)*(1 - t) + pow(t, 3);

        }
        double y_bez = 3*this->y1*t*pow(1 - t, 2) + 3*this->y2*pow(t, 2)*(1 - t) + pow(t, 3);




        //cout << x_bez << endl;
        if(this->x_bez >= 0.999){
            this->finish = true;
            this->timer.launch();
        }


        this->x_bez += this->pas;

        if(this->x_bez >= 0.999){
            this->x_bez = 1;
        }

        //if(!this->finish)
            //this->x_bez += this->pas;

        return this->debut + this->distance*y_bez;
    }
    return this->fin;
}


Keyframe::~Keyframe()
{
    //dtor
}
