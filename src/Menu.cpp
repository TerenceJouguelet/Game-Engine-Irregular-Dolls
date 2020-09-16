#include "Menu.h"
#include <iostream>
#include "Game.h"
#include "System.h"

Menu::Menu(Game *game, string id, string path_text_file):GameState(game, id, path_text_file)
        ,background(System::WINDOW_WIDTH/2, System::WINDOW_HEIGHT/2, "Black_Image", 0), wait_time_action(1.0)
{

    vector<string> AllId;
    this->file_assigner->assignListofValue("button", &AllId);
    for(int i = 0 ; i < AllId.size() ; i++){
        this->buttons.push_back(new Button(this, AllId[i], "data/text/Menu_Button.txt"));
    }
    AllId.clear();

    if(!this->buttons.empty()){
        this->selector = this->buttons.begin();
    }

    float time = 1.0;
    this->file_assigner->assignSimpleValue("wait_time_action:", "f", &time);
    this->wait_time_action = Timer(time);
    this->wait_time_action.launch();

    this->deco = new DecorationGroup(this->file_assigner, "decoration", "data/text/Menu.txt");

}


void Menu::manageEventsState(SDL_Event &event){
    if(!this->wait_time_action.isFinished()){
        this->wait_time_action.pass();
        if(this->wait_time_action.isFinished()){
            for(list<Button*>::iterator it_button = this->buttons.begin() ; it_button != this->buttons.end() ; it_button++){
                //(*it_button)->select();
                (*it_button)->deSelect();
            }
        }
    }



    if(this->wait_time_action.isFinished()){

        switch(event.type)
        {
            case SDL_KEYDOWN:
                if(!this->buttons.empty()){

                    if( event.key.keysym.sym == SDLK_UP ){

                        (*this->selector)->deSelect();
                        if(this->selector != this->buttons.begin()){
                            this->selector--;
                        }
                        else{
                            this->selector = --this->buttons.end();
                        }
                    }
                    if( event.key.keysym.sym == SDLK_DOWN ){
                        (*this->selector)->deSelect();
                        if(this->selector != --this->buttons.end()){
                            this->selector++;
                        }
                        else{
                            this->selector = this->buttons.begin();
                        }
                    }


                    if( event.key.keysym.sym == SDLK_RETURN ){
                        this->pause_music = (*this->selector)->isMusicMustPaused();
                        (*this->selector)->press();
                    }

                    break;

                }

        }


        if(!this->buttons.empty()){
            (*this->selector)->select();
        }
    }

}


void Menu::updateState(){
    this->deco->moveDecorations();
    for(list<Button*>::iterator it_button = this->buttons.begin() ; it_button != this->buttons.end() ; it_button++){
        (*it_button)->Move(nullptr);
    }
}

void Menu::drawState(bool with_animation){
    //this->background.Draw(with_animation);
    this->deco->drawDecorations(with_animation);
    for(list<Button*>::iterator it_button = this->buttons.begin() ; it_button != this->buttons.end() ; it_button++){
        (*it_button)->Draw(with_animation);
    }

    this->drawTransitionEffect(with_animation);
}


Menu::~Menu(){
    delete this->deco;
    for(list<Button*>::iterator it_button = this->buttons.begin() ; it_button != this->buttons.end() ; it_button++){
        delete (*it_button);
    }
}
