#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <iostream>
#include <stack>
#include <list>
#include "Sound.h"

class Pattern;
class GameState;
class AssignFileSystem;

using namespace std;

class Game
{
    public:
        static SDL_Renderer *renderer;
        static Music *main_music;

        //facteurs de zoom et offset pour le mode fullscreen
        static float X_SCALE_ELEMENT;
        static float Y_SCALE_ELEMENT;
        static float X_OFFSET_ELEMENT;

        //facteurs de zoom et offset courant
        static float X_current_window_scale; //vaut X_SCALE_ELEMENT si mode fullscreen activé, vaut 1.0 sinon
        static float Y_current_window_scale; //vaut Y_SCALE_ELEMENT si mode fullscreen activé, vaut 1.0 sinon
        static float X_current_window_offset;  //vaut X_OFFSET_ELEMENT si mode fullscreen activé, vaut 0 sinon

    private:
        SDL_Window* window;
        bool run;
        bool fullscreen;    //vrai si le mode fullscreen (plein écran est activé)

        SDL_Rect right_rect;    //bande noire droite lors du mode fullscreen
        SDL_Rect left_rect;     //bande noire gauche lors du mode fullscreen

        list<GameState*> state_stack; //pile d'état de jeu
        AssignFileSystem *file_assigner;

    public:
        Game();
        void initialization(const char* title, int x_win_pos, int y_win_pos, bool fullscreen);
        bool isRunning();
        void manageEvents();
        void update();
        void render();
        void clean();

        GameState* getStateByName(const string state_name);
        void changeState(const string state_name);
        void pushState(const string state_name);
        void popState();

        virtual ~Game();
};


class StateDoesntExistException{};

#endif // GAME_H
