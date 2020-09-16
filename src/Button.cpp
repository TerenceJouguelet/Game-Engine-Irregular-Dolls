#include "Button.h"

void Button::assignAttributsWithTextFile(){
    string event_name = "None";
    this->state_name = "None";
    this->file_assigner->assignSimpleValue("event_name:", "s", &event_name);
    this->file_assigner->assignSimpleValue("state_name:", "s", &this->state_name);

    string sound_name_click = "Ok";
    this->file_assigner->assignSimpleValue("sound_on_click:", "s", &sound_name_click);
    this->ok_sound = Sound(sound_name_click);
    string sound_name_selec = "Selection";
    this->file_assigner->assignSimpleValue("sound_selec:", "s", &sound_name_selec);
    this->select_sound = Sound(sound_name_selec);

    this->pause_music = true;
    this->file_assigner->assignSimpleValue("pause_music:", "b", &this->pause_music);

    if(event_name == "changeState"){
        this->event_state_fct = GameState::changeState;
    }
    else if(event_name == "pushState"){
        this->event_state_fct = GameState::pushState;
    }
    else if(event_name == "popState"){
        this->event_state_fct = GameState::popState;
    }
    else{
        cout << event_name << "is not a valid event name." << endl;
        throw NoEventAssignException();
    }
}


Button::Button(GameState *state, string id, string path_text_file):AutoMovable(0, 0, id, path_text_file), Movator(id, path_text_file)
        , just_pressed(false), selected(false), select_sound("Selection"), ok_sound("Ok") {
    this->corresponding_state = state;
    this->assignAttributsWithTextFile();
}


bool Button::isMusicMustPaused(){
    return this->pause_music;
}

void Button::press(){
    //if(!this->just_pressed){
        this->ok_sound.playSound();
        this->shake();

        this->just_pressed = true;
        (this->corresponding_state->*this->event_state_fct)(this->state_name);
    //}
}


void Button::select(){
    if(!this->selected){
        this->selected = true;
        this->reverseMoveSegment();
        this->reverseTransformSegment();
        this->select_sound.playSound();
    }
}

void Button::deSelect(){
    if(this->selected){
        this->reverseMoveSegment();
        this->reverseTransformSegment();
    }
    this->changeMoveSegment(2);
    this->changeTransformSegment(2);
    this->selected = false;
}

Button::~Button(){

}
