#ifndef BUTTON_H
#define BUTTON_H

#include "Movable.h"
#include "GameState.h"
#include <iostream>
#include "Sound.h"

using namespace std;

class Button : public AutoMovable
{
    private:
        string state_name;
        void (GameState::*event_state_fct)(const string state_name);
        GameState *corresponding_state;
        bool just_pressed;
        bool selected;

        Sound select_sound;
        Sound ok_sound;

        bool pause_music;   //vrai la musique doit être mis en pause lorsque l'on appuie sur le bouton

    public:
        Button(GameState *state, string id, string path_text_file);
        void assignAttributsWithTextFile() override;
        void press();
        void select();
        void deSelect();
        bool isMusicMustPaused();
        virtual ~Button();
};


class NoEventAssignException{};

#endif // BUTTON_H
