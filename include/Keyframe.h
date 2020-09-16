//--------------------------------------------------------------------------------
//                                    Keyframe
//
//
//  Une étape dans l'animation
//--------------------------------------------------------------------------------

#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <string>
#include "Timer.h"

using namespace std;

class Keyframe
{
    private:
        double distance;
        bool loop;

        //attribut pour les keyframes de type bezier
        double debut;
        double fin;
        double x1, x2, y1, y2;    //definisant une courbre de bezier d'ordre 3

        double pas;
        double x_bez;



        //attribut pour les keyframes utilisant des fonction mathématiques (position)
        double oX, oY;

        double time;
        double angle;
        string fct_dir;
        float inf_bound_dir;
        float sup_bound_dir;
        float amp_dir;
        double x_fct_dir;   //le x courant dans la fonction d'orientation
        double bound_a;  //borne inf de la fonction de vitesse
        double bound_b;  //borne sup de la fonction de vitesse
        double last_theta;   //la valeur de l'angle lorsque la trajectoire est terminé sera stockée ici

        string fct_speed;
        float fct_speed_step;      // On avencera dans la fonction de vitesse avec ce pas
        double x_fct_speed;   //le x courant dans la fonction de viteese
        double scale_factor;    //facteur echelle
        double sum_lambda;

        int i; //numero frame
        bool infinite;  //true si l'on veut que la trajectoire ne s'arrête pas
        float const_speed_value; //dans ce cas, on pourra définir une valeur constante de vitesse pour l'élément
        bool const_speed;   //lorsque l'élément aura atteint cette vitesse, on mettra ce booléen à vrai

        bool finish;    //true si la dernière valeur est atteinte

        Timer timer;    //le timer pour gérer le temps d'attente de déclancher de la prochaine keyframe

    public:
        //Keyframe(double debut, double fin, double duree, double x1, double y1, double x2, double y2);
        Keyframe();
        Keyframe(double debut, double fin, double duree, double x1, double y1, double x2, double y2, float wait_val, bool loop);
        Keyframe(double debut, double fin, double duree, string preset, float wait_val, bool loop);

        Keyframe(double distance, double duree, double angle, string fct_dir, float inf_bound_dir,
                              float sup_bound_dir, float amp_dir, string fct_speed, float const_speed_value, bool infinite, float wait_val, bool loop);
        double nextSpeed();
        double nextDirection();
        double nextBezier();

        void newOrigin(Keyframe *previous_key);
        bool isFinished();
        bool isInfinite();
        bool isLooping();
        bool isSpeedConstante();

        bool wait();

        virtual ~Keyframe();

};

#endif // KEYFRAME_H
