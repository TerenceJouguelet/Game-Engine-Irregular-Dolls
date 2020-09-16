#include "Drawable.h"
#include <stdexcept>
#include <iostream>
#include <vector>



//****************************************************************//

Drawable::Drawable(double X, double Y, Sheet *sheet, int color):Positionable(X, Y), color(color), animation(0), x_draw_offset(0), y_draw_offset(0)
{

    this->current_sheet = sheet;
    if(sheet != nullptr){
        this->sheet_name = sheet->getSheetName();
        this->addAnimSystem();
        this->newRectPosition();
    }
    /*if(this->current_sheet == NULL){
        cout << "Pointeur NULL vers une feuiile de sprite" << endl;
        throw new std::invalid_argument("Pointeur NULL vers une feuiile de sprite");
    }*/

    this->role = "Handler";
    this->current_frame = 0;
}

Drawable::Drawable(double X, double Y, string sheet_name, int color):Positionable(X, Y), color(color), sheet_name(sheet_name), animation(0)
                                                                    , x_draw_offset(0), y_draw_offset(0)
{
    this->current_sheet = Sheet::newSheet(sheet_name);
    this->addAnimSystem();
    this->newRectPosition();
    this->role = "Alone";
    this->current_frame = 0;
}

Drawable::Drawable(Drawable const& cpy): Positionable(cpy.X, cpy.Y), color(cpy.color), rect(cpy.rect), clip(cpy.clip), animation(0){
    if(cpy.animation != nullptr){
        animation = new AnimSystem(*(cpy.animation));
    }
    if(this->role == "Alone"){
        this->current_sheet = Sheet::newSheet(cpy.sheet_name);
    }

}


void Drawable::setDrawOffset(int x, int y){
    this->x_draw_offset = x;
    this->y_draw_offset = y;
}


void Drawable::setInitDrawOffset(int x, int y){
    this->x_init_draw_offset = x;
    this->y_init_draw_offset = y;
}


void Drawable::backToInitDrawOffset(){
    this->x_draw_offset = this->x_init_draw_offset;
    this->y_draw_offset = this->y_init_draw_offset;
}


bool Drawable::hasAnimSystem(){
    return this->animation != nullptr;
}


SDL_Rect *Drawable::getClip(int color, int frame, int statut){
    return this->current_sheet->getReferenceOfClip(frame, statut, color);
}

void Drawable::newRectPosition(){
     this->current_sheet->updateRect(&this->rect, this->X, this->Y);
     this->rect.x += this->x_draw_offset;
     this->rect.y += this->y_draw_offset;
}


//certains objets possederont des effets qu'il faudra dessiner aussi. Cette méthode est donc à redéfinir
//par defaut, il n'y a pas d'effets
void Drawable::drawFrontEffects(bool with_animation){

}

void Drawable::drawBackEffects(bool with_animation){

}


void Drawable::setSheet(string sheet_name){
    this->current_sheet = Sheet::newSheet(sheet_name);
    this->sheet_name = sheet_name;
}


void Drawable::changeCurrentColor(int new_color){
    this->color = new_color;
}


void Drawable::Draw(bool with_animation){
    if(this->hasAnimSystem()){
        if(with_animation){
            this->current_frame = this->animation->nextFrame();
        }
        this->clip = this->getClip(this->color, this->current_frame, this->animation->getStatut());
    }
    else{
        this->clip = this->getClip(this->color, 0, 0);
    }

    this->drawBackEffects(with_animation);

    this->newRectPosition();
    if(!this->hasAnimSystem() || !this->animation->isFinised()){     //on ne dessine pas si l'aniamtion est terminée
        this->current_sheet->blitImage(&this->rect, this->clip);
    }

    this->drawFrontEffects(with_animation);
}


void Drawable::addAnimSystem(){
    if(this->hasAnimSystem()){
        delete this->animation;
    }
    this->animation = this->current_sheet->createAnimCorresponding();
}


void Drawable::playAnimation(){
    if(this->hasAnimSystem()){
        this->animation->lauch();
    }
}

bool Drawable::isAnimationFinish(){
    if(this->hasAnimSystem()){
        return this->animation->isFinised();
    }
    return true;
}


Drawable::~Drawable()
{
    if(this->role == "Alone"){
        //delete this->current_sheet;
        this->current_sheet->deleteSheet();
    }
    if(this->hasAnimSystem()){
        delete this->animation;
    }
}

//****************************************************************//

