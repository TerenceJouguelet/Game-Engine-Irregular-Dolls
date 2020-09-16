#include "Text.h"
#include "Game.h"
#include <string.h>

//---------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------Text------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------//

void Text::assignOneGroupOfKey(){
    double debut; double fin; double duree; string preset; float wait_val; bool key_loop;
    //double debut = 0; double fin = 1; double duree = 2.0; string preset = "linear"; float wait_val = 0.0; bool key_loop = false;

    bool def_R = this->file_assigner->assignSimpleValue("KEY_R:", "dddsfb", &debut, &fin, &duree, &preset, &wait_val, &key_loop);
    if(def_R){
        Keyframe Key(debut, fin, duree, preset, wait_val, key_loop);
        this->a_R.addKeyframe(Key);
    }
    bool def_G = this->file_assigner->assignSimpleValue("KEY_G:", "dddsfb", &debut, &fin, &duree, &preset, &wait_val, &key_loop);
    if(def_G){
        Keyframe Key(debut, fin, duree, preset, wait_val, key_loop);
        this->a_G.addKeyframe(Key);
    }
    bool def_B = this->file_assigner->assignSimpleValue("KEY_B:", "dddsfb", &debut, &fin, &duree, &preset, &wait_val, &key_loop);
    if(def_B){
        Keyframe Key(debut, fin, duree, preset, wait_val, key_loop);
        this->a_B.addKeyframe(Key);
    }

    if(def_R && def_G && def_B){
        this->number_of_def_color_key++;
    }
    this->number_of_def_key++;
}


void Text::assignAttributsWithTextFile(){

    string cpp_text;
    this->file_assigner->assignSimpleValue("text:", "s", &cpp_text);

    this->loop_color = false;
    this->file_assigner->assignSimpleValue("loop_color:", "b", &this->loop_color);
    this->number_of_def_color_key = 0;
    this->file_assigner->assignWithLoop("MOVEMENT:", this);

    if(this->number_of_def_color_key > 0){
        this->R = VarStat(this->a_R.getIterSequence(), this->loop_color);
        this->G = VarStat(this->a_G.getIterSequence(), this->loop_color);
        this->B = VarStat(this->a_B.getIterSequence(), this->loop_color);
    }

    this->file_assigner->assignSimpleValue("shadow:", "i", &this->shadow);
    this->file_assigner->assignSimpleValue("font:", "s", &this->font_name);
    this->file_assigner->assignSimpleValue("size:", "f", &this->size);

    this->alignment = FC_ALIGN_CENTER;
    this->file_assigner->assignSimpleValue("alignment:", "i", &this->alignment);
}




Text::Text(string id, string path_text_file): a_R(1, "bezier"), a_G(1, "bezier"), a_B(1, "bezier")
                            ,Movator(id, path_text_file), AutoMovable(100, 400, id, path_text_file){

    //couleur par défaut
    this->R = VarStat(255);
    this->G = VarStat(255);
    this->B = VarStat(255);

    this->shadow = 0;
    this->size = 40;

    this->assignAttributsWithTextFile();

    this->font = FC_CreateFont();
    string font_path = "data/fonts/"+this->font_name+".ttf";
    FC_LoadFont(this->font, Game::renderer, font_path.c_str(), size, FC_MakeColor(0,0,0,255), TTF_STYLE_NORMAL);
}



void Text::changeText(string new_text){
    this->cpp_string = new_text;
}



FC_Font* Text::getFCFont(){
    return this->font;
}




void Text::Draw(bool with_animation){
    Transformator::UpdateSequences();

    this->a_R.update();
    this->a_G.update();
    this->a_B.update();


    double x_scale_text = this->scale.getValue()*Game::X_current_window_scale;
    double y_scale_text = this->scale.getValue()*Game::Y_current_window_scale;


    if(with_animation){
        this->updateTransformValue();
        this->shakeEffect();
        this->R.newValue();
        this->G.newValue();
        this->B.newValue();

        this->text = this->cpp_string.c_str();
    }

    /*char* result;
    FC_GetWrappedText(this->font, result, 100, 350, this->text);
    this->text = result;*/

    float final_X;
    float final_Y;

    //ombre
    FC_Effect shadow_effect;
    shadow_effect.alignment = this->alignment;
    shadow_effect.color = SDL_Color{0, 0, 0, 255};
    shadow_effect.scale = FC_Scale{x_scale_text, y_scale_text};

    final_X = (this->X + this->shadow)*Game::X_current_window_scale + Game::X_current_window_offset;
    final_Y = (this->Y + this->shadow)*Game::Y_current_window_scale;
    FC_DrawEffect(font, Game::renderer, final_X, final_Y, shadow_effect, this->text); //bordure

    //texte
    FC_Effect text_effect;
    text_effect.color = SDL_Color{this->R.getValue(), this->G.getValue(), this->B.getValue(), this->alpha.getValue()};
    text_effect.alignment = this->alignment;
    text_effect.scale = FC_Scale{x_scale_text, y_scale_text};


    final_X = (this->X + this->x_draw_offset)*Game::X_current_window_scale + Game::X_current_window_offset;
    final_Y = (this->Y + this->y_draw_offset)*Game::Y_current_window_scale;
    FC_DrawEffect(font, Game::renderer, final_X, final_Y, text_effect, this->text);
    //cout << FC_GetWidth(this->font, "AAAAAAAAAAAAAAAAAAAAAAAA\nAAAAA") << endl;
}


Text::~Text(){
    FC_FreeFont(this->font);
    //free(this->text);
}


