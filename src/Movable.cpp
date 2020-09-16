#include "Movable.h"
#include <string>
#include <cmath>
#include <algorithm>

#include "System.h"

using namespace std;


Movable::Movable(double oX, double oY, double base_angle, double speed_scale, Movator *movator)
                                    :Transformable::Transformable(oX, oY, movator),
    oX(oX),
    oY(oY),
    base_angle(base_angle),
    init_base_angle(base_angle),
    speed_scale(speed_scale),
    init_speed_scale(speed_scale)
{
    /*if(this->sheet_name == "Little_Pill"){
        cout << *this->it_colors << endl;
    }*/

    this->fabricateWith(movator);
    /*if(this->sheet_name == "Satori_Portrait"){
        cout << this->X << endl;
    }*/

    this->launch(); //on lance la transforamtion
    this->frozen = false;

    this->out = System::isOut(this->X, this->Y, -this->maximum_size, -this->maximum_size);

}

void Movable::initialiseMovable(Movator *movator){

}




void Movable::fabricateWith(Transformator *transformator){
    Movator *movator = dynamic_cast<Movator*>(transformator);
    this->lambda = movator->getSystVarLambda();
    this->theta = movator->getSystVarTheta();

    this->same_dir = movator->getSameDirIter();
    this->same_speed = movator->getSameSpeedIter();
    this->aimed = movator->getAimedIter();
    this->bounce = movator->getBounceIter();

    this->setPositionType(movator->absolute_position);
    this->margin_out = movator->margin_out;



    if(movator->def_trajectory_origin){
        this->oX = movator->oX_trajectory;
        this->oY = movator->oY_trajectory;
        this->X = this->oX;
        this->Y = this->oY;

        //if(movator->id == "D-2-1")
        //cout << "OUI" << endl;
    }
}



void Movable::setPositionType(bool absolute_position){
    this->absolute_position = absolute_position;
}



void Movable::updateValues(){
    this->lambda.newValue();
    this->theta.newValue();
    //this->updateTransformValue();
    //cout << << endl;
}


void Movable::stopMovement(){
    this->frozen = true;
}

void Movable::Move(Player *player){
    /*if(this->sheet_name == "Satori_Portrait"){
        cout << "X: " << this->X << endl;
    }*/

    if(!this->frozen){

        double prev_lambda = this->lambda.getValue()*this->speed_scale;

        this->updateValues();

        double new_lambda = this->lambda.getValue()*this->speed_scale;
        double new_theta = this->theta.getValue() + this->base_angle;

        if(this->absolute_position){
            this->Vect.changeVal(new_lambda, new_theta);

            X += this->Vect.getCarthX();
            Y += this->Vect.getCarthY();
        }
        else{
            new_theta = this->theta.getValue() + this->init_base_angle;

            this->Vect.changeVal(fabs(new_lambda - prev_lambda), new_theta);

            X = this->oX + new_lambda*cos(degToRad(new_theta));
            Y = this->oY + new_lambda*sin(degToRad(new_theta));

        }

        //printf("%lf\n",this->X);

        if( this->out && !System::isOut(this->X, this->Y, -this->maximum_size, -this->maximum_size) ){
            this->out = false;  //si il a été invoqué en dehors et que maintenant il ne l'ai plus
        }


    }

    this->adaptToKeyframe(player);

}

void Movable::changeMoveSegment(int segment_index){
    this->lambda.changeSegment(segment_index);
    this->theta.changeSegment(segment_index);
}

void Movable::reverseMoveSegment(){
    this->lambda.reverse();
    this->theta.reverse();
}



void Movable::adaptOrigin(double X, double Y){
    this->oX = X;
    this->oY = Y;
}

void Movable::setMirrorOrigin(bool flip_to_left){
    if(flip_to_left && this->oX > System::X_GAME_MIDDLE){
        this->oX = System::X_GAME_MIDDLE - fabs(this->oX - System::X_GAME_MIDDLE);
    }
    else if(!flip_to_left && this->oX <= System::X_GAME_MIDDLE){
        this->oX = System::X_GAME_MIDDLE + fabs(this->oX - System::X_GAME_MIDDLE);
    }
}


bool Movable::isTrajectoryEnd(){
    return this->lambda.isLastValue() && this->theta.isLastValue();
}


