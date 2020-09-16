#include "Diapo.h"
#include "Movable.h"

Diapo::Diapo(Game *game, string id, string path_text_file):GameState(game, id, path_text_file){
    this->end_of_panorama = false;
    this->file_assigner->assignSimpleValue("state_at_end:", "s", &this->state_at_end);

    int number_of_diapo_declaration = -1;
    this->file_assigner->assignSimpleValue("diapo_declaration:", "i", &number_of_diapo_declaration);
    if(number_of_diapo_declaration == -1){
        cout << "No number of diapo declared in panorama " << id << endl;
        throw DiapoDeclarationException();
    }

    string image_id; string diapo_id; float diapo_duration; string id_trans;
    for(int i_diapo = 0 ; i_diapo < number_of_diapo_declaration ; i_diapo++){
        bool found = this->file_assigner->assignSimpleValue(to_string(i_diapo)+"-diapo:", "ssfs", &image_id, &diapo_id, &diapo_duration, &id_trans);
        if(!found){
            cout << "Incorrect declaration of element " << i_diapo << " in panorama " << id << endl;
            throw DiapoDeclarationException();
        }
        AutoMovable *image = new AutoMovable(0, 0, diapo_id, path_text_file);
        image->setSheet(image_id);
        this->diapos.push_back(image);
        this->duration.push_back(Timer(diapo_duration));
        this->id_transition.push_back(id_trans);
    }
    this->i_diap = 0;
    if(number_of_diapo_declaration != 0){
        this->duration[0].launch();
        this->stack_diapos.push_back(this->diapos[0]);
    }


    string dialogue_name;
    bool dialogue_phase = this->file_assigner->assignSimpleValue("dialogue:", "s", &dialogue_name);
    if(dialogue_phase){
        this->dialogue = new Dialogue(dialogue_name, "data/Panorama/");
        //cout << "C.A.V.E.D." << endl;
        this->VN_mode = true;
    }
    else{
        this->dialogue = nullptr;
        this->VN_mode = false;
    }

}

void Diapo::manageEventsState(SDL_Event &event){
    switch(event.type)
    {
        case SDL_KEYDOWN:
            if ( event.key.keysym.sym == SDLK_RETURN )
            {
                this->end_of_panorama = true;
                this->changeState(this->state_at_end);
            }

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

void Diapo::updateState(){
    if(this->VN_mode){
        this->dialogue->updateDialogue();
        if(this->dialogue->isFinished()){
            this->VN_mode = false;
        }
    }
    if(!this->end_of_panorama){
        for(list<AutoMovable*>::iterator it = this->stack_diapos.begin() ; it != this->stack_diapos.end() ; it++){
           (*it)->Move(nullptr);
        }

        //lorsque la transition est terminé (this->stack_diapos.size() > 1 garentie bien que c'est la transition qui est concerné
        //et non pas le transformable en tant que telle, l'element en bas de la pile est forcement en transition sauf si il est seul)
        if(this->stack_diapos.size() > 1 && this->stack_diapos.front()->isTransformationFinish()){
            this->stack_diapos.pop_front();
        }

        this->duration[this->i_diap].pass();
        if(this->duration[this->i_diap].isFinished()){
            //la diapo en haut de la pile est forcement la courante
            this->stack_diapos.back()->changeTransformation(this->id_transition[this->i_diap], this->path_text_file); //on lance la transformation correspondant à la transition
            this->i_diap++;
            if(this->i_diap < this->diapos.size()){
                this->stack_diapos.push_back(this->diapos[this->i_diap]);
                this->duration[this->i_diap].launch();
            }
            else{
                this->end_of_panorama = true;
                this->changeState(this->state_at_end);
            }
        }
    }
}

void Diapo::drawState(bool with_animation){
    for(list<AutoMovable*>::iterator it = this->stack_diapos.begin() ; it != this->stack_diapos.end() ; it++){
        (*it)->Draw(with_animation);
    }
    if(this->VN_mode){
        this->dialogue->Draw(with_animation);
    }

    this->drawTransitionEffect(with_animation);
}

Diapo::~Diapo(){
    if(this->dialogue != nullptr){
        delete this->dialogue;
    }
    for(vector<AutoMovable*>::iterator it = this->diapos.begin() ; it != this->diapos.end() ; it++){
        delete *it;
    }
}
