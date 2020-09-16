#include "Dialogue.h"
#include "Movable.h"
#include "Text.h"
#include "System.h"
#include "Game.h"
#include <algorithm>

#define LINE_MAX_WIDTH 400


map<string, Expression> Dialogue::exp_map = {
    { "NEUTRAL", NEUTRAL },
    { "ANGRY", ANGRY },
    { "EMBARRASSED", EMBARRASSED },
    { "SMILE", SMILE },
    { "SAD", SAD },
    { "SURPRISED", SURPRISED },
};




Character::Character(string id_for_lines, string name, string portrait_sheet_name, string speak_sound_name):speak_sound(speak_sound_name){
    this->exp = NEUTRAL;
    this->line = new Text(id_for_lines, "data/text/Dialogue_info.txt");
    this->portrait = new AutoMovable(0, 0, "Portrait_Left", "data/text/Dialogue_info.txt");
    this->portrait->setSheet(portrait_sheet_name);
    /*this->portrait->changeMoveSegment(1);
    this->portrait->changeTransformSegment(1);*/

    this->full_name = new Text("Character_Name", "data/text/Dialogue_info.txt");
    this->full_name->changeText(name);
    this->current_line_progression = "";
    this->line_finished = false;
}


void Character::movePortrait(){
    this->portrait->Move(nullptr);
}


void Character::changeExprsession(string new_expression){
    this->exp = Dialogue::exp_map.at(new_expression);
    this->portrait->changeAllColor(this->exp);
    this->portrait->changeCurrentColor(this->exp);
}


void Character::changeSpeakSound(string sound_name){
    this->speak_sound = Sound(sound_name);
}


void Character::changePortraitTrajectory(string id_of_trajectory){
    this->portrait->changeTrajectory(id_of_trajectory, "data/text/Dialogue_info.txt");
}



void Character::callInPortrait(){
    this->portrait->reverseMoveSegment();
    this->portrait->reverseTransformSegment();
}
void Character::callOutPortrait(){
    this->portrait->reverseMoveSegment();
    this->portrait->reverseTransformSegment();
    this->portrait->changeMoveSegment(2);
    this->portrait->changeTransformSegment(2);
}


void Character::launchPortraitEndAnimation(){
    this->portrait->changeTrajectory("Portrait_Exit", "data/text/Dialogue_info.txt", 1);
    if(this->side == LEFT_SIDE){
        this->portrait->changeAngle(180);
    }
}


void Character::changeTextPosition(){
}


void Character::changePortraitSide(string new_side){
    if(new_side == "RIGHT"){
        /*this->portrait->changeAngle(0);
        this->portrait->setMirrorOrigin(false);*/
        this->changePortraitTrajectory("Portrait_Right");
        this->portrait->setFlip(false);
        this->side = RIGHT_SIDE;
    }
    else if(new_side == "LEFT"){
        /*this->portrait->changeAngle(180);
        this->portrait->setMirrorOrigin(true);*/
        this->changePortraitTrajectory("Portrait_Left");
        this->portrait->setFlip(true);
        this->side = LEFT_SIDE;
    }
}




void Character::affectNewLine(string new_line){
    this->line_finished = false;

    char result[200];
    FC_GetWrappedText(this->line->getFCFont(), result, 100, LINE_MAX_WIDTH, new_line.c_str());
    //cout << result << endl;

    this->full_line = result + 1;
    this->cursor_char = this->full_line.c_str();
    this->current_line_progression = "";
}

void Character::updateLineProgression(){
    if(*this->cursor_char == '\0'){
        this->line_finished = true;
    }
    if(!this->line_finished){
        this->current_line_progression += *this->cursor_char;
        this->cursor_char++;
        this->speak_sound.playSound();
    }
}

void Character::skipScroll(){
    this->current_line_progression = this->full_line;
    this->line_finished = true;
}

void Character::printLine(bool with_animation){
    this->line->changeText(this->current_line_progression);
    this->line->Draw(with_animation);
}

void Character::drawFullName(bool with_animation){
    this->full_name->Draw(with_animation);
}


void Character::drawPortrait(bool with_animation){
    this->portrait->Draw(with_animation);
}


