
#include "Interface.h"

#define LIVES_SYMBOL_WIDTH 15
#define LIVES_SYMBOL_OFFSET 0

#define BOMBS_SYMBOL_WIDTH 15
#define BOMBS_SYMBOL_OFFSET 0

Interface::Interface(int *n_lives, int *n_bombs){
    this->n_lives = n_lives;
    this->n_bombs = n_bombs;
    for(int i = 0 ; i < MAX_LIVES ; i++){
        this->lives_symbol.push_back(new Drawable(636 + LIVES_SYMBOL_WIDTH*i + LIVES_SYMBOL_OFFSET*i , 65, "Life", 0));
    }
    for(int i = 0 ; i < MAX_BOMBS ; i++){
        this->bombs_symbol.push_back(new Drawable(636 + BOMBS_SYMBOL_WIDTH*i + BOMBS_SYMBOL_OFFSET*i , 110, "Bomb", 0));
    }

    this->background = new Drawable(System::WINDOW_WIDTH/2, System::WINDOW_HEIGHT/2, "Background_Interface", 0);
}

void Interface::updateInterface(){
    int new_color;
    for(int i = 0 ; i < MAX_LIVES ; i++){
        new_color = i < *n_lives ? 1 : 0;
        this->lives_symbol[i]->changeCurrentColor(new_color);
    }
    for(int i = 0 ; i < MAX_BOMBS ; i++){
        new_color = i < *n_bombs ? 1 : 0;
        this->bombs_symbol[i]->changeCurrentColor(new_color);
    }
}

void Interface::drawInterface(bool with_animation){
    this->background->Draw(with_animation);
    for(int i = 0 ; i < MAX_LIVES ; i++){
        this->lives_symbol[i]->Draw(with_animation);
    }
    for(int i = 0 ; i < MAX_BOMBS ; i++){
        this->bombs_symbol[i]->Draw(with_animation);
    }
}

Interface::~Interface(){
    delete this->background;
    for(int i = 0 ; i < MAX_LIVES ; i++){
        delete this->lives_symbol[i];
    }
    for(int i = 0 ; i < MAX_BOMBS ; i++){
        delete this->bombs_symbol[i];
    }
}
