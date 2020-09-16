#ifndef PATTERN_H
#define PATTERN_H

#include "Handler.h"
#include "Collisionable.h"
#include "Player.h"
#include "Bullet.h"

#include "Transformable.h"
#include <string>

using namespace std;

class Foe;


class Pattern : public Handler, public Collisionor
{
    private:
        Sheet *destroy_effect_sheet;
        int destroy_effect_color;

        Transformator *destroy_effect_transformation;

        Foe* owner; //le posseseur

        string death_sound_name;


    public:
        Pattern(string id, string path_text_file);

        void UpdateSequences() override;

        void assignAttributsWithTextFile() override;
        virtual Movable* addComponent(double oX, double oY, double angle_base, double speed_scale) override;

        void checkElementsCollision(Collisionable *entity);
        void checkCollisionWithOtherPattern(Pattern *other);
        bool aliveStatue(Movable* entity) override;

        void destroyAll();

        void assignOwner(Foe* owner);
        void disconnectOwnerFromThisPattern();
        void disconnectOwner();

        //void UpdateSequences() override;
        virtual ~Pattern();

};

#endif // PATTERN_H
