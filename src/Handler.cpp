#include "Handler.h"
#include <string>
#include <iostream>
#include <math.h>

#include "System.h"

using namespace std;


void Handler::assignOneGroupOfKey(){

    if(!this->absolute_position){
        double debut; double fin; double duree; string preset; float wait_val; bool key_loop;
        bool found = this->file_assigner->assignSimpleValue("KEY_SPREAD:", "dddsfb", &debut, &fin, &duree, &preset, &wait_val, &key_loop);

        if(found){
            this->def_spread = true;
            Keyframe Key(debut, fin, duree, preset, wait_val, key_loop);
            this->A_spread.addKeyframe(Key);
        }
    }
}




void Handler::assignAttributsWithTextFile(){
    //cout << *it << endl;
    //cout << *it << endl;

    this->file_assigner->assignWithLoop("MOVEMENT:", this);

    bool all_update = false;
    this->file_assigner->assignSimpleValue("all_update:", "b", &all_update);

    if(!this->absolute_position){
        this->spread = VarStat(this->A_spread.getIterSequence(all_update), this->loop_trajectory);
    }


    this->file_assigner->assignSimpleValue("type:", "s", &this->type);
    /*int size = this->sheets.size();
    for(int i = 0 ; i < size ; i++){    //si l'on défini le type par le biais de cette balise, toute les feuilles sont de ce style
        delete this->sheets.front();
        this->sheets.erase(this->sheets.begin());
        this->file_assigner->assignSheetAndHitBox(this->type);
    }*/

    this->file_assigner->assignSimpleValue("origin:", "dd", &this->ooX, &this->ooY);

    string origin_id; this->origin = nullptr;
    this->mobile_origin = this->file_assigner->assignSimpleValue("mobile_origin:", "s", &origin_id);
    if(this->mobile_origin){
        this->origin = new AutoMovable(0, 0, origin_id, this->path_text_file);
    }

    this->origin_on_player = false;
    this->file_assigner->assignSimpleValue("origin_on_player:", "b", &this->origin_on_player);
    this->x_origin_offset = 0;
    this->y_origin_offset = 0;
    this->file_assigner->assignSimpleValue("origin_offset:", "ii", &this->x_origin_offset, &this->y_origin_offset);


    this->file_assigner->assignSimpleValue("mode:", "s", &this->arrange_mode);
    //int init_row;
    this->file_assigner->assignSimpleValue("count:", "ii", &this->rows, &this->columns);
    this->init_rows = this->rows;
    this->row_increment = false;
    this->file_assigner->assignSimpleValue("row_increment:", "b", &this->row_increment);



    this->file_assigner->assignValueWithBezierOrRandom("color:", &this->color, &this->A_color);
    this->file_assigner->assignSimpleValue("min_speed_columns:", "d", &this->min_speed_scale);


    this->file_assigner->assignValueWithBezierOrRandom("offset_angle:", &this->offset_angle, &this->A_offset_angle);
    this->file_assigner->assignValueWithBezierOrRandom("offset_columns:", &this->offset_columns, &this->A_offset_columns);
    this->file_assigner->assignValueWithBezierOrRandom("spawn_offset:", &this->spawn_offset, &this->A_spawn_offset);

    this->file_assigner->assignSimpleValue("aya_mode:", "b", &this->aya_mode);
    this->file_assigner->assignSimpleValue("sanae_mode:", "b", &this->sanae_mode);
    this->file_assigner->assignSimpleValue("cadence:", "t", &this->cadence);
    this->file_assigner->assignSimpleValue("wave_cadence:", "t", &this->wave_cadence);
    this->file_assigner->assignSimpleValue("wave_time:", "t", &this->wave_time);
    this->file_assigner->assignSimpleValue("n_wave:", "i", &this->n_wave);
    this->file_assigner->assignSimpleValue("infinite:", "b", &this->immortal);
    this->reset_wave = false;
    this->file_assigner->assignSimpleValue("reset_wave:", "b", &this->reset_wave);

    this->file_assigner->assignValueWithBezierOrRandom("row_scale_speed:", &this->row_speed_scale, &this->A_row_speed_scale);


    string spawn_sound_name = "None";
    string other_path_sound_name = "None";
    this->file_assigner->assignSimpleValue("spawn_sound:", "s", &spawn_sound_name);
    this->file_assigner->assignSimpleValue("other_path_sound:", "s", &other_path_sound_name);

    this->detached = false;
    this->file_assigner->assignSimpleValue("detached:", "b", &this->detached);
    if(!this->absolute_position){
        this->all_element_connected = true;
        this->file_assigner->assignSimpleValue("all_element_connected:", "b", &this->all_element_connected);
    }

    //int tempsPrec = SDL_GetTicks();
    this->spawn_sound.assignSound(spawn_sound_name);
    //cout << this->id << '\t' << (SDL_GetTicks() - tempsPrec) << endl;



}

