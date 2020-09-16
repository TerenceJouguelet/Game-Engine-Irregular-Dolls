#ifndef BULLET_H
#define BULLET_H

#include "Movable.h"
#include "Sheet.h"
#include "Destructible.h"

class Bullet : public Movable, public Collisionable, public Destructible{
    private:
        bool dead;
        int damage;
        //double margin_out;
    public:
        //static map<string, Sheet> bank_images; //La banque d'images necessaire pour les instances

        Bullet(double oX, double oY, double angle_base, double speed_scale, Movator *movator);

        int getDamage();
        virtual void updateCara() override;
        bool isDestroy() override;
        void Deal(Collisionable*) override;
        void drawFrontEffects(bool with_animation) override;
        virtual ~Bullet();

    protected:
        void updateValues() override;

    private:
        void adaptToKeyframe(Player *player) override;

        void setDamageBySheet();
};

#endif // BULLET_H
