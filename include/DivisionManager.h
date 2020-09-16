#ifndef DIVISIONMANAGER_H
#define DIVISIONMANAGER_H

#include "Division.h"

class DivisionManager
{
    private:
        Division *division;
        list<Pattern*> currents_patterns;
        Timer spawn_delay;

        void updateCurrentPattern();
    public:
        DivisionManager(string division_id, string path_text_file, float spawn_delay);

        void passSpawnDelay();
        bool isReadyToSpawn();

        void createDivElements(Player &player);
        void moveDivElements(Player &player);
        void shootTheBulletsOfDiv(Player &player);
        void checkDivCollisonWithPlayerShoot(Player &player);

        void adaptDivOrigin(double X, double Y);

        void movePatterns(Player &player);
        void checkPatternsCollision(Collisionable *target, Player *player);

        bool isDivisionDestroy();
        bool isAllPatternsFinish();
        bool isUseless();

        void destroyAll();

        void drawDivAndPatterns(bool with_animation);


        void changeMoveSegmentOfElements(int segment_index);
        void reverseTrajectoryOfElements();

        virtual ~DivisionManager();
};

#endif // DIVISIONMANAGER_H
