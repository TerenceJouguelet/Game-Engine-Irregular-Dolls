#ifndef DESTRUCTIBLE_H
#define DESTRUCTIBLE_H

#include <string>
#include <iostream>
#include "Sound.h"

using namespace std;

class Transformable;
class Transformator;
class Sheet;

class Destructible
{
    private:
        Sound destroy_sound;

    protected:
        Transformable *destroy_effect;

    public:
        Destructible();
        void addDestroyAnimation(Transformator *transformator);
        void addDestroyAnimation(string sheet_name, int color, string preset);
        void addDestructionSound(string sound_name);
        virtual bool isDestroy()=0;
        bool isDestroyAnimationFinish();
        virtual void destroy();
        virtual ~Destructible();
};

#endif // DESTRUCTIBLE_H
