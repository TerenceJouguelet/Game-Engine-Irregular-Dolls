#include "AnimationStat.h"

#include <string>
#include <iostream>

AnimationStat::AnimationStat(int id, std::string type)
{
    //Keyframe Key(600, 2.0, 0, "sinus", 0, 3.14, 1.0, "dec", -2, 0.0);
    //this->addKeyframe(Key);

    this->type = type;
    this->size = 0;
    this->key_index = 0;
    this->current_key == this->keyframes.end();
}

void AnimationStat::changeType(string real_type){
    this->type = real_type;
}


int AnimationStat::getSize(){
    return this->size;
}

void AnimationStat::addKeyframe(Keyframe Key){
    this->keyframes.push_back(Key);
    this->size++;
    this->current_key = this->keyframes.begin();
    this->key_index = 0;
    /*if(this->getSize() >= 2 && (this->type == "lambda" || this->type == "theta")){  //on calcule les nouvelles origines
        list<Keyframe>::iterator it_key = this->keyframes.end();
        it_key--;
        Keyframe *next_key = &(*it_key);
        it_key--;
        Keyframe *previous_key = &(*it_key);
        next_key->newOrigin(previous_key);
    }*/
}


void AnimationStat::eraseAllKeyFrame(){
    /*this->keyframes.erase(this->keyframes.begin(), this->keyframes.end());*/
    this->keyframes.clear();
    this->sequence.clear();
    this->size = 0;
}


void AnimationStat::update(){
    if(this->size != 0){
        Keyframe *key = &(*this->current_key);
        double val;
        //cout << this->sequence.size() << endl;
        if( this->current_key != this->keyframes.end() ){


            if(this->type == "theta"){
                val = key->nextDirection();
                this->sequence.push_back( tuple<double, int, bool>(val, this->key_index, key->isLooping()));
                //printf("%lf\t", val);
            }

            else if(this->type == "lambda"){
                val = key->nextSpeed();
                this->sequence.push_back( tuple<double, int, bool>(val, this->key_index, key->isLooping()) );
                //printf("%lf\n", val);
            }

            else if(this->type == "bezier"){
                val = key->nextBezier();
                this->sequence.push_back( tuple<double, int, bool>(val, this->key_index, key->isLooping()) );
                //printf("%lf\n", val);
            }


            if( (*this->current_key).isFinished() ){
                //cout << "mukyu";
                bool wait_finish = (*this->current_key).wait();

                if(wait_finish){
                    bool prec_key_loop = (*this->current_key).isLooping();
                    this->current_key++;
                    this->key_index++;
                    if(this->key_index >= this->keyframes.size()){
                        this->sequence.push_back( tuple<double, int, bool>(99999999,  this->key_index - 1, prec_key_loop));
                    }
                }
            }
        }
    }
}

list<tuple<double,int,bool>>::iterator AnimationStat::getIterSequence(){
    /*while(!this->isCurrentKeyInfiniteOrDoesntExist()){
        this->update();
    }*/
    /*while(!this->isCurrentKeyInfiniteOrDoesntExistOrIsntFisrt()){
        this->update();
    }*/
    this->update();
    return this->sequence.begin();
}


list<tuple<double,int,bool>>::iterator AnimationStat::getIterSequence(bool all_update){
    if(all_update){
        while(!this->isCurrentKeyInfiniteOrDoesntExist()){
            this->update();
        }
    }
    this->update();
    return this->sequence.begin();
}



list<tuple<double,int,bool>>::iterator AnimationStat::getIterSequence(int index){
    list<tuple<double,int,bool>>::iterator it = this->sequence.begin();
    advance(it, index);
    return it;
}


bool AnimationStat::isCurrentKeyInfiniteOrDoesntExist(){
    //cout << (this->size == 0) << (this->current_key == this->keyframes.end()) << ((*this->current_key).isInfinite()) << endl;
    return this->size == 0 || this->current_key == this->keyframes.end() || ( (*this->current_key).isInfinite() /*&& (*this->current_key).isSpeedConstante()*/ );
}

bool AnimationStat::isCurrentKeyInfiniteOrDoesntExistOrIsntFisrt(){
    return this->size == 0 || this->current_key == this->keyframes.end() || this->key_index >= 1 || ( (*this->current_key).isInfinite());
}

AnimationStat::~AnimationStat(){
}