bool Movable::isPathChanged(){
    return this->path_just_changed;
}


void Movable::changeAngle(double angle){
    this->init_base_angle = angle;
}


void Movable::adaptToKeyframe(Player *player){

    bool has_bounced = false;
    this->path_just_changed = false;

    int current_segment = this->lambda.getCurrentSegemnt();


    if(!this->out && this->bounce[current_segment] && this->isTouchingWall()){

        this->replaceIfOut();
        this->base_angle = this->newAngleAfterWallCollision();
        this->init_base_angle = base_angle;

        this->lambda.advenceUntilNextKey();
        this->theta.advenceUntilNextKey();

        has_bounced = true;
        current_segment = this->lambda.getCurrentSegemnt();

    }



    if( has_bounced || (this->lambda.isKeyChanged() && this->theta.isKeyChanged()) ){//si l'on est passé à la prochaine keyframe (un seul des deux devrait suffir en réalité)
        //int current_segment = this->lambda.getCurrentSegemnt();
        /*this->same_dir[current_segment];
        this->same_speed[current_segment];
        this->aimed[current_segment];
        this->bounce[current_segment];*/


        /*if(this->sheet_name == "Satori_Portrait"){
            cout << "9" << endl;
        }*/

        /*this->it_colors = this->it_colors - this->it_colors + current_segment;
        this->it_sheets[current_segment];*/

        /*if(this->sheet_name == "Fire_Ball"){
            cout << current_segment << '\t' << *this->it_colors << endl;
        }*/

        this->path_just_changed = true;

        string old_sheet_name = this->sheet_name;
        int old_color = this->color;

        //cout << *(this->it_sheets) << current_segment << endl;

        if(*(this->it_sheets + current_segment) != nullptr){
            this->current_sheet = *(this->it_sheets + current_segment);
            this->sheet_name = this->current_sheet->getSheetName();

            delete this->animation;

            this->animation = this->current_sheet->createAnimCorresponding();
        }


        this->color = this->it_colors[current_segment];

        if(this->color != old_color || this->sheet_name != old_sheet_name){
            this->launch();
        }


        if(player != nullptr){
            if(this->aimed[current_segment]){
                if(this->same_dir[current_segment]){
                    this->base_angle = player->getDirAimed(this->oX, this->oY);
                }
                else{
                    this->base_angle = player->getDirAimed(this->X, this->Y);
                }
            }
        }

        //cout << "mukyu" << endl;

    }


    if(this->bounce[current_segment]){

    }
    else if(this->aimed[current_segment]){
       // this->theta.setValue(this->aimed_angle);
    }
    else if(this->same_dir[current_segment]){
        this->base_angle = 0;
    }
    else{
        this->base_angle = this->init_base_angle;
    }


    if(this->same_speed[current_segment]){
        this->speed_scale = 1.0;
    }
    else{
        this->speed_scale = this->init_speed_scale;
    }


}


bool Movable::isTouchingWall(){
    if(!System::isOut(this->X, this->Y, -this->maximum_size, -this->maximum_size)){
        return false;
    }
    else{
        this->newBoundingBoxSize(); //on a besoin de la bounding box pour calculer la collision exact sur le bord du mur
        return System::isOut(this->X, this->Y, -this->width_BB/2.0 , -this->height_BB/2.0);
    }
}


double Movable::newAngleAfterWallCollision(){
    if(System::isLeft(this->X, -this->width_BB) || System::isRight(this->X, -this->width_BB)){
        return 180 - this->base_angle;
    }
    else{
        return -this->base_angle;
    }
}


void Movable::replaceIfOut(){
    //double temp_angle = (this->base_angle + 360)%360;
    System::replaceIfOut(&this->X, &this->Y, this->width_BB, this->height_BB);
}

bool Movable::isOut(){
    return System::isOut(this->X, this->Y, this->margin_out, this->margin_out);
}

bool Movable::isOutScreen(){
    return System::isOut(this->X, this->Y, 20, 20);
}



double Movable::getX(){
    return this->X;
}
double Movable::getY(){
    return this->Y;
}


Movable::~Movable(){
}








//----------------------------------------------MOVATOR----------------------------------------------//

/*
Movator::Movator(){
}
*/

