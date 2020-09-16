#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <string>
#include <SDL2/SDL.h>
#include <iostream>
#include "Movable.h"




using namespace std;

class Background : public AutoMovable
{
    private:

    public:
        Background(string id);
        //void Draw(bool with_animation);
        virtual ~Background();
};

#endif // BACKGROUND_H
