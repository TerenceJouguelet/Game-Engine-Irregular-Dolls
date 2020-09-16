#include "GameState.h"
#include "Game.h"

GameState::GameState(Game *game, string id, string path_text_file):transition_time(transition_time), changing_state(false), pushing_state(false), poping_state(false){
    this->game = game;
    this->file_assigner = new AssignFileSystem(path_text_file, id, "@", "#");
    this->path_text_file = path_text_file;

    this->prev_trans_time = 0.0;
    this->next_trans_time = 0.0;

    this->file_assigner->assignSimpleValue("prev_trans_time:", "f", &this->prev_trans_time);
    this->file_assigner->assignSimpleValue("next_trans_time:", "f", &this->next_trans_time);
    this->transitioning = false;
    string path_text_for_transition = path_text_file;

    this->file_assigner->assignSimpleValue("path_text_for_transition:", "s", &path_text_for_transition);
    this->transition_effect = new DecorationGroup(this->file_assigner, "transition", path_text_for_transition);


    string music_name;
    bool found = this->file_assigner->assignSimpleValue("music:", "s", &music_name);
    if(found || music_name == "None"){
        Game::main_music->changeMusic(music_name);
        Game::main_music->playMusic();
    }
    this->pause_music = true;
    //this->file_assigner->assignSimpleValue("pause_music:", "b", &this->pause_music);

    this->fad_out_for_music = 0.0;
    this->file_assigner->assignSimpleValue("music_fad_out:", "i", &fad_out_for_music);
}




void GameState::changeState(const string state_name){
    this->transition_time = Timer(this->next_trans_time);
    this->next_state_name = state_name;
    this->transition_time.launch();
    this->changing_state = true;
    this->transitioning = true;
}

void GameState::pushState(const string state_name){
    this->transition_time = Timer(this->next_trans_time);
    this->next_state_name = state_name;
    this->transition_time.launch();
    this->pushing_state = true;
    this->transitioning = true;
}

void GameState::popState(const string state_name){
    this->transition_time = Timer(this->prev_trans_time);
    this->transition_time.launch();
    this->poping_state = true;
    this->transitioning = true;
}



void GameState::pause(){
    if(this->pause_music){
        Game::main_music->pause();
    }
}

void GameState::resume(){
    if(this->pause_music){
        Game::main_music->resume();
    }
}


/*
void GameState::changeToAnotherState(const string state_name){
    this->game->changeState(state_name);
}
*/



void GameState::doTransition(){
    if(this->transitioning){
        this->transition_effect->moveDecorations();
    }
    if(this->changing_state){
        if(this->pause_music){
            Game::main_music->fadeOut(this->fad_out_for_music);
        }
    }


    this->transition_time.pass();
    if(this->transition_time.isFinished()){
        if(this->changing_state){
            this->game->changeState(this->next_state_name);
            this->changing_state = false;
            this->transitioning = false;
        }
        else if(this->pushing_state){
            this->game->pushState(this->next_state_name);
            this->pushing_state = false;
            this->transitioning = false;
        }
        else if(this->poping_state){
            this->game->popState();
            this->poping_state = false;
            this->transitioning = false;
        }
    }
}



void GameState::drawTransitionEffect(bool with_animation){
    if(this->transitioning){
        this->transition_effect->drawDecorations(with_animation);
    }
}


GameState::~GameState(){
    delete this->transition_effect;
}