Movator::Movator(string id, string path_text_file):Transformator(id, path_text_file), lambdaS(1, "lambda"), thetaS(1, "theta"){
    //init

    this->n_key = 0;
    this->loop_trajectory = false;


    this->assignAttributsWithTextFile();

}


void Movator::assignAttributsWithTextFile(){

    this->n_key = 0;
    this->loop_trajectory = false;

    this->number_of_def_key = 0;
    this->file_assigner->assignSimpleValue("absolute_position:", "b", &this->absolute_position);
    this->file_assigner->assignSimpleValue("loop_trajectory:", "b", &this->loop_trajectory);

    string str_oX, str_oY;
    this->def_trajectory_origin = this->file_assigner->assignSimpleValue("trajectory_origin:", "ss", &str_oX, &str_oY);
    if(this->def_trajectory_origin){

        if(str_oX != "MIDDLE_WINDOW"){
            this->oX_trajectory = (str_oX == "MIDDLE") ? System::X_GAME_MIDDLE : stof(str_oX);
        }
        if(str_oX != "MIDDLE_WINDOW"){
            this->oY_trajectory = (str_oY == "MIDDLE") ? System::Y_GAME_MIDDLE : stof(str_oY);
        }

        if(str_oX != "MIDDLE"){
            this->oX_trajectory = (str_oX == "MIDDLE_WINDOW") ? System::WINDOW_WIDTH/2 : stof(str_oX);
        }
        if(str_oY != "MIDDLE"){
            this->oY_trajectory = (str_oY == "MIDDLE_WINDOW") ? System::WINDOW_HEIGHT/2 : stof(str_oY);
        }
    }

    this->lambdaS.eraseAllKeyFrame();
    this->thetaS.eraseAllKeyFrame();

    this->file_assigner->assignWithLoop("MOVEMENT:", this);
    //cf. Movable.h (Movator)


    bool all_update = false;
    all_update = !all_of(this->bounce.begin(), this->bounce.end(), [](bool val){return val == false;});  //si on cherche à rebondir, il faudra aussi tout mettre à jour
    this->file_assigner->assignSimpleValue("all_update:", "b", &all_update);

    this->r_lambda = VarStat(this->lambdaS.getIterSequence(all_update), this->loop_trajectory);
    this->r_theta = VarStat(this->thetaS.getIterSequence(all_update), this->loop_trajectory);

    this->margin_out = 150;
    this->file_assigner->assignSimpleValue("margin_out:", "d", &this->margin_out);

    string other_path_sound_name = "None";
    this->file_assigner->assignSimpleValue("other_path_sound:", "s", &other_path_sound_name);

    this->other_path_sound.assignSound(other_path_sound_name);


}


