#ifndef COLLISIONABLE_H
#define COLLISIONABLE_H

#include "HitBox.h"
#include "Drawable.h"

class Collisionable
{
    protected:
        HitBox *hitbox;
        bool untouchable;   //vrai si l'on veut que l'entité ne subisse pas les conséquences de la colision
        bool penetrable;    //inverse de untouchable : vrai si l'on veut que l'entité qui touche celle ci ne subisse pas les conséquences de la colision
    public:
        Collisionable();
        Collisionable(string sheet_name);
        Collisionable(Collisionable const& cpy);
        //void turnUntouchable();

        void setTouchableValue(bool);
        void setPenetrableValue(bool);

        void turnPenetrable();
        void turnUnPenetrable();
        bool isPenetrable();
        virtual bool isDestroy()=0;
        virtual void updateCara()=0;
        bool collisionTo(Collisionable *other);
        virtual void Deal(Collisionable*);    //ce que causera la collision pour l'objet concerner
        virtual ~Collisionable();
};

class Collisionor{
    public:
        bool or_penetrable;
        bool or_untouchable;

    public:
        Collisionor();
};

#endif // COLLISIONABLE_H
