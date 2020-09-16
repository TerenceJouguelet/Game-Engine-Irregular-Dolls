#include "Transformable.h"
#include <iostream>
#include "Point.h"
#include <algorithm>
#include "System.h"

//------------------------------------------------------Transformator------------------------------------------------------//

/*
//on a un constructeur par défaut car au moment de la création du handler, on ne sait pas encore quelle type de transformation ce sera
Transformator::Transformator(): a_scale(1, "bezier"), a_angle(1, "bezier"), a_alpha(1, "bezier"){

}*/

Transformator::Transformator(string preset, string sheet_name, int color): a_scale(1, "bezier"), a_angle(1, "bezier"), a_alpha(1, "bezier")
                                                , a_shake_x(1, "bezier"), a_shake_y(1, "bezier"), file_assigner(nullptr)
                                                , x_offset(0) , y_offset(0){
    this->loop_transformation = false;
    this->initWithPreset(preset);

    this->def_main_type = true;
    this->sheets.push_back(Sheet::newSheet(sheet_name));
    this->colors.push_back(color);
}


Transformator::Transformator(string id, string path_text_file):a_scale(1, "bezier"), a_angle(1, "bezier"), a_alpha(1, "bezier")
                                                , a_shake_x(1, "bezier"), a_shake_y(1, "bezier"), id(id)
                                                , x_offset(0), y_offset(0){
    this->loop_transformation = false;

    int tempsPrec = SDL_GetTicks();

    file_assigner = new AssignFileSystem(path_text_file, id, "@", "#");
    this->path_text_file = path_text_file;
    this->assignAttributsWithTextFile();
}



//pour initialisé les différentes feuilles, ainsi que leur hitbox associées, pour un Handler
void Transformator::assignSheetAndHitBox(string type){
    if(this->def_main_type){
        this->sheets.push_back( this->sheets.front() ); //on copie simplement l'adresse
    }
    else{
        this->sheets.push_back( Sheet::newSheet(type) );
    }

    if(this->sheets.back() == nullptr){
        cout << "Le type choisi pour le handler est invalide" << endl;
        throw new std::invalid_argument("Le type choisi pour le handler est invalide");
    }

    /*if(this->id == "2"){
        for(int i = 0 ; i < this->sheets.size() ; i++){
            cout << this->sheets[i]->getSheetName() << endl;
        }
        cout << endl;
    }*/
}

void Transformator::assignOneGroupOfKey(){
    //-------------------TYPE POUR CHAQUE CHEMIN-------------------//
    string sheet_name; int color;
    if(this->def_main_type){    //on prend la coleur défini avec la balise "type:"
        color = this->colors.front();
    }
    bool found = this->file_assigner->assignSimpleValue("TYP:", "si", &sheet_name, &color);
    if(found){
        this->assignSheetAndHitBox(sheet_name);
        this->colors.push_back(color);
    }
    else{
        this->sheets.push_back(nullptr);
        this->colors.push_back(0);
    }



    //les étapes de la transformation
    double debut; double fin; double duree; string preset; float wait_val; bool key_loop;
    //double debut = 0; double fin = 1; double duree = 2.0; string preset = "linear"; float wait_val = 0.0; bool key_loop = false;

    bool def_alpha = this->file_assigner->assignSimpleValue("KEY_ALPHA:", "dddsfb", &debut, &fin, &duree, &preset, &wait_val, &key_loop);
    if(def_alpha){
        Keyframe Key(debut, fin, duree, preset, wait_val, key_loop);
        this->a_alpha.addKeyframe(Key);
    }
    bool def_angle = this->file_assigner->assignSimpleValue("KEY_ANGLE:", "dddsfb", &debut, &fin, &duree, &preset, &wait_val, &key_loop);
    if(def_angle){
        Keyframe Key(debut, fin, duree, preset, wait_val, key_loop);
        this->a_angle.addKeyframe(Key);
    }
    bool def_scale = this->file_assigner->assignSimpleValue("KEY_SCALE:", "dddsfb", &debut, &fin, &duree, &preset, &wait_val, &key_loop);
    if(def_scale){
        Keyframe Key(debut, fin, duree, preset, wait_val, key_loop);
        this->a_scale.addKeyframe(Key);
    }


    if(def_alpha && def_angle && def_scale){
        this->number_of_def_trans_key++;
    }


    bool def_shake = this->file_assigner->assignSimpleValue("KEY_SHAKE:", "dddsfb", &debut, &fin, &duree, &preset, &wait_val, &key_loop);
    if(def_shake){
        Keyframe Key(debut, fin, duree, preset, wait_val, key_loop);
        this->a_shake_x.addKeyframe(Key);
        this->a_shake_y.addKeyframe(Key);
    }

    bool def_shake_x = this->file_assigner->assignSimpleValue("KEY_SHAKE_X:", "dddsfb", &debut, &fin, &duree, &preset, &wait_val, &key_loop);
    if(def_shake_x){
        Keyframe Key(debut, fin, duree, preset, wait_val, key_loop);
        this->a_shake_x.addKeyframe(Key);
    }

    bool def_shake_y = this->file_assigner->assignSimpleValue("KEY_SHAKE_Y:", "dddsfb", &debut, &fin, &duree, &preset, &wait_val, &key_loop);
    if(def_shake_y){
        Keyframe Key(debut, fin, duree, preset, wait_val, key_loop);
        this->a_shake_y.addKeyframe(Key);
    }


    this->number_of_def_key++;


}