void Movator::assignOneGroupOfKey(){
    string distance; double duree; double angle;
    string fct_dir; float inf_bound_dir;
    float sup_bound_dir; float amp_dir;
    string fct_speed; float speed_last;
    bool infinite;
    double wait_val;


    //-------------------CLE POUR LA TRAJECTOIRE-------------------//
    if(this->absolute_position){
        bool found_normal = this->file_assigner->assignSimpleValue("KEY_TRAJ:", "sddsfffsfbd", &distance, &duree, &angle, &fct_dir, &inf_bound_dir, &sup_bound_dir, &amp_dir, &fct_speed
                                , &speed_last, &infinite, &wait_val);

        bool found_spc = this->file_assigner->assignSimpleValue("KEY_TRAJ_LOOP:", "sddsfffsfbd", &distance, &duree, &angle, &fct_dir, &inf_bound_dir, &sup_bound_dir, &amp_dir, &fct_speed
                                , &speed_last, &infinite, &wait_val);
        if(found_normal || found_spc){
            double i_distance = (distance == "MIDDLE") ? System::X_GAME_MIDDLE - this->oX_trajectory : stod(distance);


            bool loop = found_spc ? true : false;
            Keyframe Key(i_distance, duree, angle, fct_dir, inf_bound_dir, sup_bound_dir, amp_dir, fct_speed, speed_last, infinite, wait_val, loop);
            this->lambdaS.addKeyframe(Key);
            this->thetaS.addKeyframe(Key);
        }
    }

    //cout << this->id << '\t' << distance << endl;


    //cout << "caved" << endl;

    if(!this->absolute_position){
        double debut = 0; double fin = 1; double duree = 2.0; string preset = "linear"; float wait_val = 0.0; bool key_loop = false;
        this->lambdaS.changeType("bezier");
        this->thetaS.changeType("bezier");

        bool found = this->file_assigner->assignSimpleValue("KEY_LAMBDA:", "dddsfb", &debut, &fin, &duree, &preset, &wait_val, &key_loop);
        Keyframe Key1(debut, fin, duree, preset, wait_val, key_loop);
        if(found){
            this->lambdaS.addKeyframe(Key1);
        }
        else{   //on peut aussi donné les point de la courbe de bezier
            double x1, x2, y1, y2;
            bool found = this->file_assigner->assignSimpleValue("KEY_LAMBDA_POINTS:", "dddddddfb", &debut, &fin, &duree, &x1, &y1, &x2, &y2, &wait_val, &key_loop);
            Keyframe Key1(debut, fin, duree, x1, y1, x2, y2, wait_val, key_loop);
            if(found){
                this->lambdaS.addKeyframe(Key1);
            }
        }


        found = this->file_assigner->assignSimpleValue("KEY_THETA:", "dddsfb", &debut, &fin, &duree, &preset, &wait_val, &key_loop);
        Keyframe Key2(debut, fin, duree, preset, wait_val, key_loop);
        if(found){
            this->thetaS.addKeyframe(Key2);
        }


        //cout << debut << endl;
    }



    //-------------------BOOLEAN DE CHANGEMENT-------------------//
    bool val_same_dir = false; bool val_same_speed = false; bool val_aimed = false; bool val_bounce = false;
    this->file_assigner->assignSimpleValue("CHG:", "bbbb", &val_same_dir, &val_same_speed, &val_aimed, &val_bounce);

    this->same_dir.push_back( val_same_dir );
    this->same_speed.push_back( val_same_speed );
    this->aimed.push_back( val_aimed );
    this->bounce.push_back( val_bounce );

    this->number_of_def_key++;

}


void Movator::UpdateSequences(){
    Transformator::UpdateSequences();

    this->lambdaS.update();
    this->thetaS.update();
}

void Movator::UpdateValues(){
}


VarStat Movator::getSystVarLambda(){
    return this->r_lambda;
}


VarStat Movator::getSystVarTheta(){
    return this->r_theta;
}


vector<bool>::iterator Movator::getSameDirIter(){
    return this->same_dir.begin();
}

vector<bool>::iterator Movator::getSameSpeedIter(){
    return this->same_speed.begin();
}

vector<bool>::iterator Movator::getAimedIter(){
    return this->aimed.begin();
}

vector<bool>::iterator Movator::getBounceIter(){
    return this->bounce.begin();
}






AutoMovable::AutoMovable(double X, double Y, string id, string path_text_file):Movator(id, path_text_file), Movable(X, Y, 0, 1, this){
}



void AutoMovable::Move(Player *player){
    Movator::UpdateSequences();
    Movable::Move(player);
}



void AutoMovable::changeTransformation(string id, string path_text_file){
    if(this->id != id){
        //ce que font les constructeurs
        this->id = id;

        delete this->file_assigner;
        this->file_assigner = new AssignFileSystem(path_text_file, id, "@", "#");

        Transformator::assignAttributsWithTextFile();

        Transformable::fabricateWith(this);
    }
}


void AutoMovable::changeTrajectory(string id, string path_text_file, bool old_origin){

    if(this->id != id){
        //ce que font les constructeurs
        this->id = id;

        delete this->file_assigner;
        this->file_assigner = new AssignFileSystem(path_text_file, id, "@", "#");


        Movator::assignAttributsWithTextFile();
        Transformator::assignAttributsWithTextFile();

        double old_oX = this->X, old_oY = this->Y;

        this->fabricateWith(this);

        if(old_origin){
            this->X = old_oX;
            this->Y = old_oY;
            this->adaptOrigin(this->X, this->Y);
        }

        //cout << "mukyu" << endl;*/
    }
}


void AutoMovable::Draw(bool with_animation){
    Transformator::UpdateSequences();
    Transformable::Draw(with_animation);
}

