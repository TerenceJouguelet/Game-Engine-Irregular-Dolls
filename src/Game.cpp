#include "Game.h"
#include <string>
#include <string.h>
#include <SDL2/SDL.h>
#include <iostream>

#include "System.h"
#include <SDL2/SDL_mixer.h>
#include "GameState.h"
#include "Stage.h"
#include "Menu.h"
#include "Diapo.h"
#include "AssignFileSystem.h"




//initialisation arbritraire des variables globlales
SDL_Renderer *Game::renderer = nullptr;
Music *Game::main_music = nullptr;

float Game::X_SCALE_ELEMENT = 1.0;
float Game::Y_SCALE_ELEMENT = 1.0;

float Game::X_current_window_scale = 1.0;
float Game::Y_current_window_scale = 1.0;

float Game::X_OFFSET_ELEMENT = 0;
float Game::X_current_window_offset = 0;



Game::Game(){
    this->file_assigner = new AssignFileSystem("data/text/Game_info.txt", "Game_info", "@", "#");
}



bool SetOpenGLAttributes();


void Game::initialization(const char* title, int x_win_pos, int y_win_pos, bool fullscreen){
    int params = 0;

    if(fullscreen){
        params = SDL_WINDOW_FULLSCREEN_DESKTOP;
        this->fullscreen = true;
    }
    else{
        this->fullscreen = false;
    }
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_EVERYTHING | SDL_INIT_JOYSTICK) >= 0 )   //SDL_INIT_JOYSTICK permet de corriger le bug 4391 (freeze toutes les 3 secondes)
    {
        this->window = SDL_CreateWindow(title, x_win_pos, y_win_pos, System::WINDOW_WIDTH, System::WINDOW_HEIGHT, SDL_WINDOW_SHOWN|params|SDL_WINDOW_OPENGL);
        if ( window == NULL)
        {
            char error[50];
            printf("[ERROR] Unable to set 640x480 video : %s", SDL_GetError());
            throw error;
        }


        //Initialistation des variables pour la gestion du plein écran
        SDL_DisplayMode screen_resolution;   //resolution de l'écran
        SDL_GetCurrentDisplayMode(0, &screen_resolution);
        this->X_SCALE_ELEMENT = ((float)screen_resolution.h)/System::WINDOW_HEIGHT;
        this->Y_SCALE_ELEMENT = ((float)screen_resolution.h)/System::WINDOW_HEIGHT;
        float fullscreen_width = System::WINDOW_WIDTH*this->X_SCALE_ELEMENT;    //largeur de l'écran de jeu en fullscreen

        this->X_OFFSET_ELEMENT = ((float)(screen_resolution.w) - fullscreen_width)/2.0; //l'ecran de jeu est placé au centre de l'écran

        //definition de la bande noire droite
        this->right_rect.x = this->X_OFFSET_ELEMENT + fullscreen_width-2;
        this->right_rect.y = 0;
        this->right_rect.w = this->X_OFFSET_ELEMENT*1.1; //*1.1 arbitraire;
        this->right_rect.h = screen_resolution.h;

        //definition de la bande noire gauche
        this->left_rect.x = 0;
        this->left_rect.y = 0;
        this->left_rect.w = this->X_OFFSET_ELEMENT;
        this->left_rect.h = screen_resolution.h;

        cout << screen_resolution.w << '\t' << screen_resolution.h << endl;
        if(this->fullscreen){
            this->X_current_window_scale = this->X_SCALE_ELEMENT;
            this->Y_current_window_scale = this->Y_SCALE_ELEMENT;
            this->X_current_window_offset = this->X_OFFSET_ELEMENT;

        }
        else{
            this->X_current_window_scale = 1.0;
            this->Y_current_window_scale = 1.0;
            this->X_current_window_offset = 0.0;
        }


        //icone de la fenêtre
        SDL_Surface *icon = IMG_Load("data/icon.png");
        SDL_SetWindowIcon(this->window, icon);
        SDL_FreeSurface(icon);

        //creation du renderer
        this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        //on peut maintenant initilaliser les feuilles globales
        Sheet::initializeGlobalsSheets();

        //Initialisation de SDL_mixer
        if( Mix_OpenAudio( 44100, AUDIO_S16SYS, 8, 4096) == -1 )
        {
            char error[50];
            printf("[ERROR] SDL_Mixer ne marche pas : %s", SDL_GetError());
            throw error;
        }

        //on peut maintenant initilaliser les sons globaux
        Sound::initializeGlobalsSound();

        //puis les musiques
        Music::initializeGlobalsMusic();
        this->main_music = new Music("None");

        //L'état initial est lu dans le fichier game.txt
        string temp_init_state;
        this->file_assigner->assignSimpleValue("init_state:", "s", &temp_init_state);
        this->changeState(temp_init_state);





        this->run = true;
    }
    else{
        this->run = false;
    }

}