void Transformator::assignAttributsWithTextFile(){
    this->number_of_def_trans_key = 0;
    this->number_of_def_key = 0;
    this->transformation_def_by_step = false;
    //this->initWithPreset("None");

    this->file_assigner->assignSimpleValue("loop_transformation:", "b", &this->loop_transformation);
    this->file_assigner->assignSimpleValue("draw_offset:", "ii", &this->x_offset, &this->y_offset);
    this->r_flip = false;
    this->file_assigner->assignSimpleValue("flip:", "b", &this->r_flip);

    string type; int color;
    bool found = this->file_assigner->assignSimpleValue("type:", "si", &type, &color);
    if(found){
        this->def_main_type = true;
        this->sheets.push_back(Sheet::newSheet(type));    //on construit une unique feuille au debut
        this->colors.push_back(color);
    }
    else{
        this->def_main_type = false;
    }

    this->file_assigner->assignWithLoop("MOVEMENT:", this);
    if(this->number_of_def_trans_key > 0){
        this->transformation_def_by_step = true;
    }


    bool all_update = false;
    this->file_assigner->assignSimpleValue("all_update:", "b", &all_update);

    if(this->transformation_def_by_step){
        //loop_transformation = true;
        this->r_alpha = VarStat(this->a_alpha.getIterSequence(all_update), this->loop_transformation);
        this->r_angle = VarStat(this->a_angle.getIterSequence(all_update), this->loop_transformation);
        this->r_scale = VarStat(this->a_scale.getIterSequence(all_update), this->loop_transformation);

        bool constant_angle = false;
        this->file_assigner->assignSimpleValue("constant_angle:", "b", &constant_angle);
        if(constant_angle){
            this->r_angle = VarStat();
        }
    }
    else{
        this->r_alpha = VarStat(255);
        this->r_angle = VarStat(0);
        this->r_scale = VarStat(1.0);
    }


    this->file_assigner->assignSimpleValue("shake_duration:", "f", &this->shake_duration);

    this->alternate = false;
    this->file_assigner->assignSimpleValue("alternate:", "b", &this->alternate);

    this->shake_cadence = 60;
    this->file_assigner->assignSimpleValue("shake_cadence:", "t", &this->shake_cadence);
    if(this->a_shake_x.getSize() != 0){
        this->r_shake_x = VarStat(this->a_shake_x.getIterSequence(all_update), false);
    }
    else{
        this->r_shake_x = VarStat(0); //par défaut
    }

    if(this->a_shake_y.getSize() != 0){
        this->r_shake_y = VarStat(this->a_shake_y.getIterSequence(all_update), false);
    }
    else{
        this->r_shake_y = VarStat(0);
    }



    //si on ne définit pas les étapes de la transformation dans le "MOVEMENT", on le fait avec un preset
    if(!this->transformation_def_by_step){
        string preset = "None";
        this->file_assigner->assignSimpleValue("transformation:", "s", &preset);
        this->initWithPreset(preset);
    }

    /*this->sheet = this->sheets[0];
    this->color = this->colors[0];*/

    //on peut définir la durée de la transformation, si celle ci est atteinte, la transformation sera considérée comme "terminé"
    //(si l'on veut exploité la méthode isTransformationFinish()
    this->file_assigner->assignSimpleValue("transformation_duration:", "f", &this->duration);
}


VarStat Transformator::getSystVarScale(){
    return this->r_scale;
}
VarStat Transformator::getSystVarAngle(){
    return this->r_angle;
}
VarStat Transformator::getSystVarAlpha(){
    return this->r_alpha;
}

