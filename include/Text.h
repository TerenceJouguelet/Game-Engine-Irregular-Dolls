#ifndef TEXT_H
#define TEXT_H

#include "Movable.h"

class Text: public AutoMovable{
    private:
        FC_Font *font;
        string cpp_string;
        const char *text;
        float x, y;
        float size;

        //couleur
        bool loop_color;
        int number_of_def_color_key;
        AnimationStat a_R, a_G, a_B;
        VarStat R, G, B;

        int shadow;
        string font_name;
        FC_AlignEnum alignment;

    public:
        Text(string id, string path_text_file);
        void changeText(string new_text);
        FC_Font * getFCFont();
        void Draw(bool with_animation) override;
        ~Text();
    protected:
        void assignAttributsWithTextFile() override;
        void assignOneGroupOfKey() override;

};

#endif // TEXT_H
