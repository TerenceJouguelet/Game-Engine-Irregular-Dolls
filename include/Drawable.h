#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <SDL2/SDL.h>
#include "Sheet.h"
#include "AnimSystem.h"
#include <string>
#include "Positionable.h"

using namespace std;

class Drawable : public Positionable{

    protected:
        /*double X;   //ses position carthésiennes
        double Y;   //*/

        int x_init_draw_offset;
        int y_init_draw_offset;

        int x_draw_offset;
        int y_draw_offset;

        public:int color; //parfois, les feuille peuvent contenir plusieurs sprite qui n'on de différents que la couleur
        AnimSystem* animation;//le system d'animation pour animer le sprite

        SDL_Rect rect; //Le rectangle représentant la surface

        SDL_Rect *clip;  //clip correspondant à la partie de la feuille que l'on veut blit
        int current_frame;

        Sheet* current_sheet;
        string role;
        public :string sheet_name;//son nom (pour pouvoir utiliser le constructeur de copie)

    public:
        Drawable(double X, double Y, Sheet *sheet, int color);
        Drawable(double X, double Y, string sheet_name, int color);
        Drawable(Drawable const& copy_drawable);

        void setDrawOffset(int x, int y);
        void setInitDrawOffset(int x, int y);
        void backToInitDrawOffset();

        void playAnimation();
        bool isAnimationFinish();

        void setSheet(string sheet_name);
        virtual void changeCurrentColor(int new_color);

        virtual void Draw(bool with_animation);    //on l'afficher à l'écran
        virtual ~Drawable();

    protected:
        virtual SDL_Rect* getClip(int color, int frame, int statut);
        virtual void newRectPosition();    //pour calculer les nouvelle position de "rect"

    protected:
        bool hasAnimSystem();
        virtual void addAnimSystem();

    protected:
        virtual void drawFrontEffects(bool with_animation);
        virtual void drawBackEffects(bool with_animation);
};

/*
class DrawableInHandler : public Drawable{
    protected:
        Sheet* current_sheet;

    public:
        DrawableInHandler(double X, double Y, Sheet *sheet, int color);
        DrawableInHandler(DrawableInHandler const& copy_drawable);
        virtual void Draw() override;   //on l'afficher à l'écran
        virtual ~DrawableInHandler();

    private:
        virtual SDL_Rect* getClip(int color, int frame, int statut) override;
        void newRectPosition() override;    //pour calculer les nouvelle position de "rect"

    protected:
        void addAnimSystem() override;
};


class DrawableAlone : public Drawable{
    protected:
        Sheet sheet;
        string sheet_name;//son nom (pour pouvoir utiliser le constructeur de copie)

    public:
        DrawableAlone(double X, double Y, string sheet_name, int color);
        DrawableAlone(DrawableAlone const& copy_drawable);
        virtual void Draw() override;    //on l'afficher à l'écran
        virtual ~DrawableAlone();

    private:
        virtual SDL_Rect* getClip(int color, int frame, int statut) override;
        void newRectPosition() override;   //pour calculer les nouvelle position de "rect"

    protected:
        void addAnimSystem() override;
};
*/
#endif // DRAWABLE_H
