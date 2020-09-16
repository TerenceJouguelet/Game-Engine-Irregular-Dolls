#ifndef FOE_H
#define FOE_H

#include <map>
#include <SDL2/SDL_image.h>
#include <string>
#include "Movable.h"
#include "Pattern.h"
#include "Destructible.h"

class Foe : public Movable, public Collisionable, public Destructible{

    private:
        //static map<string, Sheet> bank_images; //La banque d'images necessaire pour les instances
        Timer atk_delay; //temps avant que l'ennemi ne tire
        bool can_attack;
        //Pattern *pattern;
        map<string, Pattern*> patterns;
        bool dead;

        Transformable *aura;
        bool has_aura;

    protected:
        int pv; //les points de vie

    public:
        Foe(double oX, double oY, double angle_base, double speed_scale, Movator *movator, float atk_delay);

        void givePattern(string pattern_id, string pattern_text_path_path);
        Pattern* getReferenceOfPattern(string id);
        void shoot(Player &player);
        bool canAttack();
        void disconnectPattern(string id);

        void adaptOriginOfPattern(Pattern *pattern);
        void adaptOriginOfDiv(DivisionManager *division_manager);   //utilisé seulement pour le boss (pour le moment)

        void addAura(Transformator *aura_transformator);
        void addAura(string sheet_name, int color, string id);


        virtual void Move(Player *player) override;

        void updateCara() override;
        bool isDestroy() override;
        virtual void Deal(Collisionable* target) override;

        ~Foe();

    private:
        void adaptToKeyframe(Player *player) override;

        void setPvBySheet();
        void drawFrontEffects(bool with_animation) override;
        void drawBackEffects(bool with_animation) override;

    protected:
        void updateValues() override;
};








class Boss : public Movator, public Foe, public Collisionor{

    private:
        AutoMovable *ennemy_indicator;
        int blink_time;
        bool in_transition;

    public:
        Boss(double oX, double oY, string id, string path_text_file);
        void changeTrajectory(string id, string path_text_file);
        void moveToCenter();
        void setPhasePv(int pv);
        int* getRefofPv();
        void Deal(Collisionable* target) override;
        void Move(Player *player) override;
        void Draw(bool with_animation) override;
        void drawIndicator(bool with_animation);
        virtual ~Boss();

};


#endif // FOE_H
