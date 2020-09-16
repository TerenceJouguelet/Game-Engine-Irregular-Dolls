#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include "Timer.h"
#include "AssignFileSystem.h"
#include "DecorationGroup.h"

class Game;

using namespace std;

class GameState
{
    private:
        Game *game; //le jeu correspondant à cette état
        Timer transition_time;  //le temps avant de changer d'état
        float prev_trans_time;
        float next_trans_time;
        bool transitioning; //vrai si l'on est entrain de changer d'état
        bool changing_state;
        bool pushing_state;
        bool poping_state;

        string next_state_name;

        DecorationGroup *transition_effect;

        int fad_out_for_music; //en ms

    protected:
        AssignFileSystem *file_assigner;
        string path_text_file;
        bool pause_music; //vrai si la musique doit doit être en pause si cette état est en pause

    public:
        GameState(Game *game, string id, string path_text_file);
        virtual void manageEventsState(SDL_Event &event)=0;
        virtual void updateState()=0;
        void doTransition();


        virtual void pause();
        virtual void resume();
        void changeState(const string state_name);
        void pushState(const string state_name);
        void popState(const string state_name);


        virtual void drawState(bool with_animation)=0;
        void drawTransitionEffect(bool with_animation);
        void changeToAnotherState(const string state_name);
        void backToPreviousState();
        virtual ~GameState();
};

#endif // GAMESTATE_H