VarStat Transformator::getSystVarShakeX(){
    return this->r_shake_x;
}

VarStat Transformator::getSystVarShakeY(){
    return this->r_shake_y;
}

vector<Sheet*>::iterator Transformator::getSheetsIter(){
    return this->sheets.begin();
}

vector<int> Transformator::getColors(){
    return this->colors;
}

vector<int>::iterator Transformator::getColorsIter(){
    return this->colors.begin();
}


void Transformator::UpdateSequences(){
    this->a_angle.update();
    this->a_scale.update();
    this->a_alpha.update();
    this->a_shake_x.update();
    this->a_shake_y.update();
}





Transformator::~Transformator(){
    if(this->file_assigner != nullptr){
        delete this->file_assigner;
    }
    if(this->def_main_type){
        (this->sheets.front())->deleteSheet();
    }
    else{
        while(!this->sheets.empty()){
            if(this->sheets.front() != nullptr){
                this->sheets.front()->deleteSheet();
            }
            this->sheets.erase(this->sheets.begin());
        }
    }
}



//------------------------------------------------------Transformable------------------------------------------------------//


Transformable::Transformable(double X, double Y, Sheet *sheet, int color):Drawable(X, Y, sheet, color)
                        , active(false), at_cancel(false), angle(0), scale(1), alpha(255), shaking(false)
{
    this->cancel_time = Timer(0.3);
    this->loop = true;
    this->finish = false;
    this->maximum_size = max(this->rect.w, this->rect.h);
}

Transformable::Transformable(double X, double Y, string sheet_name, int color):Drawable(X, Y, sheet_name, color)
                        , active(false), at_cancel(false), angle(0), scale(1), alpha(255), shaking(false)
{
    this->cancel_time = Timer(0.3);
    this->loop = true;
    this->finish = false;
    this->maximum_size = max(this->rect.w, this->rect.h);
}


Transformable::Transformable(double X, double Y, Transformator *transformator):Drawable(X, Y, nullptr, 0)
                            , active(false), at_cancel(false), angle(0), scale(1), alpha(255), shaking(false){
    this->cancel_time = Timer(0.3);
    this->loop = true;
    this->finish = false;

    this->fabricateWith(transformator);

    //PROLO
    this->initDrawElements();

    this->maximum_size = max(this->rect.w, this->rect.h);


    /*if(this->sheet_name == "Little_Pill"){
        cout << this->it_sheets[0]->getSheetName() << endl;
        cout << this->it_sheets[1]->getSheetName() << endl;
    }*/

}


void Transformable::initDrawElements(){
    //PROLO
    this->current_sheet = *this->it_sheets;
    this->color = *this->it_colors;

    if(this->current_sheet != nullptr){
        this->sheet_name = this->current_sheet->getSheetName();
        this->newRectPosition();
        this->addAnimSystem();
    }
    else{
        this->sheet_name = "None";
    }
}



void Transformable::changeAllColor(int new_color){
    for(int i = 0 ; i < this->colors.size() ; i++){
        this->colors[i] = new_color;
    }
}



//on assigne les valeurs provenant du transformateur
void Transformable::setSystVar(VarStat scale, VarStat angle, VarStat alpha, float duration, bool loop_transformation){
    this->scale = scale;
    this->angle = angle;
    this->alpha = alpha;

    if(loop_transformation){
        this->loop = true;
    }
    else{
        this->transform_delay = Timer(duration);
        this->loop = false;
    }
}


bool Transformable::isTransformationFinish(){
    return this->finish;
}


void Transformable::fabricateWith(Transformator *transformator){
    this->finish = false;

    this->setSystVar(transformator->getSystVarScale(), transformator->getSystVarAngle(), transformator->getSystVarAlpha(), transformator->duration, transformator->loop_transformation);
    this->shake_factor_x = transformator->getSystVarShakeX();
    this->shake_factor_y = transformator->getSystVarShakeY();
    this->shake_delay = Timer(transformator->shake_duration);
    this->alternate = transformator->alternate;
    this->shake_cadence = transformator->shake_cadence;
    this->alter_val = false;

    this->flip = transformator->r_flip;

    this->it_sheets = transformator->getSheetsIter();
    this->colors = transformator->getColors();
    this->it_colors = this->colors.begin();//transformator->getColorsIter();

    this->setInitDrawOffset(transformator->x_offset, transformator->y_offset);


}


