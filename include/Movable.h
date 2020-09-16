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
        bool absolute_position; //vrai si les positions des �l�ments sont d�fini de fa�on absolu

        double oX_trajectory;   //origines de la trajectoire
        double oY_trajectory;
        bool def_trajectory_origin;

        double margin_out;

    protected:
        bool loop_trajectory;   //vrai si l'on veut que la trajectoire enti�re boucle
        int n_key;//le nombre de keyframes associ�es
        Sound other_path_sound; //le sons qui sera jou� au moment ou un �l�ment du handler changera de trajectoire (Keyframe)

    protected:

        //[position absolue] : les positions des �l�ments seront calcul�es � partir d'un vecteur de d�placement, vecteur que chaque instance
        //de la classe Movable poss�de. Chaque Movable calcule donc � chaque nouvelle frame la nouvelle valeur de son vecteur, ind�pendamment du Handler
        //lambdaS contient les distances que vont parcourir l'�l�ment � chaque frame (taille du vecteur)
        //thetaS contient les directions que vont empreinter l'�l�ment � chaque frame (orientation du vecteur)
        //
        //[position relative] : les positions des �l�ments seront calcul�es � partir de l'origine du Handler
        //lambdaS contient les distances entre l'�l�ment et le l'origine du Handler pour chaque frame
        //thetaS contient les angles entre l'�l�ment et le l'origine du Handler pour chaque frame

        AnimationStat thetaS;
        AnimationStat lambdaS;

        VarStat r_theta;
        VarStat r_lambda;

        //**************************************************************************************************//
        //Path Values :
        //La plupart des caract�ristiques (celles qui sont bien sp�cifiques) des entit�s sont d�finis
        //lors de leur cr�ation. A partir de la deuxi�me keyframe, un Movable ne peut changer ces caract�riqriques
        //qu'a travers des it�rateurs. Par exemple, les deux it�rateurs sur les deux s�quences pr�c�dentes
        //(coord polaires)
        //On doit donc cr�er d'autres s�quence si l'on veut modifier d'autre caract�ristiques. A la cr�ation du Movable,
        //on passera en argument les it�rateur sur ces s�quence. Chaque �l�ments de ces s�quences correspond � une keyframe.
        //Le contenu de ces s�quence est d�t�ermin� � l'aide de la balise CHG: dans le fichier ecl.
        //On utilisera la m�thode adaptToKeyframe() pour effectuer ces modifications

        vector<bool> same_dir;    //vrai si l'on veut que les �l�ments aient la m�me orientation (pour chaque keyframes)
        vector<bool> same_speed;  //vrai si l'on veut que les �l�mnets aient la m�me vitesee (pour chaque keyframes)
        protected:vector<bool> aimed;  //vrai si les �l�ments sont dirig�s vers le joueur (pour chaque keyframes)

        private:
        vector<bool> bounce;  //vrai si les �l�ments rebondissent sur le mur (pour chaque keyframes)
        //en v�rit�, si la valeur est vrai, cela veut dire que la prochaine keyframes se jouera au moment au l'on touchera un mur
        //autrement dit, la derin�re valeur de bounce sera forcement "false"
        //si le veut un vrai effet de rebond, il faudra mettre l'angle de base de la prochaine Keyframe � 0

        //Sound other_path_sound; //le sons qui sera jou� au moment ou un �l�ment du handler changera de trajectoire (Keyframe)

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

        bool out; //vrai si l'entit� est invoqu�e hors de la zone de jeu

        double oX;  // l'origine
        double oY;  //

        protected: double base_angle;  //le direction g�n�rale
        double init_base_angle; //puisque base_angle peut changer, on stockera sa valeur initiale ici
        double aimed_angle; //en direction du joueur

        double speed_scale; //le facteur vitesse (normalement < 1)
        double init_speed_scale; //idem


        bool absolute_position; //cf. Handler.h
        VarStat lambda;             //ses position polaire
        protected:VarStat theta;    //

        Vector2D Vect;   //le vecteur de d�placement

        vector<bool>::iterator same_dir;
        vector<bool>::iterator same_speed;
        vector<bool>::iterator aimed;
        vector<bool>::iterator bounce;

        bool frozen;  //si vrai, l'objet sera immobilis�

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
        bool isTrajectoryEnd(); //vrai si la trajectoire (compl�te) est termin�e
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
        void stopMovement();    //pour immobilis� l'entit�

    private:
        bool isTouchingWall();
        void replaceIfOut();
        double newAngleAfterWallCollision();

};



//objet d�pla�able autonome
class AutoMovable : public Movable, virtual public Movator{
    public:
        AutoMovable(double X, double Y, string id, string path_text_file);
        void changeTransformation(string id, string path_text_file);
        void changeTrajectory(string id, string path_text_file, bool old_origin = 0);
        void Move(Player *player) override;
        void Draw(bool with_animation) override;

};


#endif
