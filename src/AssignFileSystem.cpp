#include "AssignFileSystem.h"
#include <iostream>
#include "Movable.h"
#include "VarStat.h"
#include "AnimationStat.h"
#include <cstdarg>
#include <string.h>
#include "System.h"

/*#include <boost/assign/std/vector.hpp> // pour avoir 'operator+=()'
#include <boost/assert.hpp>;*/

//using namespace boost::assign;

AssignFileSystem::AssignFileSystem(string path_for_file, string target_ID, string symbol_for_id, string end_instruction){
    ifstream file(path_for_file);
    this->end_instruction = end_instruction;

    if ( !file ){
        cout << "PROBLEME CHEMIN : " << path_for_file << endl;
        throw FileNotFoundException();
    }


    string row;
    bool found = false;
    while (!found && getline( file, row )){
        if(row == symbol_for_id+target_ID){   //la ou il y a l'ID
            found = true;
            this->ID_assign = true;
            this->pos_of_id = 0;
            this->buffer.push_back(row);
        }

    }
    if(!found){
        cout << "ID " << target_ID << " NOT FOUND" << endl;
        throw IDInFileNotFoundException();
    }

    string val;
    found = false;
    while(!found && !file.eof()){
        file >> val;

        this->buffer.push_back(val);
        if(val == "#"){   //la ou il y a la fin de descripteur
            found = true;
        }
    }
    if(file.eof() && !found){
        cout << "End instruction not found durring buffer creation with ID " << target_ID << endl;
        throw IDInFileNotFoundException();
    }

    file.close();
}

void AssignFileSystem::createBufferFromID(string target_ID){

}




bool AssignFileSystem::assignWithLoop(string markup_name, Transformator *transformator){
    if(!this->ID_assign){
        throw IDNotAssignException();
    }
    string val;


    //file.clear();
    int current_pos = this->pos_of_id;
    bool found = false;

    /*
    if(markup_name == "KEY_TRAJ:"){
        //cout << file.tellg() << endl;
        //cout << val << endl;
    }*/
    val = this->buffer[current_pos];
    while(!found && val != this->end_instruction){


        if(val == markup_name){

            //string val;
            found = false;
            while(!found && current_pos < this->buffer.size()){

                val = this->buffer[++current_pos];

                if(val == "{"){   //la ou il y a le debut de la déclaration du groupe de keyframe
                    cout << "'{' found before the number of key" << endl;
                    throw IncorrectDeclarationOfGroupKeysException();
                }
                else if(val == "n_key:"){
                    string n_key = this->buffer[++current_pos];
                    this->n_elements = stoi(n_key);
                    found = true;
                }

            }
            if(current_pos >= this->buffer.size()){
                cout << "'n_key:' not found" << endl;
                throw IncorrectDeclarationOfGroupKeysException();
            }

            int old_id = this->pos_of_id;
            string old_end_instruction = this->end_instruction;

            int pos_loop_begin = current_pos;
            for(int i = 0 ;  i < this->n_elements ; i++){

                current_pos = pos_loop_begin;
                found = false;

                string i_str = to_string(i);

                while(!found && current_pos < this->buffer.size()){
                    val = this->buffer[++current_pos];

                    if(val == i_str+":"){   //la ou il y a le debut de la déclaration du groupe de keyframe
                        found = true;
                        this->ID_assign = true;
                        this->pos_of_id = current_pos;  //on va maintenant chercher à partir d'ici
                    }
                    /*else if(val == "}"){
                        cout << "'}' after '{'" << endl;
                        throw IncorrectDeclarationOfGroupKeysException();
                    }*/
                }

                if(current_pos >= this->buffer.size()){
                    cout << "group declaration not found" << endl;
                    throw IncorrectDeclarationOfGroupKeysException();
                }
                this->end_instruction = "}";

                transformator->assignOneGroupOfKey();     //cette methode est redéfinie dans chaque sous classes car les valeurs à lire dans les groupes seront différentes
            }


            this->pos_of_id = old_id;
            this->end_instruction = old_end_instruction;
            found = true;

        }
        else{
            ++current_pos;
            val = this->buffer[current_pos];
        }
    }
    return found;
}





