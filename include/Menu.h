#ifndef MENU_H
#define MENU_H


#include "Background.h"
#include <string>
#include "GameState.h"
#include "Movable.h"
#include "Button.h"
#include "DecorationGroup.h"

using namespace std;
class Game;

class Menu: public GameState
{
    private:
        Drawable background;
        //AutoMovable troll;
        list<Button*> buttons;
        list<Button*>::iterator selector;

        DecorationGroup *deco;

        Timer wait_time_action;


    public:
        Menu(Game *game, string id, string path_text_file);
        void manageEventsState(SDL_Event &event) override;
        void updateState() override;
        void drawState(bool with_animation) override;
        virtual ~Menu();
};

#endif // MENU_H
