#ifndef DECORATIONGROUP_H
#define DECORATIONGROUP_H

#include "AssignFileSystem.h"
#include <vector>
#include <iostream>

class AutoMovable;

using namespace std;

class DecorationGroup
{
    private:
        vector<AutoMovable*> decorations;

    public:
        DecorationGroup(AssignFileSystem *file_assigner, string id_of_list, string path_text_file_for_deco);
        void moveDecorations();
        void drawDecorations(bool with_animation);
        virtual ~DecorationGroup();
};

#endif // DECORATIONGROUP_H
