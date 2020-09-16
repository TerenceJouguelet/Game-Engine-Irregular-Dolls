#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H

#include "Drawable.h"
#include "VarStat.h"
#include "AnimationStat.h"
#include "Timer.h"
#include "AssignFileSystem.h"

using namespace std;


//class géreant les transfomration
class Transformator{
    private:
        AnimationStat a_scale;
        AnimationStat a_angle;
        AnimationStat a_alpha;

        AnimationStat a_shake_x;
        AnimationStat a_shake_y;

        //VarStat destinées à être renvoyées pour être exploités par un transfomrable
        VarStat r_scale;
        VarStat r_angle;
        VarStat r_alpha;

        VarStat r_shake_x;
        VarStat r_shake_y;

        bool transformation_def_by_step;
        int number_of_def_trans_key;

        vector<Sheet*> sheets;  //les feuilles de sprite utilisés (pour chaque keyframes)
        bool def_main_type; //vrai si l'on utilise la même feuille pour toute les keyframes

    protected:
        AssignFileSystem *file_assigner;
        string path_text_file;
        int number_of_def_key;
        vector<int> colors;  //les couleur utilisés (pour chaque keyframes)

    public:
        string id;
        float duration; //la durée de la transformation
        bool loop_transformation;
        float shake_duration;
        bool r_flip;
        bool alternate;
        int shake_cadence;

        int x_offset;
        int y_offset;

    public:
        Transformator();
        Transformator(string preset, string sheet_name, int color);
        Transformator(string id, string path_text_file);
        VarStat getSystVarScale();
        VarStat getSystVarAngle();
        VarStat getSystVarAlpha();
        VarStat getSystVarShakeX();
        VarStat getSystVarShakeY();


        vector<Sheet*>::iterator getSheetsIter();
        vector<int> getColors();
        vector<int>::iterator getColorsIter();

        virtual void UpdateSequences();
        virtual void assignOneGroupOfKey();

        ~Transformator();

    protected:
        virtual void assignAttributsWithTextFile();
        void initWithPreset(string preset);

    private:
        void assignSheetAndHitBox(string type);
};

//objet transfromable ayant des besoin des valeurs du transformateur
class Transformable : public Drawable
{
    private:
        bool active; //vrai si la transformation est active
        bool at_cancel; //vrai si la transformation est entrain de s'annuler
        Timer cancel_time; //temps pour que la transformation s'annule

        protected:VarStat scale;
        protected:VarStat angle;
        VarStat alpha;

        bool flip;


        vector<Sheet*>::iterator it_sheets;
        vector<int> colors;
        vector<int>::iterator it_colors;



        bool shaking;   //vrai si l'image tremble
        Timer shake_delay;    //le temps de temblement
        VarStat shake_factor_x;   //le facteur de tremblement (puissance)
        VarStat shake_factor_y;   //le facteur de tremblement (puissance)
        bool alternate;
        int shake_cadence;
        bool alter_val;



        Timer transform_delay; //le temps qu'il faut attendre avant de stopper la transformation
        bool finish; //vrai si elle est finie, attention : une transformation est terminée ssi le delay est atteint
        bool loop; //vrai si la transforamtion boucle

        void setSystVar(VarStat scale, VarStat angle, VarStat alpha, float duration, bool loop_transformation);

        //taille de la Bounding Box
        int width_BB;
        int height_BB;
        protected:
            int maximum_size;   //lorsqu'il y a rotation: c'est la longueur et hauteur maximum que peut avoir l'entité
            void newBoundingBoxSize();


    public:
        //les deux type de constructeur (cf. Drawable.h)
        Transformable(double X, double Y, Sheet *sheet, int color);
        Transformable(double X, double Y, string sheet_name, int color);
        Transformable(double X, double Y, Transformator *transformator);

        virtual void fabricateWith(Transformator *transformator);
        void changeAllColor(int new_color);

        void launch();
        bool isActive();
        bool isTransformationFinish();
        void stop();
        void cancel();
        virtual void updateTransformValue();
        void adaptAngle(double angle);
        void reposition(double X, double Y);

        void changeTransformSegment(int segment_index);
        void reverseTransformSegment();

        void shake();

        void setFlip(bool new_flip);
        //void changeColor(int new_color) override;

        virtual void Draw(bool with_animation) override;
        //virtual ~Transformable();

    protected:
        void initDrawElements();
        void shakeEffect();

};


//objet transfromable autonome
class AutoTransformable : public Transformable, virtual public Transformator{
    public:
        AutoTransformable(double X, double Y, string sheet_name, int color, string preset);
        AutoTransformable(double X, double Y, string sheet_name, int color, string id, string path_text_file);
        //void UpdateSequences() override;
        void Draw(bool with_animation) override;
};


class IncorrectPresetOfTransformationException{};

#endif // TRANSFORMABLE_H
