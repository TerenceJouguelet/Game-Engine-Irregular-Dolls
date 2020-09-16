#include "Stage.h"
#include "System.h"
#include "Foe.h"
#include "Game.h"

#include <string>
#include <iostream>

#define N_NORMAL_PHASE 2
#define N_BOSS_PHASE 4

Stage::Stage(Game *game, int num_phase):GameState(game, "Stage", "data/text/Stage.txt")
                , n_normal_phases(N_NORMAL_PHASE), n_boss_phases(N_BOSS_PHASE){


    this->background = new Background("Stage_Background");
    this->player = new Player("Alice", System::X_GAME_MIDDLE, 550, 0);

    this->interface = new Interface(this->player->getRefOfLives(), this->player->getRefOfBombs());

    this->num_top_phase = num_phase;


    int boss_oX = System::X_GAME_MIDDLE, boss_oY = -50; string base_traj = "Satori_traj_0";
    this->file_assigner->assignSimpleValue("boss_spawn_coord:", "ii", &boss_oX, &boss_oY);
    this->file_assigner->assignSimpleValue("boss_base_traj:", "s", &base_traj);
    this->boss = new Boss(boss_oX, boss_oY, base_traj, "data/text/Satori.txt");


    this->file_assigner->assignSimpleValue("n_normal_phases:", "i", &this->n_normal_phases);
    this->file_assigner->assignSimpleValue("n_boss_phases:", "i", &this->n_boss_phases);


    this->file_assigner->assignSimpleValue("begin_phase:", "i",&this->num_top_phase);
    if(this->num_top_phase < this->n_normal_phases){
        this->current_phases.push_back(new Phase(to_string(this->num_top_phase)+"~Normal_Phase"));
    }
    else{
        int num_phase = this->num_top_phase - this->n_normal_phases;
        this->current_phases.push_back(new BossPhase(to_string(num_phase)+"~Boss_Phase", num_phase, this->boss));
    }
}


void Stage::manageEventsState(SDL_Event &event){
    switch(event.type)
    {
        case SDL_KEYDOWN:
            //si on appuie sur ECHAP, on me le jeu en pause
            if ( event.key.keysym.sym == SDLK_ESCAPE )
            {
                Sound("Pause").playSound();
                this->pushState("Pause");
            }
            break;
    }


    if(!this->current_phases.empty()){
        Phase *main_phase = this->current_phases.back();
        main_phase->Event(event);
    }
}


void Stage::updateState(){
    this->background->Move(nullptr);
    this->player->action();
    if(this->player->Lost()){
        this->pushState("Game_Over");
    }

    if(!this->current_phases.empty()){
        Phase *main_phase = this->current_phases.back();
        if(!main_phase->isInDialogueSection()){
            this->player->shoot();
        }
    }

    this->player->moveShoot();

    if(!this->current_phases.empty()){
        Phase *main_phase = this->current_phases.back();
        if(main_phase->isFinished() && this->num_top_phase < this->n_normal_phases - 1){
            this->num_top_phase++;
            this->current_phases.push_back(new Phase(to_string(this->num_top_phase)+"~Normal_Phase"));
        }
        else if(main_phase->isFinished() && this->num_top_phase < this->n_normal_phases + this->n_boss_phases - 1){
            this->num_top_phase++;
            int num_phase = this->num_top_phase - this->n_normal_phases;
            this->current_phases.push_back(new BossPhase(to_string(num_phase)+"~Boss_Phase", num_phase, this->boss));
        }
    }

    for(list<Phase*>::iterator it_phase = this->current_phases.begin() ; it_phase != this->current_phases.end() ; it_phase++){
        Phase *phase = *it_phase;
        phase->Update(this->player);

        if(phase->isEmpty() && it_phase != --this->current_phases.end()){   //si il n'est plus utile et que ce n'est pas celui au dessus de la pile
            cout << "caved phase" << endl;
            delete *it_phase;
            phase = nullptr;
            it_phase = this->current_phases.erase(it_phase);
        }
    }

    this->interface->updateInterface();
}

void Stage::drawState(bool with_animation){
    this->background->Draw(with_animation);
    (this->current_phases.back())->drawBackground(with_animation);
    this->player->Draw(with_animation);
    for(list<Phase*>::iterator it_phase = this->current_phases.begin() ; it_phase != this->current_phases.end() ; it_phase++){
        Phase *phase = *it_phase;
        phase->drawPhase(with_animation);
    }
    this->interface->drawInterface(with_animation);
    for(list<Phase*>::iterator it_phase = this->current_phases.begin() ; it_phase != this->current_phases.end() ; it_phase++){
        Phase *phase = *it_phase;
        phase->drawBossIndicator(with_animation);
    }

    this->drawTransitionEffect(with_animation);
}



/*
void Stage::pause(){
    Game::main_music->pause();
}

void Stage::resume(){
    //cout << "MUKYU" << endl;
    Game::main_music->resume();
}
*/

Stage::~Stage(){
    delete this->background;
    delete this->interface;
    delete this->player;
    delete this->boss;

    for(list<Phase*>::iterator it_phase = this->current_phases.begin() ; it_phase != this->current_phases.end() ; it_phase++){
        Phase *phase = *it_phase;
        delete phase;
        it_phase = this->current_phases.erase(it_phase);
    }
}