void Transformable::setFlip(bool new_flip){
    this->flip = new_flip;
}



//on lance la transformation
void Transformable::launch(){
    this->active = true;
    this->finish = false;
    this->at_cancel = false;
    this->angle.reset();
    this->scale.reset();
    this->alpha.reset();

    if(!this->loop){
        this->transform_delay.launch();
    }
}


//on stop la transformation
void Transformable::stop(){
    this->active = false;
}

bool Transformable::isActive(){
    return this->active;
}

//on l'annule, ce qui à pour effet de la jouer à l'envers
void Transformable::cancel(){
    this->angle.reverse();
    this->scale.reverse();
    this->alpha.reverse();

    this->at_cancel = true;
    this->cancel_time.launch();
}


void Transformable::updateTransformValue(){
    if(this->isActive() /*&& !this->isTransformationFinish()*/){
        this->angle.newValue();
        this->scale.newValue();
        this->alpha.newValue();
        if(this->shaking){
            this->shake_factor_x.newValue();
            this->shake_factor_y.newValue();
        }
    }


    if(this->at_cancel){
        this->cancel_time.pass();
        if(this->cancel_time.isFinished()){
            this->stop();
            this->at_cancel = false;
        }
    }
}



void Transformable::changeTransformSegment(int segment_index){
    this->alpha.changeSegment(segment_index);
    this->angle.changeSegment(segment_index);
    this->scale.changeSegment(segment_index);
}


void Transformable::reverseTransformSegment(){
    this->alpha.reverse();
    this->angle.reverse();
    this->scale.reverse();
}





void Transformable::reposition(double X, double Y){
    this->X = X;
    this->Y = Y;
}

//on voudra parfois réadapter la valeur de l'angle (souvent parce qu'il dépend de la direrction de l'objet lorsque celui-ci est déplaçable)
void Transformable::adaptAngle(double angle){
    //if(this->angle.isConstant()){
        this->angle.setValue(angle);
    //}
}




void Transformable::newBoundingBoxSize(){
    double temp_angle = this->angle.getValue();
    if(temp_angle == 0){
        this->clip = this->getClip(this->color, 0, 0);
        this->width_BB = this->clip->w;
        this->height_BB = this->clip->h;
    }
    else{
        Point O(clip->w/2, clip->h/2);

        Point P1(clip->x           , clip->y);
        P1.rotation(temp_angle, O);

        Point P2(clip->x + clip->w  , clip->y);
        P2.rotation(temp_angle, O);

        Point P3(clip->x + clip->w  , clip->y + clip->h);
        P3.rotation(temp_angle, O);

        Point P4(clip->x           , clip->y + clip->h);
        P4.rotation(temp_angle, O);

        int x_values[] = {P1.x, P2.x, P3.x, P4.x};
        int y_values[] = {P1.y, P2.y, P3.y, P4.y};
        this->width_BB = *max_element(x_values, x_values+4) - *min_element(x_values, x_values+4);
        this->height_BB = *max_element(y_values, y_values+4) - *min_element(y_values, y_values+4);
    }
}




void Transformable::shake(){
    this->shake_delay.launch();
    this->shaking = true;
}


void Transformable::shakeEffect(){
    if(this->shaking){
        this->shake_delay.pass();

        double X_shake = System::getRandomNumber(-this->shake_factor_x.getValue(), this->shake_factor_x.getValue());
        double Y_shake = System::getRandomNumber(-this->shake_factor_y.getValue(), this->shake_factor_y.getValue());

        if(this->alternate){
            bool alter_moment = (int)timeToFrame(this->shake_delay.getTime())%this->shake_cadence == 0;
            this->alter_val = alter_moment ? !this->alter_val : this->alter_val;

            X_shake = this->alter_val ? -this->shake_factor_x.getValue() : this->shake_factor_x.getValue();
            Y_shake = this->alter_val ? -this->shake_factor_y.getValue() : this->shake_factor_y.getValue();
        }

        this->setDrawOffset(X_shake, Y_shake);

        if(this->shake_delay.isFinished()){
            this->backToInitDrawOffset();
            this->shaking = false;
        }
    }
}


