#include "Phase.h"
#include "Player.h"
#include "System.h"
#include "DivisionManager.h"
#include "AssignFileSystem.h"
#include "Movable.h"
#include "Text.h"
#include "Dialogue.h"
#include "DecorationGroup.h"

#include <stdlib.h>
#include <stdio.h>




//---------------------------------------------------------------------------------------------------------------------------------------
//
//                                                          NORMAL PHASE
//
//---------------------------------------------------------------------------------------------------------------------------------------
Phase::Phase(string phase_name){

    this->file_assigner = new AssignFileSystem("data/text/Stage.txt", phase_name, "@", "#");

    int number_of_spawn_declaration = -1;
    this->file_assigner->assignSimpleValue("spawn_declaration:", "i", &number_of_spawn_declaration);
    if(number_of_spawn_declaration == -1){
        cout << "No number of spawn declared in phase " << phase_name << endl;
        throw SpawnDeclarationException();
    }

    string div_path_text_file = "data/text/foe_control.txt";
    this->file_assigner->assignSimpleValue("div_path_file:", "s", &div_path_text_file);

    string div_id; float spawn_delay;
    for(int i_spawn = 0 ; i_spawn < number_of_spawn_declaration ; i_spawn++){

        bool found = this->file_assigner->assignSimpleValue(to_string(i_spawn)+"-spawn:", "sf", &div_id, &spawn_delay);
        if(!found){
            cout << "Incorrect declaration of element " << i_spawn << " in phase " << phase_name << endl;
            throw SpawnDeclarationException();
        }
        this->bundle_div.push_back(new DivisionManager(div_id, div_path_text_file, spawn_delay));
    }

    float time_duration = timeToFrame(1.0);  //par défaut
    this->file_assigner->assignSimpleValue("duration:", "f", &time_duration);
    this->duration = Timer(time_duration);
    this->duration.launch();

    string dialogue_name; string path = "data/dialogues/";
    this->dialogue_phase = this->file_assigner->assignSimpleValue("dialogue:", "s", &dialogue_name);
    if(this->dialogue_phase){
        this->dialogue = new Dialogue(dialogue_name, path);
        this->VN_mode = true;
    }
    else{
        this->dialogue = nullptr;
        this->VN_mode = false;
    }
}


void Phase::Event(SDL_Event &event){
    switch(event.type)
    {
        case SDL_KEYDOWN:
            if(this->VN_mode){
                if ( event.key.keysym.sym == SDLK_x ){
                    this->dialogue->skipScroll();
                }
                if ( event.key.keysym.sym == SDLK_z ){
                    this->dialogue->skipSection();
                }
            }
            break;
    }
}



void Phase::manageDivOrigin(DivisionManager *division_manager){
    //rien
}

void Phase::updateDivisions(Player *player){
    if(!this->VN_mode){
        if(!this->duration.isFinished()){
            this->duration.pass();
        }
    }
    else{
        this->dialogue->updateDialogue();
        if(this->dialogue->isFinished()){
            this->VN_mode = false;
        }
    }

    for(list<DivisionManager*>::iterator it_div = this->bundle_div.begin() ; it_div != this->bundle_div.end() ; it_div++){
        DivisionManager *division_manager = *it_div;
        if(division_manager->isReadyToSpawn()){

            this->manageDivOrigin(division_manager);
            division_manager->createDivElements(*player);
            division_manager->moveDivElements(*player);
            division_manager->shootTheBulletsOfDiv(*player);
            division_manager->checkDivCollisonWithPlayerShoot(*player);

            division_manager->movePatterns(*player);
            division_manager->checkPatternsCollision(player, player);

            if(division_manager->isUseless()){
                cout << "caved muda" << endl;
                delete *it_div;
                division_manager = nullptr;
                it_div = this->bundle_div.erase(it_div);
                cout << "CAVED_MUDA" << endl;
            }
        }
        else{
            division_manager->passSpawnDelay();
        }
    }

}

void Phase::Update(Player *player){
    this->updateDivisions(player);
}


//une phase est terminé si sa durée est atteinte
bool Phase::isFinished(){
    return this->duration.isFinished();
}


//renvoi vrai si la phase est terminée ET qu'il n'y a plus rien en lien avec cette phase sur l'écran (pour la libérer de la mémoire)
bool Phase::isEmpty(){
    return this->duration.isFinished() && this->bundle_div.empty();
}


void Phase::destroyAllDivisionsManager(){
    for(list<DivisionManager*>::iterator it_div = this->bundle_div.begin() ; it_div != this->bundle_div.end() ; it_div++){
        DivisionManager *division_manager = *it_div;
        division_manager->destroyAll();
    }
}




