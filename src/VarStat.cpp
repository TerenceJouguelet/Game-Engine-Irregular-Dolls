#include "VarStat.h"
#include <stdio.h>
#include "System.h"

VarStat::VarStat(): end(false){
    this->constant = false;
    this->main_loop = false;
    this->random = false;
    this->null = true;
    this->value = 0;

    this->key_just_changed = false;
    this->reversed = false;
}

bool VarStat::isNull(){
    return this->null;
}

bool VarStat::isConstant(){
    return this->constant;
}

//constructeur classique d'une VarStat qui variera selon un itérateur sur une séquence de valeur
//(séquence présente dans l'objet AnimationStat auquel il est associé)
VarStat::VarStat(list<tuple<double,int,bool>>::iterator it_begin, bool main_loop): end(false){
    this->it_value = it_begin;
    this->it_begin = it_begin;
    this->value = get<0>(*it_begin);
    this->constant = false;
    this->random = false;
    this->null = false;
    this->main_loop = main_loop;

    this->key_just_changed = false;
    this->reversed = false;

    //cout << "mukyu" << endl;
    this->segments_begin.push_back(it_begin);
    this->it_end = it_begin;

    this->segment_is_changing = false;
}

//constructeur de simple valeur constante
VarStat::VarStat(double constant): end(false){
    this->value = constant;
    this->constant = true;
    this->main_loop = false;
    this->null = false;
    this->random = false;
    this->reversed = false;
}

//constructeur pour une stat variant aléatoirement
VarStat::VarStat(float inf_bound, float sup_bound): end(false){
    this->inf_bound = inf_bound;
    this->sup_bound = sup_bound;
    this->main_loop = false;
    this->constant = false;
    this->null = false;
    this->random = true;
    this->reversed = false;

    this->value = System::getRandomNumber(inf_bound, sup_bound);
}

bool VarStat::searchAndAssignBeginOfSegement(int segment_index){
    if(segment_index >= this->segments_begin.size()){
        list<tuple<double,int,bool>>::iterator temp_it = this->it_begin;
        int current_num_key = get<1>(*temp_it);
        while(current_num_key < segment_index){
            int prev_num_key = current_num_key;
            ++temp_it;
            if(get<0>(*temp_it) == 99999999){
                return false;    //le segment n'existe pas
            }
            current_num_key = get<1>(*temp_it);
            if(prev_num_key != current_num_key){    //si l'on a changé de segment
                this->segments_begin.push_back(temp_it);    //on en profite pour aussi assigner les débuts des segments précédents
            }
        }
        //this->segments_begin.push_back(temp_it);
    }
    return true;
}


void VarStat::searchAndAssignEnd(){
    if(this->it_end == it_begin){
        list<tuple<double,int,bool>>::iterator temp_it = this->it_begin;
        if(get<0>(*temp_it) == 99999999){    //si sa valeur est celle correspondant à la fin de séquence
            this->it_end = --temp_it;   //on stocke la fin
        }
    }
}


void VarStat::changeSegment(int segment_index){
    if(!this->constant && !this->random && !this->null){
        bool is_segment_exist = this->searchAndAssignBeginOfSegement(segment_index);
        if(  is_segment_exist && (    (!this->reversed && this->getCurrentSegemnt() < segment_index)
                                   || (this->reversed && this->getCurrentSegemnt() > segment_index) ) ){
            this->target_segment = segment_index;
            this->segment_is_changing = true;
            /*if(!this->reversed){
                this->it_value = this->segments_begin[segment_index];
            }
            else{
                if(segment_index == this->segments_begin.size() - 1){    //si c'est le dernier segment
                    this->searchAndAssignEnd();
                    this->it_value = this->it_end;
                }
                else{
                    this->searchAndAssignBeginOfSegement(segment_index + 1);
                    this->it_value = this->segments_begin[segment_index + 1];
                    --this->it_value;
                }
            }*/
        }
    }
}


double VarStat::getValue(){
    return this->value;
}

