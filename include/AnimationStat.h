//--------------------------------------------------------------------------------
//                              AnimationStat
//
//
//  Permettant de gérer une séquence d'animation prédéfinie pour une statistique
//  donnée (position, opacité...) à l'aide de Keyframes
//--------------------------------------------------------------------------------


#ifndef ANIMATIONSTAT_H
#define ANIMATIONSTAT_H

#include <list>
#include <iterator>
#include <string>
#include <tuple>

#include "Keyframe.h"

using namespace std;

class AnimationStat
{
    private:
        list<tuple<double,int,bool>> sequence;  //1er: valeur, 2eme: numero de la keyframe
        list<Keyframe> keyframes;
        list<Keyframe>::iterator current_key;
        /*list<bool> kryloop;
        list<bool>::iterator current_key;*/
        int size;
        int key_index; //numero de la keyframe en cours

        string type;

    public:
        AnimationStat(int id, string type);  //identifiant de l'animation
        void changeType(string real_type);
        void addKeyframe(Keyframe Key);
        void eraseAllKeyFrame();
        int getSize();
        void update();
        bool isCurrentKeyInfiniteOrDoesntExist();
        bool isCurrentKeyInfiniteOrDoesntExistOrIsntFisrt();
        //void fillSequence();
        list<tuple<double,int,bool>>::iterator getIterSequence();
        list<tuple<double,int,bool>>::iterator getIterSequence(int index);
        list<tuple<double,int,bool>>::iterator getIterSequence(bool all_update);
        virtual ~AnimationStat();
};

#endif // ANIMATIONSTAT_H