bool AssignFileSystem::assignSimpleValue(string markup_name, const char* fmt...){
    if(!this->ID_assign){
        throw IDNotAssignException();
    }
    string val;


    //file.clear();
    int current_pos = this->pos_of_id;
    bool found = false;

    /*
    if(markup_name == "KEY_TRAJ:"){
        //cout << file.tellg() << endl;
        //cout << val << endl;
    }*/
    val = this->buffer[current_pos];
    while(!found && val != this->end_instruction){

        if(val == markup_name){

            va_list args;
            va_start(args, fmt);

            string value = this->buffer[++current_pos];

            while (*fmt != '\0'){

                /*if(markup_name == "destroy_anim:"){
                    cout << *fmt << '\t' << value << endl;
                }*/

                if (*fmt == 'i'){
                    int* attribut = va_arg(args, int*);
                    *attribut = stoi(value);
                }
                else if (*fmt == 'f'){
                    float* attribut = va_arg(args, float*);
                    *attribut = stof(value);
                }
                else if (*fmt == 'd'){
                    double* attribut = va_arg(args, double*);
                    *attribut = stod(value);
                }
                else if (*fmt == 's'){
                    string* attribut = va_arg(args, string*);
                    *attribut = (value);
                }
                else if (*fmt == 'b'){
                    bool* attribut = va_arg(args, bool*);
                    *attribut = (value == "true") ? true : false;
                }
                else if (*fmt == 't'){
                    int* attribut = va_arg(args, int*);
                    *attribut = timeToFrame(stof(value));
                }
                else if (*fmt == 'e'){  //chaine de caractère avec espace (le ligne doit contenir un ';' à la fin)
                    string* attribut = va_arg(args, string*);

                    string final_str = "";
                    while(value != ";"){
                        final_str += (value+" ");
                        value = this->buffer[++current_pos];
                    }

                    *attribut = final_str;
                }
                else{
                    throw TypeNotSupportForAssignmentException();
                }
                ++fmt;
                value = this->buffer[++current_pos];

            }

            va_end(args);

            //*v = *(++this->it_file);


            found = true;
        }
        else{
            current_pos++;
            val = this->buffer[current_pos];

        }
    }
    return found;
    /*if(file.eof()){
        cout << "incorrect end_instruction" << endl;
    }*/
}


bool AssignFileSystem::assignListofValue(string markup, vector<string>* AllId){
        int n_values;
        bool found = this->assignSimpleValue("n_"+markup+":", "i", &n_values);
        if(!found){
            cout << "No n_values declared in phase for markup " << markup << endl;
            throw ListDeclarationInFileException();
        }
        for(int i_value = 0 ; i_value < n_values ; i_value++){
            string id_value;
            bool found = this->assignSimpleValue(markup+"_"+to_string(i_value)+":", "s", &id_value);
            if(!found){
                cout << "Declaration #" << i_value << " not found for markup " << markup << endl;
            }
            AllId->push_back(id_value);
        }
}



/*

void AssignFileSystem::assignMultipleValues(vector<string> *v, string markup_name){
    if(!this->ID_assign){
        throw IDNotAssignException();
    }
    string row;
    this->it_file = this->it_target_ID;
    bool found = false;
    while(*this->it_file != "#" && !found){
        if(*this->it_file == markup_name){

            int size = 0;


            values += 1,2,3,4,5,6,7,8,9; // insert values at the end of the container
            BOOST_ASSERT( values.size() == 9 );
            BOOST_ASSERT( values[0] == 1 );
            BOOST_ASSERT( values[8] == 9 );
            *value = stoi(*(++this->it_file));


            found = true;
        }
        getline(this->file, row);
        it_file++;
    }
}
*/

//retourne une valeur aléatoire en lisant l'intervalle écrit dans le fichier texte
void AssignFileSystem::assignRandom(VarStat *vStat){
}


