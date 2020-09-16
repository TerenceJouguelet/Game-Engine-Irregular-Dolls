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
        list<tuple<double,int,bool>>::iterator it_value;//si les valeurs sont pr�d�finis, on les lira avec cet it�rateur
        list<tuple<double,int,bool>>::iterator it_begin;//on conserve le debut, n�cessaire si l'on veut boucl� l'animation
        //list<double>::iterator it_end; //la fin (pour faire le test d'arr�t)
        list<tuple<double,int,bool>>::iterator it_end;  //quand on aura trouv� la fin, on la stockera ici

        vector<list<tuple<double,int,bool>>::iterator> segments_begin;  //le debut de chaque segments

        bool constant;  //true si cette stat est constante
        bool main_loop;   //vrai si l'on veut que l'animation enti�re boucle
        bool null;  //vrai lorsque l'on initialise la VarStat avec le constructeur par defaut (aucun type n'a �t� affect�)

        bool random;    //vrai si la stat varie al�atoirement
        float inf_bound;    //les bornes
        float sup_bound;    //

        bool key_just_changed;  //vrai si l'on vient de changer de "partie"(segment) dans la s�quence
        //(je dis pas keyframe car une VarStat n'est pas cens�e savoir ce que c'est [Encapsulation])

        bool reversed;

        int target_segment; //le segment cibl� par le changement
        bool segment_is_changing;   //vrai si le segment est entrain de changer

        bool end;

    protected:
        double value; //LA VALEUR
    public:
        VarStat();
        VarStat(list<tuple<double,int,bool>>::iterator it_begin, bool main_loop);
        VarStat(double constant);
        VarStat(float inf_bound, float sup_bound);
        bool isNull();   //vrai si aucun type n'a �tait affect�
        bool isConstant();

        virtual double newValue();
        double getValue();  //si l'on veut simplement la valeur sans avencer dans l'it�rateur

        void changeSegment(int segment_index);

        void setValue(double value);
        bool isKeyChanged();    //cf. "key_just_changed" au dessus:
        int getCurrentSegemnt();
        bool isLastValue();    //vrai si on a atteint la derni�re valeur
        void advenceUntilNextKey();
        void reset();   //on repart au d�but
        void reverse();   //on parcours l'it�rateur � l'envers

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
