#ifndef PLAYER_H
#define PLAYER_H

#include "Drawable.h"
#include "Collisionable.h"
#include "Transformable.h"
#include "Timer.h"
#include "Destructible.h"
#include "System.h"


#define MAX_BOMBS 2
#define MAX_LIVES 5


class Pattern;
class DivisionManager;
class Foe;

class Player : public AutoTransformable, public Collisionable, public Destructible
{
    private:
        int speed;  //nombre de pixels parcouru par frame

        bool dead; //vrai si on est mort
        Timer death_time;   //pour gérer le temps liée à la mort du personnage

        bool ghost_mode;    //vrai si l'on est entrain de réapparaître
        Timer ghost_time;   //idem

        int lives;   //le nombre de vie;
        bool lost;  //vrai si l'on a perdu

        bool focus; //vrai si l'on est en mode focus
        bool first_focus;
        AutoTransformable aura_1; //l'aura autour du joueur apparaissant en mode focus
        AutoTransformable aura_2; //il y en a aussi une deuxième

        bool is_shooting; //vrai si l'on est entrain de tirer
        Pattern *main_shoot; //le pattern du tire principal

        DivisionManager *dolls;

        Pattern *bomb;
        int n_bomb;
        Timer bomb_duration;
        bool using_bomb;


        class Death_Circle: public AutoTransformable, public Collisionable{
            public:
                bool is_activate;
                Death_Circle():AutoTransformable(System::GAME_WIDTH/2, System::GAME_HEIGHT - 50, "Death_Circle", 0, "Death_Circle_Aniamtion")
                                , Collisionable("Death_Circle"), is_activate(false), Transformator("Death_Circle_Aniamtion", "Death_Circle", 0){
                }
                void updateCara() override{
                    this->hitbox->updateCara(System::GAME_WIDTH/2, System::GAME_HEIGHT - 50, 0, this->scale.getValue());
                };
                void activate(){
                    this->is_activate = true;
                    this->launch();
                }
                bool isActivate(){
                    return this->is_activate;
                }
                void pass(){
                    if(this->isTransformationFinish()){   //si la transforamtion est terminée
                        this->is_activate = false;
                    }
                }
                bool isDestroy(){
                    return false;
                }

        } *death_circle;

        Sound death_circle_sound;


    public:
        Player(std::string sheet_name, double X, double Y, int color);
        void action();
        void shoot();
        void moveShoot();
        bool Lost();
        void checkShootCollision(Collisionable *target_foe);
        void checkBombCollision(Collisionable *target_foe);
        void checkCollisionWithDeathCircle(Pattern *target_pattern);
        void checkCollisionWithBomb(Pattern *target_pattern);
        void updateCara() override;
        double getDirAimed(double X, double Y);
        bool isDestroy() override;
        void Deal(Collisionable*) override;

        int* getRefOfLives();
        int* getRefOfBombs();

        void affectOriginOf(Handler *handler);

        void Draw(bool with_animation) override;
        virtual ~Player();
    private:
        void moveEvent(const Uint8 *keystates);
        void focusEvent(const Uint8 *keystates);
        void replaceIfOut();

        void drawFrontEffects(bool with_animation) override;
};

#endif // PLAYER_H