void Handler::initAttribut(){
    //init
    this->time = 0;
    this->num_pulse = 0;
    this->i_wave = 0;

    //anim
    this->unused = true;
    this->i_wave = 0;

    //valeurs par défaut
    this->v_O_lambda = VarStat();
    this->v_O_theta = VarStat();
    this->immortal = false;
    this->def_spread = false;
    this->aya_mode = false;
    this->sanae_mode = false;

    this->cadence = 1.0;
    this->wave_cadence = 1.0;
    this->wave_time = 1.0;
    this->n_wave = 1;

    this->arrange_mode = "ring";
    this->rows = 1;
    this->columns = 1;
    this->row_speed_scale = VarStat(1.0);
    this->min_speed_scale = 1.0;
    this->offset_columns = VarStat(0);
    this->offset_angle = VarStat(0);
    this->spawn_offset = VarStat(0);
}

Handler::Handler(string id, string path_text_file):
                            A_offset_columns(1, "bezier") , A_offset_angle(1, "bezier"), A_spawn_offset(1, "bezier")
                            , A_row_speed_scale(1, "bezier"), A_color(1, "bezier"), A_spread(1, "bezier")
                            , a_O_lambda(1, "lambda"), a_O_theta(1, "theta"), Movator(id, path_text_file)

{

    this->initAttribut();
    this->assignAttributsWithTextFile();

}





void Handler::UpdateSequences(){

    Movator::UpdateSequences();

    /*this->a_O_lambda.update();
    this->a_O_theta.update();*/

    this->A_offset_columns.update();
    this->A_offset_angle.update();
    this->A_spawn_offset.update();
    this->A_row_speed_scale.update();
    this->A_color.update();

    if(!this->absolute_position){
        this->A_spread.update();
    }
}

//pour avoir la valeur suivante, que l'on lira avec getValue()
void Handler::UpdateValues(){


    this->v_O_lambda.newValue();
    this->v_O_theta.newValue();

    this->offset_columns.newValue();
    this->offset_angle.newValue();
    this->spawn_offset.newValue();
    this->row_speed_scale.newValue();
    this->color.newValue();

    if(!this->absolute_position){
        this->spread.newValue();
    }

    //cout << this->offset_angle.getValue() << endl;
}


void Handler::adaptOrigin(double X, double Y){
    if(!this->detached || this->origin_on_player){
        /*if(this->id == "Satori_Div_4"){
            cout << X << Y << endl;
        }*/
        this->ooX = X;
        this->ooY = Y;
    }
}


