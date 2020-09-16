#ifndef IMAGE_H
#define IMAGE_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <map>
#include "AnimSystem.h"
#include "HitBox.h"
#include "SDL_FontCache.h"


class Sheet
{
    private:
        SDL_Texture* texture_sheet; //La texture
        std::vector<std::vector<SDL_Rect>> clips; //chaque clip correspond à un élément

        int W_Elements;  //nombre d'élement sur la longueur
        int H_Elements;  //nombre d'élement sur la hauteur

        int W_entity; //largeur d'une entité
        int H_entity; //hauteur d'une entité

        int x_gap_size; //espace entre les éléments sur une ligne
        int y_gap_size; //espace entre les éléments sur une colonne

        int nb_color;   //le nombre de couleur disponible
        std::vector<int> statuts;   //les statuts (indices) avec leur nombre de frame (valeurs)

        std::string name_image;   //le nom de la feuille
        bool directional; //vrai si c'est un sprite directionel (change de forme après rotation)
        bool colors_in_column;  //vrai si les autres palettes de colour pour un même sprite sont réparties en colonnes

        public:
            bool is_define_globally;    //vi si la feuille est défini de façon glabal;
        public:
            static map<string, Sheet*> global_sheets;  //la map contenant ces feuilles globales
            static void initializeGlobalsSheets();


    public:
        //Sheet();
        Sheet(Sheet const& cpy);
        Sheet(string name_image);
        static Sheet* newSheet(string name_image);  //constructeur statique (feuille normal)
        static Sheet* newText(string text);  //constructeur statique (texte)
        void deleteSheet();
        SDL_Rect* getReferenceOfClip(int frame, int statue, int color);  //pour renvoyer l'adresse du clip "num_clip"
        void updateRect(SDL_Rect* rect, double X, double Y) const;   //on preferera demender à la feuille de modifier le rect
        void blitImage(SDL_Rect* rect, SDL_Rect* clip);   //on affiche le sprite à partir de la feuille
        void blitTransformedImage(SDL_Rect* rect, SDL_Rect* clip, double angle, double scale, double alpha, bool flip);    //on affiche le sprite transformé à partir de la feuille
        AnimSystem* createAnimCorresponding();

        static HitBox* createHitBoxBySheetName(string sheet_name);
        HitBox* createHitBoxCorresponding();
        string getSheetName();

        virtual ~Sheet();

    private:
        void initInfo(SDL_Surface *surf_sheet, int W_Elements, int x_gap_size, int H_Elements, int y_gap_size, vector<int>& statuts, int nb_color);
        void initSheet(string name_image);
};



class LifeBar{
    private:
        /*
        int x_pos;  //position du début de la barre
        int y_pos;  //
        int thickness;  //épaisseur
        */

        SDL_Rect bar;

        int max_width;  //la largeur quand la barre est pleine
        int max_value;  //la valeur (de PV) qui correspond à la barre pleine


        //couleur
        int R, G, B;

    public:
        LifeBar(int max_value, string color);
        void Draw(int current_value);

};





#endif // IMAGE_H