double VarStat::newValue(){
    if(this->null){
        //this->value = 0;
        return 0;
    }
    //printf("%lf\n", *this->it_value);
    if(this->constant){     //si on la construit sans itérateur (valeur constante)
        return this->value;
    }
    if(this->random){
        double val = System::getRandomNumber(this->inf_bound, this->sup_bound);
        this->value = val;
        //cout << val << endl;
        return val;
    }



    bool segment_loop = get<2>(*this->it_value);

    int prev_num_key = get<1>(*this->it_value);

    //cout << prev_num_key << '\t' << segment_loop << endl;
    //cout << get<0>(*this->it_value) << endl;

    if(!this->reversed){
        this->it_value++; //on passe au prochain tuple
    }
    else{
        if(this->it_value != this->it_begin){
            this->it_value--; //on passe au précédent tuple
        }
    }

    int next_num_key = get<1>(*this->it_value);
    if(prev_num_key != next_num_key){    //on test si le numero à changé
        this->key_just_changed = true;

        if(next_num_key >= this->segments_begin.size()){    //si le segment n'a pas encore été assigner
            this->segments_begin.push_back(this->it_value);
        }

        if(this->segment_is_changing && next_num_key == this->target_segment){
            this->segment_is_changing = false;
        }
        else{
            if(segment_loop){
                this->key_just_changed = false;

                if(this->reversed){
                    if(prev_num_key == this->segments_begin.size() - 1){    //si c'était le dernier segment
                        this->searchAndAssignEnd();
                        this->it_value = this->it_end;
                    }
                    else{
                        this->searchAndAssignBeginOfSegement(prev_num_key + 1);
                        this->it_value = this->segments_begin[prev_num_key + 1];
                        --this->it_value;
                    }
                }
                else{
                    //cout << prev_num_key << endl;
                    this->it_value = this->segments_begin[prev_num_key];
                }
            }
        }
    }
    else{
        this->key_just_changed = false;
    }


    if(get<0>(*this->it_value) == 99999999){    //si sa valeur est celle correspondant à la fin de séquence

        //on stocke la fin
        this->it_end = --this->it_value;
        ++this->it_value;

        if(main_loop){
            this->it_value = this->it_begin;    //si on boucle, on replace l'itérateur au debut
            this->key_just_changed = true;
        }
        else if(get<2>(*this->it_value)){   //si le segment boucle
            this->key_just_changed = true;
            this->it_value = this->segments_begin[get<1>(*this->it_value)];
        }
        else{
            this->key_just_changed = false;
            this->end = true;
            this->it_value--;   //sinon on reprend l'élement précedent (pour ne pas garder le 99999999 en valeur !)
        }
        //return get<0>(*(this->it_value));
    }


    else if(this->reversed && this->it_value == this->it_begin){
        if(main_loop){
            this->searchAndAssignEnd();
            this->it_value = this->it_end;    //si on boucle, on replace l'itérateur à la fin

            this->key_just_changed = true;
        }
        else if( get<2>(*this->it_begin) ){  //si le segment boucle
            this->searchAndAssignBeginOfSegement(1);
            if(this->segments_begin.size() > 1){
                this->it_value = this->segments_begin[1];
                --this->it_value;
            }
            else{
                this->searchAndAssignEnd();
                this->it_value = this->it_end;    //si on boucle, on replace l'itérateur à la fin
            }

            this->key_just_changed = false;
        }
    }




    this->value = get<0>(*this->it_value);
    return this->value;
}


void VarStat::advenceUntilNextKey(){    //très dangereux quand même !
    if(!this->isLastValue() && !this->reversed && !this->isConstant() && !this->random){
        while(!this->isKeyChanged()){
            //cout << get<2>(*this->it_begin) << endl;
            this->newValue();
            /*if(this->isLastValue() && get<2>(*this->it_begin)){
                break;
            }*/
        }
        this->newValue();
    }
}


void VarStat::setValue(double value){
    this->value = value;
}


void VarStat::reset(){
    if(!this->null && !this->constant){
        this->it_value = this->it_begin;
        this->reversed = false;
        this->end = false;
    }
}

void VarStat::reverse(){
    this->reversed = !this->reversed;
    this->end = false;
}

bool VarStat::isKeyChanged(){
    return this->key_just_changed;
}

int VarStat::getCurrentSegemnt(){
    return get<1>(*this->it_value);
}


bool VarStat::isLastValue(){
    return this->end;
}

VarStat::~VarStat(){
}




//****************************************************************************************************//