Character::~Character(){
    delete this->portrait;
    delete this->line;
    delete this->full_name;
}





//***************************************************************************************************************************//
//***************************************************************************************************************************//
//***************************************************************************************************************************//
//***************************************************************************************************************************//
//***************************************************************************************************************************//




Dialogue::Dialogue(string dialogue_name, string path):script(dialogue_name, path), current_show(nullptr){
    this->finished = false;
    this->time_mode = false;
    this->narration_mode = false;
    string box_name = "Box";

    while(!this->script.isMarkupEqual("BOX:") && !this->script.isMarkupEqual("CHARACTER:")){
        this->script.nextMarkup();
    }
    if(this->script.isMarkupEqual("BOX:")){
        this->script.getValues("s", &box_name);
    }
    this->box = new AutoMovable(0, 0, box_name, "data/text/Dialogue_info.txt");

    while(!this->script.isMarkupEqual("CHARACTER:") && !this->script.isScriptFinished()){
        this->script.nextMarkup();
    }
    if(this->script.isScriptFinished()){
        cout << "No characters declared for " << dialogue_name << endl;
        throw IncorrectCharactersDeclarationException();
    }
    while(this->script.isMarkupEqual("CHARACTER:")){
        string character_name, character_id, text_id, portrait_sheet_name, side, speak_sound_name;
        this->script.getValues("ssssss", &character_name, &character_id, &text_id, &portrait_sheet_name, &side, &speak_sound_name);
        replace(character_name.begin(), character_name.end(), '_', ' '); // remplace tout les '_' par ' '
        Character *new_character = new Character(text_id, character_name, portrait_sheet_name, speak_sound_name);
        new_character->changePortraitSide(side);
        this->characters[character_id] = new_character;
        this->script.nextMarkup();

        //cout << character_name << text_id << portrait_sheet_name << side << speak_sound_name << endl;
    }

    this->time = 0;
    this->scroll_cadence = timeToFrame(0.05);
    this->enter_delay = Timer(0.7);
    this->exit_delay = Timer(0.7);
    this->enter_delay.launch();

    //this->skipSection();
}


void Dialogue::showAthotherCharacter(string name){
    if(this->current_show != nullptr){
        cout << name << endl;
        this->current_show->callOutPortrait();
    }
    this->current_show = this->characters.at(name);
    this->current_show->callInPortrait();
}

void Dialogue::skipScroll(){
    if(!this->time_mode){
        if(this->enter_delay.isFinished()){
            if(this->current_show == nullptr){
                throw NoCharacterShowException();
            }
            this->current_show->skipScroll();
        }
    }
}


void Dialogue::letInAthotherCharacter(string name){
    //this->current_show = this->characters.at(name);
    //this->current_show->changePortraitTrajectory("Portrait_Enter");
    this->enter_delay.launch();
}


