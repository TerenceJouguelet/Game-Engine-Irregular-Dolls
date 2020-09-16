#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <map>
#include <list>
#include <iostream>
#include <string>
#include "AssignFileSystem.h"
#include "Timer.h"
#include "Sound.h"

using namespace std;

class AutoMovable;
class Text;
class Transformable;

enum Expression{NEUTRAL, ANGRY, EMBARRASSED , SMILE, SAD, SURPRISED};
enum Side{LEFT_SIDE, RIGHT_SIDE};


class Character{
    private:
        AutoMovable *portrait;
        Text *line;
        Text *full_name;
        Expression exp;
        Side side;
        //string full_name;
        Sound speak_sound;

        string current_line_progression;
        string full_line;
        const char *cursor_char;
        bool line_finished;

    public:
        Character(string id_for_lines, string name, string portrait_sheet_name, string speak_sound_name);
        void changeExprsession(string new_expression);
        void changeSpeakSound(string sound_name);
        void changeTextPosition();
        void changePortraitSide(string new_side);
        void changePortraitTrajectory(string id_of_trajectory);

        void movePortrait();
        void callInPortrait();
        void callOutPortrait();
        void launchPortraitEndAnimation();

        void affectNewLine(string new_line);
        void updateLineProgression();
        void skipScroll();
        void printLine(bool with_animation);
        void drawFullName(bool with_animation);
        void drawPortrait(bool with_animation);

        virtual ~Character();

};


class Dialogue
{
    public:
        static map<string, Expression> exp_map;

    private:
        Script script;
        map<string, Character*> characters; //la liste des personnages concernés avec leur nom
        Character *current_show;    //pointeur vers le personnage qui parle à l'écran
        AutoMovable *box;   //la boite de dialogue

        bool finished;

        bool time_mode; //vrai si la prochaine ligne de dialogue doit se lancer au bout d'un certain temps
        Timer text_duration;    //ce temps

        bool narration_mode;    //vrai si l'on est en mode narration (la narrateur parle)

        int scroll_cadence;
        unsigned int time;
        Timer enter_delay;  //temps que met le personnage pour entrer en scène (animation)
        Timer exit_delay;   //temps que met le personnage pour sortir (animation)

    public:
        Dialogue(string dialogue_name, string path);
        void skipScroll();
        void skipSection();
        void updateDialogue();
        bool isFinished();
        void Draw(bool with_animation);
        virtual ~Dialogue();

    private:
        void showAthotherCharacter(string name);
        void letInAthotherCharacter(string name);
        void launchEndOfDialogue();
};



class NoCharacterShowException{};
class IncorrectCharactersDeclarationException{};


#endif // DIALOGUE_H
