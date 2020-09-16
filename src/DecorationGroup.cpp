#include "DecorationGroup.h"
#include "Movable.h"
#include <vector>

DecorationGroup::DecorationGroup(AssignFileSystem *file_assigner, string id_of_list, string path_text_file_for_deco){
    vector<string> AllId;
    file_assigner->assignListofValue(id_of_list, &AllId);
    for(int i = 0 ; i < AllId.size() ; i++){

        this->decorations.push_back(new AutoMovable(0, 0, AllId[i], path_text_file_for_deco));
    }
}


void DecorationGroup::moveDecorations(){
    for(vector<AutoMovable*>::iterator it_deco = this->decorations.begin() ; it_deco != this->decorations.end() ; it_deco++){
        (*it_deco)->Move(nullptr);
    }
}


void DecorationGroup::drawDecorations(bool with_animation){
    for(vector<AutoMovable*>::iterator it_deco = this->decorations.begin() ; it_deco != this->decorations.end() ; it_deco++){
        (*it_deco)->Draw(with_animation);
    }
}

DecorationGroup::~DecorationGroup(){
    for(vector<AutoMovable*>::iterator it_deco = this->decorations.begin() ; it_deco != this->decorations.end() ; it_deco++){
        delete (*it_deco);
    }
}
