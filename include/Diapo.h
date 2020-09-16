#ifndef DIAPO_H
#define DIAPO_H

#include "GameState.h"
#include "Dialogue.h"

class Diapo : public GameState
{
    private:
        vector<AutoMovable*> diapos;    //les diapos
        list<AutoMovable*> stack_diapos;    //la pile des diapo que l'on doit traité simultanément
        vector<Timer> duration; //leur durée
        vector<string> id_transition;   //id de la transformation qui sera joué lorsque la diapo courante cède sa place à la suivante
        int i_diap;  //indice de la diapo courante
        string state_at_end;    //lorsque le panorama est terminé, on se rendera à l'état qui porte ce nom
        bool end_of_panorama;

        Dialogue *dialogue;
        bool VN_mode;
    public:
        Diapo(Game *game, string id, string path_text_file);
        void manageEventsState(SDL_Event &event) override;
        void updateState() override;
        void drawState(bool with_animation) override;
        virtual ~Diapo();
};

class DiapoDeclarationException{};

#endif // DIAPO_H