bool Game::isRunning(){
    return this->run;
}

void Game::manageEvents(){
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type)
    {
        case SDL_QUIT: // Événement de la fenêtre
            this->run = false;
            break;
        case SDL_KEYUP: // Événement de relâchement d'une touche clavier
            if ( event.key.keysym.sym == SDLK_f )
            {
                int flag = this->fullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP;
                SDL_SetWindowFullscreen(this->window, flag);
                this->fullscreen = !this->fullscreen;

                if(this->fullscreen){
                    this->X_current_window_scale = this->X_SCALE_ELEMENT;
                    this->Y_current_window_scale = this->Y_SCALE_ELEMENT;
                    this->X_current_window_offset = this->X_OFFSET_ELEMENT;
                }
                else{
                    this->X_current_window_scale = 1.0;
                    this->Y_current_window_scale = 1.0;
                    this->X_current_window_offset = 0.0;
                }
            }
            break;
        default:
            this->run = true;
            break;
    }


    (this->state_stack.back())->manageEventsState(event);

}

void Game::update(){
    (this->state_stack.back())->updateState();
    (this->state_stack.back())->doTransition();


    if(this->state_stack.empty()){
        this->run = false;
    }
}

void Game::render(){

    for(list<GameState*>::iterator it = this->state_stack.begin() ; it != this->state_stack.end() ; ++it){
        if(it == --this->state_stack.end()){
            (*it)->drawState(true);
        }
        else{
            (*it)->drawState(false);
        }
    }

    if(this->fullscreen){
        SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(Game::renderer, &right_rect);
        SDL_RenderFillRect(Game::renderer, &right_rect);

        SDL_RenderDrawRect(Game::renderer, &left_rect);
        SDL_RenderFillRect(Game::renderer, &left_rect);
    }

    SDL_RenderPresent(this->renderer);
    SDL_RenderClear(this->renderer);


}

void Game::clean(){
    delete this->main_music;
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}




GameState* Game::getStateByName(const string state_name){
    if(state_name == "Stage"){
        return new Stage(this, 0);
    }
    else if(state_name == "Pause"){
        return new Menu(this, "Pause", "data/text/Menu.txt");
    }
    else if(state_name == "Main_Menu"){
        return new Menu(this, "Main_Menu", "data/text/Menu.txt");
    }
    else if(state_name == "Game_Over"){
        return new Menu(this, "Game_Over", "data/text/Menu.txt");
    }
    else if(state_name == "Loading"){
        return new Diapo(this, "Loading", "data/text/Panorama.txt");
    }
    else if(state_name == "Credit"){
        return new Menu(this, "Credit", "data/text/Menu.txt");
    }
    else{
        throw StateDoesntExistException();
    }
}



void Game::changeState(const string state_name){

    while(!this->state_stack.empty()){
        delete this->state_stack.back();
        this->state_stack.pop_back();
    }

    this->state_stack.push_back(this->getStateByName(state_name));
}


void Game::pushState(const string state_name){
    if(!this->state_stack.empty()){
        this->state_stack.back()->pause();
    }

    this->state_stack.push_back(this->getStateByName(state_name));
}


void Game::popState(){
    delete this->state_stack.back();
    this->state_stack.pop_back();

    if(!this->state_stack.empty()){
        this->state_stack.back()->resume();
    }
}



Game::~Game()
{

}






/*
bool SetOpenGLAttributes(){
	// Set our OpenGL version.
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}
*/




