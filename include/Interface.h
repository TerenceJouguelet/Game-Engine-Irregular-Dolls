#ifndef INTERFACE_H
#define INTERFACE_H

#include "Player.h"
#include "Drawable.h"

class Interface
{
    private:
        int *n_lives;
        int *n_bombs;

        Drawable *background;

        vector<Drawable*> lives_symbol;
        vector<Drawable*> bombs_symbol;

    public:
        Interface(int *n_lives, int *n_bombs);
        void updateInterface();
        void drawInterface(bool with_animation);
        virtual ~Interface();
};

#endif // INTERFACE_H
