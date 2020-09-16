#ifndef DIVISION_H
#define DIVISION_H

#include "Pattern.h"
#include "Player.h"
#include "Foe.h"
#include "Sound.h"

#include "Transformable.h"
#include <string>
#include <vector>

using namespace std;


class Division : public Handler, public Collisionor
{
    private:
        vector<string> pattern_id;
        string pattern_text_path_path;
        float atk_delay;    //le temps avant que les entités attaquent
        //Pattern *base_pattern;  //le pattern modèle correspondant à cette division
        list<Pattern*> pattern_bundle;    //liste représentant une fournée de pattern (les patterns qui viennent de se créer)
        bool new_bundle;    //vrai si l'on a une nouvelle fournée

        Transformator *destroy_effect_transformation;

        Transformator *aura_transformator;
        bool no_aura;

        string death_sound_name;    //le sons qui sera joué lors de la mort des entité de la division


    public:
        Division(string id, string path_text_file);
        void assignAttributsWithTextFile() override;
        virtual Movable* addComponent(double oX, double oY, double angle_base, double speed_scale) override;
        //void checkElementsCollision(Player &player);
        void checkCollisionWithPlayerShoot(Player &player);
        bool aliveStatue(Movable*) override;

        list<Movable*>* getRefOfComponents();

        bool hasNewBundle();
        list<Pattern*> getPatternsBundle();
        void shootTheBullets(Player &player);

        void destroyAll();

        //void UpdateSequences() override;
        virtual ~Division();
};



#endif // DIVISION_H