void Phase::drawDivisions(bool with_animation){
    /*for(list<DivisionManager*>::iterator it_div = this->bundle_div.begin() ; it_div != this->bundle_div.end() ; it_div++){
        DivisionManager *division_manager = *it_div;
        division_manager->drawDivAndPatterns(with_animation);
    }*/
}

void Phase::drawPhase(bool with_animation){
    //this->drawDivisions(with_animation);
    for(list<DivisionManager*>::iterator it_div = this->bundle_div.begin() ; it_div != this->bundle_div.end() ; it_div++){
        DivisionManager *division_manager = *it_div;
        division_manager->drawDivAndPatterns(with_animation);
    }
    if(this->VN_mode){
        this->dialogue->Draw(with_animation);
    }
}

void Phase::drawBackground(bool with_animation){
}



bool Phase::isInDialogueSection(){
    return this->VN_mode;
}



void Phase::drawBossIndicator(bool with_animation){

}



Phase::~Phase(){
    delete this->file_assigner;
    if(this->dialogue != nullptr){
        delete this->dialogue;
    }
    for(list<DivisionManager*>::iterator it_div = this->bundle_div.begin() ; it_div != this->bundle_div.end() ; it_div++){
        DivisionManager *division_manager = *it_div;
        delete division_manager;
        it_div = this->bundle_div.erase(it_div);
    }
}









//---------------------------------------------------------------------------------------------------------------------------------------
//
//                                                          BOSS PHASE
//
//---------------------------------------------------------------------------------------------------------------------------------------
BossPhase::BossPhase(string phase_name, int num_phase, Boss *boss):Phase(phase_name), spell_card(false), spell_background(nullptr), empty(false){
    this->boss = boss;
    if(phase_name != "0~Boss_Phase"){
        string satori_traj = "Satori_traj_3";
        this->file_assigner->assignSimpleValue("Satori_traj:", "s", &satori_traj);
        this->boss->changeTrajectory(satori_traj, "data/text/Satori.txt");
    }

    int pv;
    bool found = this->file_assigner->assignSimpleValue("PV:", "i", &pv);
    if(!found){
        cout << "No PV declared in phase " << num_phase << "(" << phase_name << ")" << endl;
        throw SpawnDeclarationException();
    }
    this->boss->setPhasePv(pv);
    this->boss_pv = this->boss->getRefofPv();

    found = this->file_assigner->assignSimpleValue("**SPELL_CARD**", "");

    if(found){
        this->spell_card = true;

        this->spell_name = new Text("Spell_Name", "data/text/Satori.txt");
        string str_spell_name = "SaaS";
        this->file_assigner->assignSimpleValue("spell_name:", "e", &str_spell_name);
        this->spell_name->changeText(str_spell_name);

        string spell_background_name = "Spell_Background";
        this->file_assigner->assignSimpleValue("background:", "s", &spell_background_name);
        this->spell_background = new AutoMovable(System::GAME_X + 600, System::Y_GAME_MIDDLE, spell_background_name, "data/text/Background.txt");
        this->spell_background->shake();


        this->spell_animation = new DecorationGroup(this->file_assigner, "decoration", "data/text/Satori.txt");


        string sound_name = "Spell_Card_Activate";  //par défaut
        this->file_assigner->assignSimpleValue("spell_sound:", "s", &sound_name);
        this->spell_sound.assignSound(sound_name);
        this->spell_sound.playSound();
    }


    if(this->spell_card){
        this->spell_time = new Text("Spell_Time", "data/text/Satori.txt");
        this->spell_time->shake();
    }
    else{
        this->spell_time = new Text("Time", "data/text/Satori.txt");
        this->spell_time->shake();
    }


    vector<string> AllId;

    this->file_assigner->assignListofValue("pattern", &AllId);
    for(int i = 0 ; i < AllId.size() ; i++){
        this->boss_patterns.push_back(new Pattern(AllId[i], "data/text/Satori.txt"));
    }
    AllId.clear();


    float delay = 2.5;  //par défaut
    this->file_assigner->assignSimpleValue("delay_next:", "f", &delay);
    this->delay_next = Timer(delay);

    this->boss_bar = new LifeBar(pv, "violet");

    delay = 0;
    this->file_assigner->assignSimpleValue("atk_delay:", "f", &delay);
    this->atk_delay = Timer(delay);
    this->atk_delay.launch();

}



void BossPhase::manageDivOrigin(DivisionManager *division_manager){
    this->boss->adaptOriginOfDiv(division_manager);
}