void Transformable::Draw(bool with_animation){
    /*if(this->sheet_name == "Player_Death"){
        cout << "mukyu" << endl;
    }*/

    int next;
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
    if(this->isActive()){

        if(with_animation){
            this->updateTransformValue();
            this->shakeEffect();
        }
        //if(sheet_name == "Fairy_Maid"){
            //cout << this->alpha.getValue() << endl;
        //}

        /*if(this->sheet_name == "Fairy_Maid"){
            cout << this->clip->x << endl;
        }*/

        bool final_flip;
        if(this->hasAnimSystem() && this->animation->moveFromRight()){
            final_flip = true;
        }
        else{
            final_flip = this->flip;
        }


        /*if(this->sheet_name == "Satori_Spell_Figure"){
            cout << this->alpha.getValue() << endl;
        }*/

        if(!this->hasAnimSystem() || !this->animation->isFinised()){
            this->current_sheet->blitTransformedImage(&this->rect, this->clip, this->angle.getValue(), this->scale.getValue(), this->alpha.getValue(), final_flip);
        }
        if(with_animation){
            if(!this->loop && !this->isTransformationFinish()){
                this->transform_delay.pass();
                if(this->transform_delay.isFinished()){
                    this->finish = true;
                }
            }
        }
    }



    this->drawFrontEffects(with_animation);

}

/*
Transformable::~Transformable(){

}*/

