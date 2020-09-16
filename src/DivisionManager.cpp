#include "DivisionManager.h"

DivisionManager::DivisionManager(string division_id, string path_text_file, float spawn_delay):spawn_delay(spawn_delay), division(nullptr){
    this->division = new Division(division_id, path_text_file);
    this->spawn_delay.launch();
}


void DivisionManager::passSpawnDelay(){
    this->spawn_delay.pass();
}

bool DivisionManager::isReadyToSpawn(){
    return this->spawn_delay.isFinished();
}


void DivisionManager::updateCurrentPattern(){
    if(division->hasNewBundle()){
        this->currents_patterns.merge(this->division->getPatternsBundle());
    }
}


void DivisionManager::createDivElements(Player &player){
    if(this->division != nullptr){
        this->division->createElements(player);
    }
}


void DivisionManager::moveDivElements(Player &player){
    if(this->division != nullptr){
        this->division->moveElements(player);
        this->updateCurrentPattern();

        if(division->isFinish()){
            cout << "caved_division" << endl;
            delete this->division;
            this->division = nullptr;
        }
    }
}


void DivisionManager::shootTheBulletsOfDiv(Player &player){
    if(this->division != nullptr){
        this->division->shootTheBullets(player);
    }
}


void DivisionManager::checkDivCollisonWithPlayerShoot(Player &player){
    if(this->division != nullptr){
        this->division->checkCollisionWithPlayerShoot(player);
    }
}






void DivisionManager::adaptDivOrigin(double X, double Y){
    if(this->division != nullptr){
        this->division->adaptOrigin(X, Y);
    }
}






void DivisionManager::movePatterns(Player &player){
    for(list<Pattern*>::iterator it = this->currents_patterns.begin() ; it != this->currents_patterns.end() ; ++it){
        Pattern *pattern = *it;
        if(pattern->isUsed()){
            pattern->moveElements(player);
        }
    }
}


void DivisionManager::checkPatternsCollision(Collisionable *target, Player *player){
    for(list<Pattern*>::iterator it = this->currents_patterns.begin() ; it != this->currents_patterns.end() ; ++it){
        Pattern *pattern = *it;
        if(pattern->isUsed()){
            pattern->checkElementsCollision(target);
            if(player != nullptr){
                player->checkCollisionWithDeathCircle(pattern);
                player->checkCollisionWithBomb(pattern);
            }
            if(pattern->isFinish()){
                cout << "caved_pattern" << endl;
                pattern->disconnectOwnerFromThisPattern();
                delete *it;
                it = this->currents_patterns.erase(it);
            }
        }

    }
}


void DivisionManager::destroyAll(){
    if(division != nullptr){
        this->division->destroyAll();
        this->division = nullptr;
    }
    for(list<Pattern*>::iterator it_pattern = this->currents_patterns.begin() ; it_pattern != this->currents_patterns.end() ; it_pattern++){
        Pattern *pattern = *it_pattern;
        pattern->destroyAll();
        pattern->disconnectOwnerFromThisPattern();
    }
}


bool DivisionManager::isDivisionDestroy(){
    return this->division == nullptr;
}


bool DivisionManager::isAllPatternsFinish(){
    return this->isDivisionDestroy() && this->currents_patterns.size() == 0;
}


bool DivisionManager::isUseless(){
    return this->isAllPatternsFinish() && this->isDivisionDestroy();
}


void DivisionManager::drawDivAndPatterns(bool with_animation){
    if(division != nullptr){
        this->division->drawElements(with_animation);
    }

    for(list<Pattern*>::iterator it = this->currents_patterns.begin() ; it != this->currents_patterns.end() ; ++it){
        Pattern *pattern = *it;
        pattern->drawElements(with_animation);
    }
}




void DivisionManager::changeMoveSegmentOfElements(int segment_index){
    if(division != nullptr){
        this->division->changeMoveSegmentOfElements(segment_index);
    }
}
void DivisionManager::reverseTrajectoryOfElements(){
    if(division != nullptr){
        this->division->reverseTrajectoryOfElements();
    }
}




DivisionManager::~DivisionManager(){
    for(list<Pattern*>::iterator it = this->currents_patterns.begin() ; it != this->currents_patterns.end() ; ++it){
        Pattern *pattern = *it;
        pattern->disconnectOwnerFromThisPattern();
        delete pattern;
        it = this->currents_patterns.erase(it);

    }
    delete this->division;
}