void BossPhase::Update(Player *player){

    bool attack_finish = this->isAttackFinished();

    this->updateDivisions(player);

    this->boss->Move(nullptr);
    this->atk_delay.pass();

    if(this->isAttackFinished()){
        if(!this->delay_next.isLaunched()){
            this->delay_next.launch();
            if(this->spell_card){
                this->spell_background->reverseTransformSegment();
            }
            this->boss->moveToCenter();
        }
        this->delay_next.pass();
    }

    if(this->spell_card){
        this->spell_name->Move(nullptr);
        this->spell_animation->moveDecorations();
        this->spell_background->Move(nullptr);
    }

    char c_new_time[10];
    sprintf(c_new_time, "%.2f", this->duration.getRemainingTime());
    string new_time = c_new_time;
    this->spell_time->changeText(new_time);
    this->spell_time->Move(nullptr);

    player->checkShootCollision(this->boss);
    //cout << *this->boss_pv << endl;

    if(!this->isAttackFinished()){
        if(this->atk_delay.isFinished()){
            for(list<Pattern*>::iterator it_pattern = this->boss_patterns.begin() ; it_pattern != this->boss_patterns.end() ; it_pattern++){
                Pattern *pattern = *it_pattern;
                this->boss->adaptOriginOfPattern(pattern);
                pattern->createElements(*player);
                if(pattern->isUsed()){
                    pattern->moveElements(*player);
                    pattern->checkElementsCollision(player);
                    if(player != nullptr){
                        player->checkCollisionWithDeathCircle(pattern);
                        player->checkCollisionWithBomb(pattern);
                    }
                    if(pattern->isFinish()){
                        delete *it_pattern;
                        it_pattern = this->boss_patterns.erase(it_pattern);
                    }
                }
            }
        }

    }

    if(attack_finish != this->isAttackFinished()){    //l'attaque vient juste de finir
        if(this->spell_card){
            Sound("Spell_Finish").playSound();
        }
        this->destroyAll();
    }
}



//L'attaque (spell card ou non) est terminé si le temps est écoulé ou si les pv du boss dans cette phase sont réduits à 0
bool BossPhase::isAttackFinished(){
    return *this->boss_pv <= 0 || this->duration.isFinished();
}


//une phase de boss est terminé si sa durée est atteinte (ou si tous les ennemis de cette phase ont été anéantis)
bool BossPhase::isFinished(){
    //cout << this->isAttackFinished() << this->delay_next.isFinished() << endl;
    if(this->isAttackFinished() && this->delay_next.isFinished()){
        this->empty = true;
        return true;
    }
    return false;
}


bool BossPhase::isEmpty(){
    return this->empty;
}


void BossPhase::destroyAll(){
    Phase::destroyAllDivisionsManager();
    for(list<Pattern*>::iterator it_pattern = this->boss_patterns.begin() ; it_pattern != this->boss_patterns.end() ; it_pattern++){
        Pattern *pattern = *it_pattern;
        pattern->destroyAll();
        /*delete *it_pattern;
        it_pattern = this->boss_patterns.erase(it_pattern);*/
    }
}



void BossPhase::drawBackground(bool with_animation){
    if(this->spell_card){
        this->spell_background->Draw(with_animation);
    }
}


void BossPhase::drawPhase(bool with_animation){
    this->boss->Draw(with_animation);
    Phase::drawPhase(with_animation);

    for(list<Pattern*>::iterator it_pattern = this->boss_patterns.begin() ; it_pattern != this->boss_patterns.end() ; it_pattern++){
        Pattern *pattern = *it_pattern;
        pattern->drawElements(with_animation);
    }

    if(!this->VN_mode && !this->dialogue_phase){
        if(!this->isAttackFinished()){
            this->spell_time->Draw(with_animation);
        }
        if(this->spell_card){
            if(!this->isAttackFinished()){
                this->spell_animation->drawDecorations(with_animation);
                this->spell_name->Draw(with_animation);
            }
        }
        if(!this->isAttackFinished()){
            this->boss_bar->Draw(*this->boss_pv);
        }
    }
}


void BossPhase::drawBossIndicator(bool with_animation){
    this->boss->drawIndicator(with_animation);
}


BossPhase::~BossPhase(){
    if(this->spell_card){
        delete this->spell_name;
        delete this->spell_background;
        delete this->spell_animation;
    }
    delete this->spell_time;
    delete this->boss_bar;

    for(list<Pattern*>::iterator it_pattern = this->boss_patterns.begin() ; it_pattern != this->boss_patterns.end() ; it_pattern++){
        Pattern *pattern = *it_pattern;
        delete pattern;
        it_pattern = this->boss_patterns.erase(it_pattern);
    }
}