void Handler::createElements(Player &player){

    this->unused = false;

    this->ooX += this->v_O_lambda.newValue()*cos(degToRad(this->v_O_theta.newValue()));
    this->ooY += this->v_O_lambda.newValue()*sin(degToRad(this->v_O_theta.newValue()));

    if(this->mobile_origin){
        this->origin->Move(&player);
        this->ooX = this->origin->getX();
        this->ooY = this->origin->getY();
        /*if(this->id == "D-2-1"){
            cout << this->ooX << '\t' << this->ooY << endl;
        }*/
    }


    if(this->time%(this->wave_time+this->wave_cadence) < this->wave_time-1 && this->i_wave < this->n_wave){
        this->in_a_wave = true;
        if(this->time%this->cadence == 0){

            this->spawn_sound.playSound();


            if(this->origin_on_player){
                player.affectOriginOf(this);
                cout << this->ooX << '\t' << this->ooY << endl;
                if(this->mobile_origin){
                    this->origin->reposition(this->ooX, this->ooY);
                }
            }

            for(int row = 0 ; row < this->rows ; row++){
                for(int column = this->columns - 1 ; column >= 0;  column--){
                    double speed_scale = this->min_speed_scale + column*((1 - this->min_speed_scale)/this->columns);
                    speed_scale *= this->row_speed_scale.getValue();    //si l'on veut changer la vitesse manuellement

                    double angle = this->offset_angle.getValue();
                    if(this->aimed.front()){
                        angle += player.getDirAimed(this->ooX, this->ooY);
                    }

                    if(this->arrange_mode.find("ring") != std::string::npos){
                        angle += (360.0/this->rows)*row + column*(this->offset_columns.getValue()/this->columns);
                    }
                    else if(this->arrange_mode.find("wall") != std::string::npos){
                        angle += row*this->offset_columns.getValue() - (this->offset_columns.getValue()*(this->rows-1))/2;
                    }
                    if(this->arrange_mode.find("rand_pulse") != std::string::npos){
                        if(this->arrange_mode.find("dir") != std::string::npos){   //pour une salve, la direction est aléatoire
                            angle += this->offset_angle.newValue();
                        }
                        if(this->arrange_mode.find("speed") != std::string::npos){  //pour une salve, la vitesse est aléatoire
                            speed_scale *= this->row_speed_scale.newValue();
                        }
                        if(this->arrange_mode.find("offset_columns") != std::string::npos){
                            this->offset_columns.newValue();
                        }
                    }


                    double x_spawn_off;
                    double y_spawn_off;
                    if(this->arrange_mode.find("front") != std::string::npos){
                        double x_first_spawn_off;
                        double y_first_spawn_off;
                        x_first_spawn_off = row*this->offset_columns.getValue() - (this->offset_columns.getValue()*(this->rows-1))/2;
                        y_first_spawn_off = -this->spawn_offset.getValue();
                        if(this->offset_angle.getValue() != 0){
                            double angle_off = this->offset_angle.getValue();
                            x_spawn_off = x_first_spawn_off*cos(degToRad(angle_off)) - y_first_spawn_off*sin(degToRad(angle_off));
                            y_spawn_off = x_first_spawn_off*sin(degToRad(angle_off)) + y_first_spawn_off*cos(degToRad(angle_off));
                        }
                        else{
                            x_spawn_off = x_first_spawn_off;
                            y_spawn_off = y_first_spawn_off;
                        }
                    }
                    else{
                        x_spawn_off = this->spawn_offset.getValue()*cos(degToRad(angle));
                        y_spawn_off = this->spawn_offset.getValue()*sin(degToRad(angle));
                    }


                    if(this->aya_mode){//on doit avoir spawn_offset != 0
                        angle = this->offset_angle.getValue();
                    }

                    if(!color.isNull()){
                        //cout << "size : " << this->colors.size() << endl;
                        for(int k = 0 ; k < this->colors.size() ; k++){
                            //cout << k << '\t' << round(this->color.getValue()) << endl;
                            this->colors[k] = round(this->color.getValue());
                            //this->colors.push_back(round(this->color.getValue()));
                        }
                    }

                    //cout << *(++this->colors.begin()) << endl;
                    /*if(this->sheets.front()->getSheetName() == "shoot_1"){
                        cout << << endl;
                    }*/
                    /*if(this->id == "D-4-1"){
                        cout << x_spawn_off << endl;
                    }*/



                    this->addComponent(this->ooX + x_spawn_off + this->x_origin_offset, this->ooY + y_spawn_off + this->y_origin_offset, angle , speed_scale);



                    if(!color.isNull()){
                        //this->colors.clear();
                    }
                }

            }
            //avant la fin de la salve
            if(this->row_increment){
                this->rows++;
            }

            if(this->sanae_mode){
                this->num_pulse++;
            }
        }
        //lorsque ce n'est pas le moment d'effectuer une salve
        //this->UpdateValues();
    }
    else{
        if(this->row_increment){
            this->rows = this->init_rows;
        }
        if(this->reset_wave){
            this->offset_columns.reset();
            this->offset_angle.reset();
            this->spawn_offset.reset();
            this->row_speed_scale.reset();
            this->color.reset();
        }
        if(this->in_a_wave){    //si le handler jouait une wave à la frame précédente
            if(!this->immortal){
                /*if(this->id == "dolls_shoot"){
                    cout << this->i_wave << this->n_wave << endl;
                }*/
                this->i_wave++;
                /*cout << this->i_wave;
                cout << this->n_wave;*/
            }
            this->in_a_wave = false;
        }

        //lorsque ce n'est pas le moment de produire une série
        if(this->sanae_mode){
            this->num_pulse = 0;
        }

    }
    //cout << this->num_salve << endl;
    this->UpdateValues();
}

