#ifndef MOVABLE_H
#define MOVABLE_H

#include <SDL2/SDL_image.h>
#include <map>
#include <string>
#include <list>
#include <vector>

#include "VarStat.h"
#include "Sheet.h"
#include "Transformable.h"
#include "Collisionable.h"
#include "Player.h"
#include "Vector2D.h"
#include "Sound.h"


using namespace std;


class Movator: public Transformator{

    public:
        bool absolute_position; //vrai si les positions des éléments sont défini de façon absolu

        double oX_trajectory;   //origines de la trajectoire
        double oY_trajectory;
        bool def_trajectory_origin;

        double margin_out;

    protected:
        bool loop_trajectory;   //vrai si l'on veut que la trajectoire entière boucle
        int n_key;//le nombre de keyframes associées
        Sound other_path_sound; //le sons qui sera joué au moment ou un élément du handler changera de trajectoire (Keyframe)

    protected:

        //[position absolue] : les positions des éléments seront calculées à partir d'un vecteur de déplacement, vecteur que chaque instance
        //de la classe Movable possède. Chaque Movable calcule donc à chaque nouvelle frame la nouvelle valeur de son vecteur, indépendamment du Handler
        //lambdaS contient les distances que vont parcourir l'élément à chaque frame (taille du vecteur)
        //thetaS contient les directions que vont empreinter l'élément à chaque frame (orientation du vecteur)
        //
        //[position relative] : les positions des éléments seront calculées à partir de l'origine du Handler
        //lambdaS contient les distances entre l'élément et le l'origine du Handler pour chaque frame
        //thetaS contient les angles entre l'élément et le l'origine du Handler pour chaque frame

        AnimationStat thetaS;
        AnimationStat lambdaS;

        VarStat r_theta;
        VarStat r_lambda;

        //**************************************************************************************************//
        //Path Values :
        //La plupart des caractéristiques (celles qui sont bien spécifiques) des entités sont définis
        //lors de leur création. A partir de la deuxième keyframe, un Movable ne peut changer ces caractériqriques
        //qu'a travers des itérateurs. Par exemple, les deux itérateurs sur les deux séquences précédentes
        //(coord polaires)
        //On doit donc créer d'autres séquence si l'on veut modifier d'autre caractéristiques. A la création du Movable,
        //on passera en argument les itérateur sur ces séquence. Chaque éléments de ces séquences correspond à une keyframe.
        //Le contenu de ces séquence est détéerminé à l'aide de la balise CHG: dans le fichier ecl.
        //On utilisera la méthode adaptToKeyframe() pour effectuer ces modifications

        vector<bool> same_dir;    //vrai si l'on veut que les éléments aient la même orientation (pour chaque keyframes)
        vector<bool> same_speed;  //vrai si l'on veut que les élémnets aient la même vitesee (pour chaque keyframes)
        protected:vector<bool> aimed;  //vrai si les éléments sont dirigés vers le joueur (pour chaque keyframes)

        private:
        vector<bool> bounce;  //vrai si les éléments rebondissent sur le mur (pour chaque keyframes)
        //en vérité, si la valeur est vrai, cela veut dire que la prochaine keyframes se jouera au moment au l'on touchera un mur
        //autrement dit, la derinère valeur de bounce sera forcement "false"
        //si le veut un vrai effet de rebond, il faudra mettre l'angle de base de la prochaine Keyframe à 0

        //Sound other_path_sound; //le sons qui sera joué au moment ou un élément du handler changera de trajectoire (Keyframe)

    protected:
        virtual void assignAttributsWithTextFile() override;
        virtual void assignOneGroupOfKey() override;

    public:
        //Movator();
        Movator(string id, string path_text_file);
        VarStat getSystVarLambda();
        VarStat getSystVarTheta();

        vector<bool>::iterator getSameDirIter();
        vector<bool>::iterator getSameSpeedIter();
        vector<bool>::iterator getAimedIter();
        vector<bool>::iterator getBounceIter();


        virtual void UpdateSequences() override;
        virtual void UpdateValues();
};


class Movable: public Transformable{

    protected:
        void initialiseMovable(Movator *movator);

    private:

        bool out; //vrai si l'entité est invoquée hors de la zone de jeu

        double oX;  // l'origine
        double oY;  //

        protected: double base_angle;  //le direction générale
        double init_base_angle; //puisque base_angle peut changer, on stockera sa valeur initiale ici
        double aimed_angle; //en direction du joueur

        double speed_scale; //le facteur vitesse (normalement < 1)
        double init_speed_scale; //idem


        bool absolute_position; //cf. Handler.h
        VarStat lambda;             //ses position polaire
        protected:VarStat theta;    //

        Vector2D Vect;   //le vecteur de déplacement

        vector<bool>::iterator same_dir;
        vector<bool>::iterator same_speed;
        vector<bool>::iterator aimed;
        vector<bool>::iterator bounce;

        bool frozen;  //si vrai, l'objet sera immobilisé

        //static map<string, Sheet> bank_images; //La banque d'images necessaire pour les instances

        bool path_just_changed;

    public:
        double margin_out;

        Movable();
        Movable(double oX, double oY, double angle_base, double speed_scale, Movator *movator);


        void fabricateWith(Transformator *transformator) override;

        void setPositionType(bool absolute_position);
        virtual void Move(Player *player);

        void adaptOrigin(double X, double Y);
        void setMirrorOrigin(bool flip_to_left);
        void changeAngle(double angle);

        bool isPathChanged();
        bool isTrajectoryEnd(); //vrai si la trajectoire (complète) est terminée
        //virtual void replayTransform()=0;

        void changeMoveSegment(int segment_index);
        void reverseMoveSegment();

        bool isOut();
        bool isOutScreen();

        double getX();
        double getY();


        ~Movable();

    protected:
        virtual void adaptToKeyframe(Player *player); //certain attribut peuvent changer selon la keyframe en cours (cf. handler.h)

        virtual void updateValues();
        void stopMovement();    //pour immobilisé l'entité

    private:
        bool isTouchingWall();
        void replaceIfOut();
        double newAngleAfterWallCollision();

};



//objet déplaçable autonome
class AutoMovable : public Movable, virtual public Movator{
    public:
        AutoMovable(double X, double Y, string id, string path_text_file);
        void changeTransformation(string id, string path_text_file);
        void changeTrajectory(string id, string path_text_file, bool old_origin = 0);
        void Move(Player *player) override;
        void Draw(bool with_animation) override;

};


#endif
