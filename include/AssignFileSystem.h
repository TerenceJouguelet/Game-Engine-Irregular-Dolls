#ifndef ASSIGNFILESYSTEM_H
#define ASSIGNFILESYSTEM_H

//Hanakoyurist

#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <list>

class Transformator;

using namespace std;

class Handler;
class VarStat;
class AnimationStat;
class Movator;


class AssignFileSystem
{
    protected:
        vector<string> buffer;

        bool ID_assign; //vrai si l'on a assigner un identifient
        int pos_of_id; //La position du début de notre descripteur (là ou est écrit l'identifiant)
        int n_elements; //dans le cas ou le nombre de valeur (ou groupe de valeur) à assigner n'est pas connu à l'avance,
        string end_instruction;  //on arrête la recherche de la valeur à assigner lorsque l'on trouve cette instruction

    public:
        AssignFileSystem(string path_for_file, string target_ID, string symbol_for_id, string end_instruction);
        void createBufferFromID(string target_ID);

        bool assignSimpleValue(string markup_name, const char* fmt...);
        bool assignListofValue(string markup, vector<string>* listOfID);
        void assignValueWithBezierOrRandom(string markup_name, VarStat *vStat, AnimationStat *aStat);
        bool assignWithLoop(string markup_name, Transformator *transformator);

        virtual void assignOtherValuesForKeys();

        void assignRandom(VarStat *vStat);

        virtual ~AssignFileSystem();

    private:
        //void assignVarStatBezier(VarStat *vStat, AnimationStat *aStat);

};



class Script{
    private:
        string end_instruction;
        istream_iterator<string> cursor;
        ifstream script;

    public:
        Script(string script_name, string path);
        void getValues(const char* fmt...);
        string getConcatenateString(char delim);
        void nextMarkup();
        bool isMarkupEqual(string markup_name);
        bool isScriptFinished();

};



class FileNotFoundException{};
class IDInFileNotFoundException{};
class IDNotAssignException{};
class TypeNotSupportForAssignmentException{};
class IncorrectDeclarationOfGroupKeysException{};
class ListDeclarationInFileException{};

#endif // ASSIGNFILESYSTEM_H