void Handler::moveElements(Player &player){

    /*if(this->sheets.front()->getSheetName() == "shoot_1"){
        cout << this->components.size() << endl;
    }*/

    this->UpdateSequences();

    list<Movable*>::iterator it = this->components.begin();

    bool one_path_has_changed = false;


    int component_index = 0;
    while(it != this->components.end()){

        Movable* M = *it;

        M->Move(&player);


        if(!this->absolute_position && this->all_element_connected){
            M->adaptOrigin(this->ooX, this->ooY);
            if(this->mobile_origin){
                M->adaptOrigin(this->origin->getX(), this->origin->getY());
            }
        }

        if(!this->absolute_position && this->def_spread){
            //cout << this->r_lambda.getValue() << endl;
            M->adaptOrigin(this->ooX, this->ooY);
            double angle = component_index*spread.getValue() - (spread.getValue()*(this->components.size()-1))/2;
            M->changeAngle(angle);
        }

        if(!one_path_has_changed && M->isPathChanged()){
            this->other_path_sound.playSound();
            one_path_has_changed = true;
        }


        if(M->isOut() || !this->aliveStatue(*it)){
            delete *it;
            it = this->components.erase(it);

            /*if(this->components.empty()){
                break;
            }*/
        }
        else{
            ++it;
        }


        component_index++;
    }
    this->time++;

}


void Handler::changeMoveSegmentOfElements(int segment_index){
    this->spread.changeSegment(segment_index);
    for(list<Movable*>::iterator it = this->components.begin(); it != this->components.end(); it++){
        (*it)->changeMoveSegment(segment_index);
    }
}

void Handler::reverseTrajectoryOfElements(){
    this->spread.reverse();
    for(list<Movable*>::iterator it = this->components.begin(); it != this->components.end(); it++){
        (*it)->reverseMoveSegment();
    }
}


void Handler::resetTime(){
    this->time = 0;
}



bool Handler::aliveStatue(Movable*){
    return true;    //par défaut, on considérera que ce cas n'existe pas et qu'un objet déplacable est toujours vivant
}

void Handler::drawElements(bool with_animation){
    for(list<Movable*>::iterator it = this->components.begin(); it != this->components.end(); it++){
        (*it)->Draw(with_animation);
    }
}


void Handler::use(){
    this->unused = false;
}

bool Handler::isUsed(){
    return !this->unused;
}


bool Handler::isFinish(){
    return this->components.empty() && !this->unused && this->time > ( wave_time*n_wave + wave_cadence*(n_wave - 1) )*2.0 && !this->immortal;
}

Handler::~Handler(){
    if(this->mobile_origin){
        delete this->origin;
    }
    for(list<Movable*>::iterator it = this->components.begin(); it != this->components.end(); it++){
        delete *it;
        it = this->components.erase(it);
    }
}


/*
//ajoute un element dans la list des composants
Movable* Handler::addComponent(double oX, double oY, double angle_base, double speed_scale, list<tuple<double,int>>::iterator it_lambda,
                                    list<tuple<double,int>>::iterator it_theta, list<bool>::iterator same_dir, list<bool>::iterator same_speed,
                                    list<Sheet*>::iterator it_sheets, int color){


    Movable *M = new Movable(oX, oY, angle_base, speed_scale, it_lambda, it_theta, same_dir, same_speed, it_sheets, color);
    this->components.push_back(M);
}
*/

