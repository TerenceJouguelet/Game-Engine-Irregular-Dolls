#ifndef DIAPO_H
#define DIAPO_H

#include "GameState.h"
#include "Dialogue.h"

class Diapo : public GameState
{
    private:
        vector<AutoMovable*> diapos;    //les diapos
        list<AutoMovable*> stack_diapos;    //la pile des diapo que l'on doit trait� simultan�ment
        vector<Timer> duration; //leur dur�e
        vector<string> id_transition;   //id de la transformation qui sera jou� lorsque la diapo courante c�de sa place � la suivante
        int i_diap;  //indice de la diapo courante
        string state_at_end;    //lorsque le panorama est termin�, on se rendera � l'�tat qui porte ce nom
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