void Transformator::initWithPreset(string preset){
    Keyframe Key = Keyframe();
    this->loop_transformation = false;
    this->duration = -1;

    this->r_angle = VarStat(0);
    this->r_scale = VarStat(1);
    this->r_alpha = VarStat(255);

    if(preset == "None"){
        this->r_angle = VarStat(0);
        this->r_scale = VarStat(1);
        this->r_alpha = VarStat(255);
        duration = 0.0;
    }

    else if(preset == "Rotation_Spawn_1"){
        Key = Keyframe(0, 360, 2.0, "linear", 0, false);
        this->a_angle.addKeyframe(Key);
        this->r_angle = VarStat(this->a_angle.getIterSequence(), true);

        Key = Keyframe(1.5, 1, 0.3, "ease_out", 0, false);
        this->a_scale.addKeyframe(Key);
        this->r_scale = VarStat(this->a_scale.getIterSequence(), false);

        Key = Keyframe(0, 255, 0.3, "ease_out", 0, false);
        this->a_alpha.addKeyframe(Key);
        this->r_alpha = VarStat(this->a_alpha.getIterSequence(), false);
        this->loop_transformation = true;
    }

    else if(preset == "Rotation_Spawn_2"){
        Key = Keyframe(360, 0, 2.0, "linear", 0, false);
        this->a_angle.addKeyframe(Key);
        this->r_angle = VarStat(this->a_angle.getIterSequence(), true);

        Key = Keyframe(1.5, 1, 0.3, "ease_out", 0, false);
        this->a_scale.addKeyframe(Key);
        this->r_scale = VarStat(this->a_scale.getIterSequence(), false);

        Key = Keyframe(0, 100, 0.3, "ease_out", 0, false);
        this->a_alpha.addKeyframe(Key);
        this->r_alpha = VarStat(this->a_alpha.getIterSequence(), false);
        this->loop_transformation = true;
    }

    else if(preset == "Bullet_Rotation_Spawn"){
        Key = Keyframe(360, 0, 1.5, "linear", 0, false);
        this->a_angle.addKeyframe(Key);
        this->r_angle = VarStat(this->a_angle.getIterSequence(), true);
        //this->r_angle = VarStat(0, false);

        Key = Keyframe(2.0, 1, 0.3, "ease_out", 0, false);
        this->a_scale.addKeyframe(Key);
        this->r_scale = VarStat(this->a_scale.getIterSequence(), false);

        Key = Keyframe(0, 255, 0.3, "ease_out", 0, false);
        this->a_alpha.addKeyframe(Key);
        this->r_alpha = VarStat(this->a_alpha.getIterSequence(), false);
        this->loop_transformation = true;
    }
    else if(preset == "Bullet_Spawn"){
        Key = Keyframe(360, 0, 1.5, "linear", 0, false);
        this->a_angle.addKeyframe(Key);
        this->r_angle = VarStat(0);

        Key = Keyframe(2.0, 1, 0.3, "ease_out", 0, false);
        this->a_scale.addKeyframe(Key);
        this->r_scale = VarStat(this->a_scale.getIterSequence(), false);

        Key = Keyframe(0, 255, 0.3, "ease_out", 0, false);
        this->a_alpha.addKeyframe(Key);
        this->r_alpha = VarStat(this->a_alpha.getIterSequence(), false);

        this->duration = 0.3;
    }

    else if(preset == "Boat"){
        Key = Keyframe(45 - 90, 135 - 90, 1.0, "ease_out", 0, false);
        this->a_angle.addKeyframe(Key);
        Key = Keyframe(135 - 90, 45 - 90, 1.0, "ease_out", 0, false);
        this->a_angle.addKeyframe(Key);
        this->r_angle = VarStat(this->a_angle.getIterSequence(), true);
        this->loop_transformation = true;
    }

    else if(preset == "Bullet_Spawn_Marisa"){
        Key = Keyframe(360, 0, 1.5, "linear", 0, false);
        this->a_angle.addKeyframe(Key);
        this->r_angle = VarStat(0);

        Key = Keyframe(2.0, 1, 0.2, "ease_out", 0, false);
        this->a_scale.addKeyframe(Key);
        this->r_scale = VarStat(this->a_scale.getIterSequence(), false);

        Key = Keyframe(0, 255, 0.2, "ease_out", 0, false);
        this->a_alpha.addKeyframe(Key);
        this->r_alpha = VarStat(this->a_alpha.getIterSequence(), false);

        this->duration = 0.2;
    }

    else if(preset == "Death_Circle_Aniamtion"){
        this->r_angle = VarStat(0);

        Key = Keyframe(0.1, 2.0, 1.0, "ease_out", 0, false);
        this->a_scale.addKeyframe(Key);
        this->r_scale = VarStat(this->a_scale.getIterSequence(), false);

        Key = Keyframe(255, 0, 1.0, "ease_out", 0, false);
        this->a_alpha.addKeyframe(Key);
        this->r_alpha = VarStat(this->a_alpha.getIterSequence(), false);

        this->duration = 1.0;
    }

    else if(preset == "Extinction"){
        this->r_angle = VarStat(-90);

        Key = Keyframe(0.2, 3.0, 0.5, "ease_out", 0, false);
        this->a_scale.addKeyframe(Key);
        this->r_scale = VarStat(this->a_scale.getIterSequence(), false);

        Key = Keyframe(255, 0, 0.5, "ease_out", 0, false);
        this->a_alpha.addKeyframe(Key);
        this->r_alpha = VarStat(this->a_alpha.getIterSequence(), false);

        this->duration = 0.5;
    }

    else if(preset == "Player_Extinction"){
        this->r_angle = VarStat(0);

        Key = Keyframe(0.2, 4.0, 0.5, "ease_out", 0, false);
        this->a_scale.addKeyframe(Key);
        this->r_scale = VarStat(this->a_scale.getIterSequence(), false);

        Key = Keyframe(255, 0, 0.5, "ease_out", 0, false);
        this->a_alpha.addKeyframe(Key);
        this->r_alpha = VarStat(this->a_alpha.getIterSequence(), false);

        this->duration = 0.8;
    }
    else{
        throw IncorrectPresetOfTransformationException();
    }


    /*//this->a_angle.isCurrentKeyInfiniteOrDoesntExist();
    while(!this->a_angle.isCurrentKeyInfiniteOrDoesntExist()){
        this->a_angle.update();
    }
    while(!this->a_scale.isCurrentKeyInfiniteOrDoesntExist()){
        this->a_scale.update();
    }
    while(!this->a_alpha.isCurrentKeyInfiniteOrDoesntExist()){
        this->a_alpha.update();
        //cout << preset;
    }*/

    /*this->a_angle.update();
    this->a_scale.update();
    this->a_alpha.update();*/
}
//------------------------------------------------------AutoTransformable------------------------------------------------------//
AutoTransformable::AutoTransformable(double X, double Y, string sheet_name, int color, string preset)
                                    :Transformator(preset, sheet_name, color), Transformable(X, Y, this) /*Transformable(X, Y, sheet_name, color)*/{

    this->setSystVar(this->getSystVarScale(), this->getSystVarAngle(), this->getSystVarAlpha(), this->duration, this->loop_transformation);
}


AutoTransformable::AutoTransformable(double X, double Y, string sheet_name, int color, string id, string path_text_file)
                                    :Transformator(id, path_text_file), Transformable(X, Y, sheet_name, color){

    //this->setSystVar(this->getSystVarScale(), this->getSystVarAngle(), this->getSystVarAlpha(), this->duration, this->loop_transformation);
    this->fabricateWith(this);
}


/*
void AutoTransformable::UpdateSequences(){
    this->UpdateSequences();
    Transformator::UpdateSequences();
}
*/



void AutoTransformable::Draw(bool with_animation){
    this->UpdateSequences();
    Transformable::Draw(with_animation);
}
