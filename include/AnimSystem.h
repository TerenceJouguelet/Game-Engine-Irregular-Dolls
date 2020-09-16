#ifndef ANIMSYSTEM_H
#define ANIMSYSTEM_H

#include <vector>
#include <SDL2/SDL.h>
#include "Vector2D.h"

using namespace std;

class AnimSystem
{
    private:
        vector<int> statuts;
        int statut;
        int frame;
        //int nb_frame;   //contient le nombre de frame (on suppose qu'il est identique pour chaque statut)
        int delay;  //le delai entre chaque frame
        int i_delay;    // pour effectuer le parcours
        bool reverse;   //vrai si l'on veut jouer l'animation à l'envers
        bool loop; //vrai si l'animation boucle
        bool finish;

        bool from_right;
    public:
        AnimSystem(vector<int> &statuts, double delay, bool loop);
        void lauch();
        bool isFinised();
        void changeStatut(const Uint8 *keystates);
        void changeStatut(Vector2D &V);
        int nextFrame();
        int getStatut();
        bool moveFromRight();
        virtual ~AnimSystem();
};

#endif // ANIMSYSTEM_H