void Dialogue::skipSection(){
    if(!this->time_mode || this->text_duration.isFinished()){

        if(this->enter_delay.isFinished() && !this->script.isScriptFinished()){
            while(!this->script.isMarkupEqual("line:") && !this->script.isScriptFinished()){

                if(this->script.isMarkupEqual("enter:")){
                    string name_char_enter;
                    this->script.getValues("s", &name_char_enter);
                    this->letInAthotherCharacter(name_char_enter);
                }
                if(this->script.isMarkupEqual("show:")){
                    string name_char_show;
                    this->script.getValues("s", &name_char_show);
                    this->showAthotherCharacter(name_char_show);
                }
                if(this->script.isMarkupEqual("exp:")){
                    if(this->current_show != nullptr){
                        string new_exp;
                        this->script.getValues("s", &new_exp);
                        this->current_show->changeExprsession(new_exp);
                    }
                    else{
                        throw NoCharacterShowException();
                    }
                }
                if(this->script.isMarkupEqual("side:")){
                    if(this->current_show != nullptr){
                        string new_side;
                        this->script.getValues("s", &new_side);
                        this->current_show->changePortraitSide(new_side);
                    }
                    else{
                        throw NoCharacterShowException();
                    }
                }
                if(this->script.isMarkupEqual("speed:")){
                    float new_speed;
                    this->script.getValues("f", &new_speed);
                    this->scroll_cadence = timeToFrame(new_speed);
                    //cout << scroll_cadence << endl;
                }
                if(this->script.isMarkupEqual("time_mode:")){
                    this->script.getValues("b", &this->time_mode);
                }
                if(this->script.isMarkupEqual("text_duration:")){
                    float duration;
                    this->script.getValues("f", &duration);
                    this->text_duration = Timer(duration);
                }

                if(this->script.isMarkupEqual("narration:")){
                    this->script.getValues("b", &this->narration_mode);
                }


                if(this->script.isMarkupEqual("stop_music:")){
                    float fad_out;
                    this->script.getValues("f", &fad_out);
                    Game::main_music->fadeOut(fad_out);
                }
                if(this->script.isMarkupEqual("change_music:")){
                    string music_name;
                    this->script.getValues("s", &music_name);
                    Game::main_music->changeAndPlayMusic(music_name);
                }
                if(this->script.isMarkupEqual("text_sound:")){
                    if(this->current_show != nullptr){
                        string text_sound;
                        this->script.getValues("s", &text_sound);
                        this->current_show->changeSpeakSound(text_sound);
                    }
                    else{
                        throw NoCharacterShowException();
                    }
                }

                this->script.nextMarkup();
            }



            if(this->script.isScriptFinished()){
            }
            else{
                this->text_duration.launch();

                string new_line = this->script.getConcatenateString(';');
                if(this->current_show != nullptr){
                    this->current_show->affectNewLine(new_line);
                }
                else{
                    throw NoCharacterShowException();
                }

            }
        }

        if(this->script.isScriptFinished() && !this->exit_delay.isLaunched()){
            cout << "FIN" << endl;
            this->launchEndOfDialogue();
            //this->finished = true;
        }
    }
}


void Dialogue::launchEndOfDialogue(){
    this->exit_delay.launch();
    this->box->reverseMoveSegment();
    this->box->reverseTransformSegment();
    for(map<string, Character*>::iterator it_char = this->characters.begin() ; it_char != this->characters.end() ; it_char++){
        it_char->second->launchPortraitEndAnimation();
    }
}



bool Dialogue::isFinished(){
    return this->finished;
}

void Dialogue::updateDialogue(){
    if(this->time_mode && this->text_duration.isFinished()){
        this->skipSection();
    }


    this->box->Move(nullptr);
    int i = 0;
    for(map<string, Character*>::iterator it_char = this->characters.begin() ; it_char != this->characters.end() ; it_char++){
        it_char->second->movePortrait();
    }


    if(this->enter_delay.isFinished()){
        if(this->time_mode){
            this->text_duration.pass();
        }
        if(this->time%this->scroll_cadence == 0){
            if(this->current_show != nullptr){
                this->current_show->updateLineProgression();
                this->time = 0;
            }
            else{
                throw NoCharacterShowException();
            }
        }
        this->time++;
    }
    else{
        this->enter_delay.pass();
        if(this->enter_delay.isFinished()){ //le delay vient de se terminer
            this->skipSection();
        }
    }

    if(this->exit_delay.isLaunched()){
        this->exit_delay.pass();
        if(this->exit_delay.isFinished()){
            this->finished = true;
        }
    }
}

void Dialogue::Draw(bool with_animation){
    for(map<string, Character*>::iterator it_char = this->characters.begin() ; it_char != this->characters.end() ; it_char++){
        it_char->second->drawPortrait(with_animation);
    }
    this->box->Draw(with_animation);

    if(this->enter_delay.isFinished() && !this->exit_delay.isLaunched()){
        if(this->current_show != nullptr){
            this->current_show->printLine(with_animation);
            if(!this->narration_mode){  //en mode narration, on affiche pas le nom du personnage
                this->current_show->drawFullName(with_animation);
            }
        }
        else{
            throw NoCharacterShowException();
        }
    }
}

Dialogue::~Dialogue(){
    delete this->box;
    for(map<string, Character*>::iterator it_char = this->characters.begin() ; it_char != this->characters.end() ; it_char++){
        delete it_char->second;
    }
}
