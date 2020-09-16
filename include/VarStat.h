#ifndef VARSTAT_H
#define VARSTAT_H

#include <list>
#include <vector>
#include <iterator>
#include <iostream>
#include <tuple>

using namespace std;

class VarStat
{
    private:
        //int n_key;  //le nombre de Keyframes
        list<tuple<double,int,bool>>::iterator it_value;//si les valeurs sont prédéfinis, on les lira avec cet itérateur
        list<tuple<double,int,bool>>::iterator it_begin;//on conserve le debut, nécessaire si l'on veut bouclé l'animation
        //list<double>::iterator it_end; //la fin (pour faire le test d'arrêt)
        list<tuple<double,int,bool>>::iterator it_end;  //quand on aura trouvé la fin, on la stockera ici

        vector<list<tuple<double,int,bool>>::iterator> segments_begin;  //le debut de chaque segments

        bool constant;  //true si cette stat est constante
        bool main_loop;   //vrai si l'on veut que l'animation entière boucle
        bool null;  //vrai lorsque l'on initialise la VarStat avec le constructeur par defaut (aucun type n'a été affecté)

        bool random;    //vrai si la stat varie aléatoirement
        float inf_bound;    //les bornes
        float sup_bound;    //

        bool key_just_changed;  //vrai si l'on vient de changer de "partie"(segment) dans la séquence
        //(je dis pas keyframe car une VarStat n'est pas censée savoir ce que c'est [Encapsulation])

        bool reversed;

        int target_segment; //le segment ciblé par le changement
        bool segment_is_changing;   //vrai si le segment est entrain de changer

        bool end;

    protected:
        double value; //LA VALEUR
    public:
        VarStat();
        VarStat(list<tuple<double,int,bool>>::iterator it_begin, bool main_loop);
        VarStat(double constant);
        VarStat(float inf_bound, float sup_bound);
        bool isNull();   //vrai si aucun type n'a était affecté
        bool isConstant();

        virtual double newValue();
        double getValue();  //si l'on veut simplement la valeur sans avencer dans l'itérateur

        void changeSegment(int segment_index);

        void setValue(double value);
        bool isKeyChanged();    //cf. "key_just_changed" au dessus:
        int getCurrentSegemnt();
        bool isLastValue();    //vrai si on a atteint la dernière valeur
        void advenceUntilNextKey();
        void reset();   //on repart au début
        void reverse();   //on parcours l'itérateur à l'envers

        virtual ~VarStat();

    private:
        bool searchAndAssignBeginOfSegement(int segment_index);
        void searchAndAssignEnd();
};


//****************************************************************************************************//

/*class SegmentedVarStat, public VarStat{
    private:
        vector<list<tuple<double,int,bool>>::iterator> segments_value;
        vector<list<tuple<double,int,bool>>::iterator> segments_begin;
};*/

#endif // VARSTAT_H
