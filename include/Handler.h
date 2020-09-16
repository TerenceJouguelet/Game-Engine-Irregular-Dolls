#ifndef HANDLER_H
#define HANDLER_H

#include <list>
#include <typeinfo>
#include <fstream>

#include "Movable.h"
#include "AnimationStat.h"
#include "VarStat.h"
#include "Sound.h"

using namespace std;

//template<typename AbleToMove>
class Handler : public Movator
{
    protected:
        list<Movable*> components;  //les éléments du handler


        bool unused;    //vrai si le handler n'a pas encore créé d'éléments


        //l'origine des origines (méta)
        double ooX;
        double ooY;

        //séquence contenant les positions de ces origines
        AnimationStat a_O_theta;
        AnimationStat a_O_lambda;
        //ces position
        VarStat v_O_theta;
        VarStat v_O_lambda;

        AutoMovable *origin;
        bool mobile_origin;
        bool origin_on_player;
        int x_origin_offset, y_origin_offset;


        string type;

        AnimationStat A_color;
        VarStat color;

        string arrange_mode;    //"ring", "wall", ...
        int rows, columns;
        int init_rows;
        bool row_increment;
        double min_speed_scale; //pour une colonne

        //décalage sur les lignes
        AnimationStat A_offset_angle;
        VarStat offset_angle;

        //décalage sur les colonnes
        AnimationStat A_offset_columns;;
        VarStat offset_columns;

        //decalage d'apparition
        AnimationStat A_spawn_offset;
        VarStat spawn_offset;

        //le facteur de vitesse pour une ligne
        AnimationStat A_row_speed_scale;
        VarStat row_speed_scale;


        //l'entendu des éléments sur une colonne tout au long du déplacement des entité
        //valable uniquement en mode "position relative"
        AnimationStat A_spread;
        VarStat spread;

        bool aya_mode;  //pour faire des cercles, les éléments d'une ligne ont la même orientation
        bool sanae_mode; //pour faire deq étoiles comme Sanae, la trajectoire des élements d'une salve commence à l'étape où en est celle de la première salve
        int num_pulse;    //on doit donc conserver le numero de la salve courrante

        int cadence;    //la cadence de création d'une salve
        int wave_cadence;    //la cadence de création d'une wave(série). Une wave est composé de plusieurs salves
        int wave_time; //la durée d'une wave
        int n_wave; //le nombre de wave
        int i_wave; //numero de la wave en cours
        bool in_a_wave;   //vrai si le hander est entrain de jouer une wave
        bool reset_wave;

        unsigned int time;  //temps écoulé depuis le lancement

        Sound spawn_sound;  //le sons joué lors de la création d'une salve


        bool immortal; //vrai si le pattern est infini (par exemple le pattern représentant le tir du joueur)
        bool def_spread;

        bool detached;
        bool all_element_connected;

    public:
        double out_margin;

    public:
        Handler(string id, string path_text_file);

        virtual void UpdateSequences() override;
        void UpdateValues() override;

        void adaptOrigin(double X, double Y);
        void createElements(Player &player);
        virtual void moveElements(Player &player);
        void drawElements(bool with_animation);

        void changeMoveSegmentOfElements(int segment_index);
        void reverseTrajectoryOfElements();

        void resetTime();

        void use(); //pire nom de méthode de l'histoire
        bool isUsed();
        bool isFinish();
        virtual ~Handler();


        void assignOneGroupOfKey() override;

        protected : virtual void assignAttributsWithTextFile() override;

    private:

        //virtual void assignOtherValuesForKeys() override;    //pour assigner les "Path Values"

       /* void assignAbsolutePosition(string markup_name);
        void assignRelativePosition(string markup_name);*/

        void assignWithLoop(string markup_name, Handler *handler);
        void initAttribut(); //Pour initialiser les valeurs par défaut des attributs

        //void assignSpreadValues();
        //void assignSheetAndHitBox(string type);

        virtual Movable* addComponent(double oX, double oY, double angle_base, double speed_scale)=0;

    protected:
        virtual bool aliveStatue(Movable*);

};

#endif // HANDLER_H
