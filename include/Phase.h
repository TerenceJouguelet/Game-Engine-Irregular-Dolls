#ifndef PHASE_H
#define PHASE_H

#include <list>
#include "Timer.h"
#include <string>
#include "Sound.h"
#include <SDL2/SDL.h>

class Player;
class DivisionManager;
class AssignFileSystem;
class Boss;
class AutoMovable;
class LifeBar;
class Text;
class Pattern;
class Dialogue;
class DecorationGroup;

using namespace std;

class Phase{
    protected:
        AssignFileSystem *file_assigner;
        Timer duration;
        bool VN_mode;   //mode visual novel
        bool dialogue_phase;    //vrai si cette phase est une phase de dialogue
    private:
        list<DivisionManager*> bundle_div;
        Dialogue *dialogue;

    public:
        Phase(string phase_name);
        void Event(SDL_Event &event);
        virtual void Update(Player *player);

        virtual bool isFinished();
        virtual bool isEmpty();

        bool isInDialogueSection();

        virtual void drawBackground(bool with_animation);
        virtual void drawPhase(bool with_animation);
        virtual void drawBossIndicator(bool with_animation);
        virtual ~Phase();

    protected:
        void updateDivisions(Player *player);
        virtual void manageDivOrigin(DivisionManager *division_manager);
        void destroyAllDivisionsManager();
        void drawDivisions(bool with_animation);
};


class BossPhase: public Phase{
    public:
        Boss *boss; //reference
        int *boss_pv;
        LifeBar *boss_bar;
        Timer delay_next;   //temps à attendre avant le lancement de la prochaine phase

        bool spell_card;
        Text *spell_name;
        AutoMovable *spell_background;
        DecorationGroup *spell_animation;
        Text *spell_time;
        Sound spell_sound;
        //AutoMovable *boss_portrait;

        bool empty;

        Timer atk_delay;
        list<Pattern*> boss_patterns;


    public:
        BossPhase(string phase_name, int num_phase, Boss *boss);
        void Update(Player *player) override;
        void manageDivOrigin(DivisionManager *division_manager) override;

        bool isFinished() override;
        bool isAttackFinished();
        bool isEmpty() override;

        void drawBackground(bool with_animation) override;
        void drawPhase(bool with_animation) override;
        void drawBossIndicator(bool with_animation) override;
        virtual ~BossPhase();

    private:
        void destroyAll();
};


class SpawnDeclarationException{};
class SpellCardDeclarationException{};

#endif // PHASE_H
