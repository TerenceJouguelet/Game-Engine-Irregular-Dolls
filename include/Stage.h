#ifndef STAGE_H
#define STAGE_H

#include "Background.h"
#include "Player.h"
#include "DivisionManager.h"
#include <string>
#include "GameState.h"
#include "Phase.h"
#include "Interface.h"

using namespace std;

class Game;

class Stage: public GameState
{
    private:
        list<Phase*> current_phases;    //les phase actives (pile)
        int num_top_phase;  //la phase qu'on l'on vient d'invoquer
        const int n_normal_phases; //le nombre de phases normal
        const int n_boss_phases; //le nombre de phases pour le boss

        Background *background;
        Interface *interface;
        Player *player;

        Boss *boss;  //le boss du stage

    public:
        Stage(Game *game, int num_phase);
        void manageEventsState(SDL_Event &event) override;
        void updateState() override;
        void drawState(bool with_animation) override;
        //void pause() override;
        //void resume() override;
        virtual ~Stage();
};

#endif // STAGE_H