//pour initialiser une stat variable lors de la lecture du fichier AVEC une courbe de bezier
//avec it_file l'itérateur sur le fichier
 void AssignFileSystem::assignValueWithBezierOrRandom(string markup_name, VarStat *vStat, AnimationStat *aStat){
    if(!this->ID_assign){
        throw IDNotAssignException();
    }
    string val;


    //file.clear();
    int current_pos = this->pos_of_id;
    bool found = false;

    /*
    if(markup_name == "KEY_TRAJ:"){
        //cout << file.tellg() << endl;
        //cout << val << endl;
    }*/
    val = this->buffer[current_pos];
    while(!found && val != this->end_instruction){

        if(val == markup_name){

            aStat->changeType("bezier");
            string val = this->buffer[++current_pos];
            if(val == "~Bezier"){
                ++current_pos;
                while(this->buffer[current_pos].find("KEY") != std::string::npos){
                    string key_type = this->buffer[current_pos];
                    double debut = stod(this->buffer[++current_pos]); double fin = stod(this->buffer[++current_pos]); double duree = stod(this->buffer[++current_pos]); string preset = this->buffer[++current_pos];
                    float wait_val = stof(this->buffer[++current_pos]);

                    bool key_loop = false;

                    if(key_type == "KEY_SEG:"){
                        key_loop = (this->buffer[++current_pos] == "true") ? true : false;
                    }

                    //cout << key_loop << endl;

                    Keyframe Key( debut, fin, duree, preset, wait_val, key_loop);
                    aStat->addKeyframe(Key);
                    aStat->update();

                    //cout << *it_file << endl;
                    ++current_pos;
                    this->assignOtherValuesForKeys();
                }
                if(vStat != NULL){
                    if(this->buffer[current_pos] == "~loop"){
                        *vStat = VarStat(aStat->getIterSequence(), true);
                    }
                    else{
                        *vStat = VarStat(aStat->getIterSequence(), false);
                    }
                }
            }
            else if(val == "~Random"){
                //this->assignRandom(vStat);
                float a = stod(this->buffer[++current_pos]);
                float b = stod(this->buffer[++current_pos]);
                *vStat = VarStat(a, b);
            }
            else{
                *vStat = VarStat(stod(val));
            }

            found = true;
        }
        ++current_pos;
        val = this->buffer[current_pos];
    }
}


//rien de base
void AssignFileSystem::assignOtherValuesForKeys(){

}


AssignFileSystem::~AssignFileSystem(){
}








//----------------------------------------------------------------------------------------------------------------------------//
//                                                             SCRIPT                                                         //
//----------------------------------------------------------------------------------------------------------------------------//

Script::Script(string script_name, string path){
    this->script = ifstream(path+script_name+".txt");
    //cout << path+script_name+".txt" << endl;
    this->cursor = istream_iterator<string>(this->script);
    this->end_instruction = "#";
}

void Script::getValues(const char* fmt...){
    va_list args;
    va_start(args, fmt);

    string value;

    while (*fmt != '\0'){


        value = *(++this->cursor);

        if (*fmt == 'i'){
            int* attribut = va_arg(args, int*);
            *attribut = stoi(value);
        }
        else if (*fmt == 'f'){
            float* attribut = va_arg(args, float*);
            *attribut = stof(value);
        }
        else if (*fmt == 'd'){
            double* attribut = va_arg(args, double*);
            *attribut = stod(value);
        }
        else if (*fmt == 's'){
            string* attribut = va_arg(args, string*);
            *attribut = (value);
        }
        else if (*fmt == 'b'){
            bool* attribut = va_arg(args, bool*);
            *attribut = (value == "true") ? true : false;
        }
        else if (*fmt == 't'){
            int* attribut = va_arg(args, int*);
            *attribut = timeToFrame(stof(value));
        }
        else{
            throw TypeNotSupportForAssignmentException();
        }
        ++fmt;

    }

    va_end(args);
}


string Script::getConcatenateString(char delim){
    string row;
    getline(this->script, row, delim);
    this->cursor = istream_iterator<string>(this->script);
    return row;
}


void Script::nextMarkup(){
    string row;
    getline(this->script, row);
    this->cursor = istream_iterator<string>(this->script);
}


bool Script::isMarkupEqual(string markup_name){
    return (*this->cursor) == markup_name;
}


bool Script::isScriptFinished(){
    return (*this->cursor) == this->end_instruction || this->script.eof();
}


















