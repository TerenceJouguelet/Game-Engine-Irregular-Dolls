#include "Sheet.h"
#include "Game.h"
#include "System.h"

#include <string>
#include <string.h>
#include <stdexcept>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string.h>


using namespace std;

/*Sheet::Sheet(){
}*/



void Sheet::initializeGlobalsSheets(){
    Sheet::global_sheets = {
        { "Shock_Wave", new Sheet("Shock_Wave") },
        { "Fairy_Maid", new Sheet("Fairy_Maid") },
        { "Spell_Backgound_1", new Sheet("Spell_Backgound_1") },
        { "Foe_Aura", new Sheet("Foe_Aura") },
        { "Satori_Spell_Figure", new Sheet("Satori_Spell_Figure") },
        { "Fire_Ball", new Sheet("Fire_Ball") },
        { "Bullet_Explosion", new Sheet("Bullet_Explosion") },
        { "Box", new Sheet("Box") },
        { "Satori_Portrait", new Sheet("Satori_Portrait") },
        { "Alice_Portrait", new Sheet("Alice_Portrait") },
        { "Black_Image", new Sheet("Black_Image") },
        { "Stage_Background", new Sheet("Stage_Background") },
        { "Spell_Background_3", new Sheet("Spell_Background_3") },
        { "Spell_Background_4", new Sheet("Spell_Background_4") },
        { "Main_Menu_Background", new Sheet("Main_Menu_Background") },
        { "Main_Menu_Alice", new Sheet("Main_Menu_Alice") },
        { "Main_Menu_Logo", new Sheet("Main_Menu_Logo") }

    };
}



map<string, Sheet*> Sheet::global_sheets = {
    /*{ "Shock_Wave", new Sheet("Shock_Wave") },
    { "Fairy_Maid", new Sheet("Fairy_Maid") }*/
};



Sheet* Sheet::newSheet(string name_image){
    map<string, Sheet*>::iterator it_on_sheet = Sheet::global_sheets.find(name_image);
    if(it_on_sheet != Sheet::global_sheets.end()){
        it_on_sheet->second->is_define_globally = true;
        return it_on_sheet->second;
    }
    else{
        Sheet *new_sheet = new Sheet(name_image);
        new_sheet->is_define_globally = false;
        return new_sheet;
    }
}


Sheet* Sheet::newText(string text){

}


void Sheet::deleteSheet(){
    if(!this->is_define_globally){
        delete this;
    }
}



void Sheet::initSheet(string name_image){
    /*if(this->name_image == "Fairy_Maid"){
        cout << "FEEF" << endl;
    }*/

    SDL_Surface *surf_sheet;
    this->colors_in_column = false;



    if(name_image == "Dummy"){
        vector<int> statuts(5, 4);
        surf_sheet = IMG_Load("data/dummy.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }



    //JOUEUR
    else if(name_image == "Marisa"){
        vector<int> statuts(5, 4);
        statuts[0] = 8;
        surf_sheet = IMG_Load("data/joueur/marisa.png");
        this->initInfo(surf_sheet, 8, 6, 5, 6, statuts, 1);
    }
    else if(name_image == "Alice"){
        vector<int> statuts(5, 4);
        surf_sheet = IMG_Load("data/joueur/alice.png");
        this->initInfo(surf_sheet, 4, 1, 5, 0, statuts, 1);
    }
    else if(name_image == "Doll"){
        vector<int> statuts(1, 1);
        surf_sheet = IMG_Load("data/joueur/Doll.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
        //this->directional = false;
    }
    else if(name_image == "Alice_Portrait"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/dialogues/Alice_Portrait.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 5);
    }
    else if(name_image == "Bomb_Sphere"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/joueur/Bomb_Sphere.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }




    else if(name_image == "Player_Aura_1"){
        vector<int> statuts(1, 1);
        surf_sheet = IMG_Load("data/joueur/aura_1.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Player_Aura_2"){
        vector<int> statuts(1, 1);
        surf_sheet = IMG_Load("data/joueur/aura_2.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "shoot_1"){
        vector<int> statuts(1, 1);
        surf_sheet = IMG_Load("data/joueur/shoot_1.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 2);
    }
    else if(name_image == "marisa_shoot_death"){
        vector<int> statuts(1, 4);
        surf_sheet = IMG_Load("data/joueur/marisa_shoot_death.png");
        this->initInfo(surf_sheet, 4, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Death_Circle"){
        vector<int> statuts(1, 1);
        surf_sheet = IMG_Load("data/joueur/Death_Circle.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Player_Death"){
        vector<int> statuts(1, 1);
        surf_sheet = IMG_Load("data/joueur/Player_Death.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }


    //INTERFACE
    else if(name_image == "Background_Interface"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/Interface/Background_Interface.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Life"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/Interface/Life.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 2);
    }
    else if(name_image == "Bomb"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/Interface/Bomb.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 2);
    }





    //FOND
    else if(name_image == "Koishi"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/background/koishi.jpg");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "TROLLFACE"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/background/TROLLFACE.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Pause_Background"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/background/Pause_Background.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Spell_Backgound_1"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/background/Spell_Backgound_1.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Spell_Background_3"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/background/Spell_Background_3.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Spell_Background_4"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/background/Spell_Background_4.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Spell_Background_5"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/background/Spell_Background_5.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Stage_Background"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/background/Stage_Background.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Spell_Name_Style"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/background/Spell_Name_Style.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }




    //PROJECTILES
    else if(name_image == "None"){
        vector<int> statuts(1, 1);
        surf_sheet = IMG_Load("data/projectiles/None.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
        this->directional = false;
    }
    else if(name_image == "Little_Circle"){
        vector<int> statuts(1, 1);
        surf_sheet = IMG_Load("data/projectiles/Little_Circle.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 16);
        this->directional = false;
    }
    else if(name_image == "Circle_Halo"){
        vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/projectiles/Circle_Halo.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 16);
        this->directional = false;
    }
    else if(name_image == "Big_Circle"){
        vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/projectiles/Big_Circle.png");
        this->initInfo(surf_sheet, 1, 5, 1, 0, statuts, 8);
        this->directional = false;
    }
    else if(name_image == "Tiny_Circle"){
        vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/projectiles/Tiny_Circle.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 16);
        this->directional = false;
    }
    else if(name_image == "Fire_Ball"){
        vector<int> statuts(1,4);
        surf_sheet = IMG_Load("data/projectiles/Fire_Ball.png");
        this->initInfo(surf_sheet, 4, 10, 1, 0, statuts, 2);
    }
    else if(name_image == "Big_Star"){
        vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/projectiles/Big_Star.png");
        this->initInfo(surf_sheet, 1, 2, 1, 0, statuts, 8);
    }
    else if(name_image == "Petal"){
        vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/projectiles/Petal.png");
        this->initInfo(surf_sheet, 1, 2, 1, 0, statuts, 8);
    }
    else if(name_image == "Big_Pill"){
        vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/projectiles/Big_Pill.png");
        this->initInfo(surf_sheet, 1, 95, 1, 0, statuts, 8);
    }
    else if(name_image == "Card"){
        vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/projectiles/Card.png");
        this->initInfo(surf_sheet, 1, 4, 1, 0, statuts, 16);
    }
    else if(name_image == "Big_Card"){
        vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/projectiles/Big_Card.png");
        this->initInfo(surf_sheet, 1, 20, 1, 0, statuts, 16);
    }
    else if(name_image == "Butterfly"){
        vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/projectiles/Butterfly.png");
        this->initInfo(surf_sheet, 1, 3, 1, 0, statuts, 8);
    }
    else if(name_image == "Little_Pill"){
        vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/projectiles/Little_Pill.png");
        this->initInfo(surf_sheet, 1, 8, 1, 0, statuts, 16);
    }
    else if(name_image == "Bullet_Explosion"){
        vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/projectiles/Bullet_Explosion.png");
        this->initInfo(surf_sheet, 1, 6, 1, 0, statuts, 8);
    }
    else if(name_image == "Pill"){
        vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/projectiles/Pill.png");
        this->initInfo(surf_sheet, 1, 17, 1, 0, statuts, 8);
    }
    else if(name_image == "Arrow"){
        vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/projectiles/Arrow.png");
        this->initInfo(surf_sheet, 1, 4, 1, 0, statuts, 16);
    }
    else if(name_image == "Elec_Circle"){
        vector<int> statuts(1,4);
        surf_sheet = IMG_Load("data/projectiles/Elec_Circle.png");
        this->initInfo(surf_sheet, 4, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Ice"){
        vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/projectiles/Ice.png");
        this->initInfo(surf_sheet, 1, 8, 1, 0, statuts, 16);
    }
    else if(name_image == "Little_Star"){
        vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/projectiles/Little_Star.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 16);
    }
    else if(name_image == "Light_Circle"){
        vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/projectiles/Light_Circle.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 8);
        this->directional = false;
    }
    else if(name_image == "Tear"){
        vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/projectiles/Tear.png");
        this->initInfo(surf_sheet, 1, 6, 1, 0, statuts, 16);
    }




    //ENNEMIS
    else if(name_image == "Fairy_Maid"){
        vector<int> statuts(5, 4);
        surf_sheet = IMG_Load("data/ennemis/Fairy_Maid.png");
        this->initInfo(surf_sheet, 4, 11, 5, 4, statuts, 3);
        this->directional = false;
    }
    else if(name_image == "Fairy"){
        vector<int> statuts(5, 4);
        surf_sheet = IMG_Load("data/ennemis/Fairy.png");
        this->initInfo(surf_sheet, 4, 11, 5, 4, statuts, 3);
        this->directional = false;
    }
    else if(name_image == "Fairy_Kamikaze"){
        vector<int> statuts(1, 4);
        surf_sheet = IMG_Load("data/ennemis/Fairy_Kamikaze.png");
        this->initInfo(surf_sheet, 4, 11, 1, 0, statuts, 1);
    }
    else if(name_image == "Fairy_Sunflower"){
        vector<int> statuts(5, 4);
        surf_sheet = IMG_Load("data/ennemis/Fairy_Sunflower.png");
        this->initInfo(surf_sheet, 4, 0, 5, 0, statuts, 1);
        this->directional = false;
    }
    else if(name_image == "Fairy_Shield"){
        vector<int> statuts(5, 4);
        surf_sheet = IMG_Load("data/ennemis/Fairy_Shield.png");
        this->initInfo(surf_sheet, 4, 0, 5, 3, statuts, 1);
        this->directional = false;
    }
    else if(name_image == "Shock_Wave"){
        vector<int> statuts(1, 8);
        surf_sheet = IMG_Load("data/ennemis/Shock_Wave.png");
        this->colors_in_column = true;
        this->initInfo(surf_sheet, 8, 0, 1, 0, statuts, 4);
    }
    else if(name_image == "Foe_Aura"){
        vector<int> statuts(1, 8);
        surf_sheet = IMG_Load("data/ennemis/Foe_Aura.png");
        this->colors_in_column = true;
        this->initInfo(surf_sheet, 8, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "!!!!"){
        vector<int> statuts(1, 1);
        surf_sheet = IMG_Load("data/ennemis/!!!!.png");
        this->colors_in_column = true;
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }



    //BOSS
    else if(name_image == "Satori"){
        vector<int> statuts(5, 2);
        statuts[0] = 8;
        statuts[1] = 4;
        statuts[3] = 4;
        surf_sheet = IMG_Load("data/ennemis/Satori.png");
        this->initInfo(surf_sheet, 8, 0, 5, 0, statuts, 1);
        this->directional = false;
    }
    else if(name_image == "Satori_Spell_Figure"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/ennemis/Satori_Portrait.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Satori_Portrait"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/dialogues/Satori_Portrait.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 4);
    }
    else if(name_image == "Box"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/dialogues/Box.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Ennemy_Indicator"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/ennemis/Ennemy_Indicator.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }


    //MENU
    else if(name_image == "Start_Button"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/Menu/Start_Button.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Quit_Button"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/Menu/Quit_Button.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Credit_Button"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/Menu/Credits_Button.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Main_Menu_Background"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/Menu/Main_Menu_Background.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Main_Menu_Alice"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/Menu/Main_Menu_Alice.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Main_Menu_Logo"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/Menu/Main_Menu_Logo.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Credit"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/Menu/Credit.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }


    else if(name_image == "Return_Game_Button"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/Menu/Return_Game_Button.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Return_Menu_Button_In_Pause"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/Menu/Return_Menu_Button_In_Pause.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Restart_In_Pause"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/Menu/Restart_In_Pause.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }

    else if(name_image == "Restart_Button"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/Menu/Restart_Button.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Return_Menu_Button"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/Menu/Return_Menu_Button.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Black_Image"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/Menu/Black_Image.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }





    //DIAPO
    else if(name_image == "Loading_1"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/Panorama/Loading_1.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Loading_2"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/Panorama/Loading_2.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }
    else if(name_image == "Narration_Box"){
        std::vector<int> statuts(1,1);
        surf_sheet = IMG_Load("data/Panorama/Narration_Box.png");
        this->initInfo(surf_sheet, 1, 0, 1, 0, statuts, 1);
    }






    else{
        cout << "Le nom [" << name_image << "] de la feuille n'est pas bon" << endl;
        throw new invalid_argument("Le nom de la feuille n'est pas bon");
    }

    if(surf_sheet == NULL){
        cout << "Mauvais chemin vers la feuille" << endl;
        throw new invalid_argument("Mauvais chemin vers la feuille");
    }

    this->texture_sheet = SDL_CreateTextureFromSurface(Game::renderer, surf_sheet); // Préparation du sprite
    SDL_FreeSurface(surf_sheet);
}

//renvoie la hitbox correspondant au sprites de cette feuille
HitBox* Sheet::createHitBoxBySheetName(string sheet_name){
    if(sheet_name == "Marisa" || sheet_name == "Alice"){
        return new CBB(0, 0, 1.0);
    }
    else if(sheet_name == "shoot_1"){
        return new AABB(0, 0, 12, 20);  //h:31
    }
    else if(sheet_name == "Death_Circle"){
        return new CBB(0, 0, 200);
    }
    else if(sheet_name == "Doll"){
        return new OBB(0, 0, 13, 16);
    }
    else if(sheet_name == "Bomb_Sphere"){
        return new CBB(0, 0, 131/2);
    }


    else if(sheet_name == "Fire_Ball"){
        return new CBB(0, 0, 6.5);
    }
    else if(sheet_name == "Big_Star"){
        return new AABB(0, 0, 25, 24);  //reel : 30w 28h
    }
    else if(sheet_name == "Petal"){
        return new CBB(0, 0, 11);
    }
    else if(sheet_name == "Big_Pill"){
        return new OBB(0, 0, 70, 140);
    }
    else if(sheet_name == "Card"){
        return new OBB(0, 0, 12, 14);
    }
    else if(sheet_name == "Big_Card"){
        return new OBB(0, 0, 12*5, 14*5);
    }
    else if(sheet_name == "Butterfly"){
        return new CBB(0, 0, 6.5);
    }
    else if(sheet_name == "Big_Circle"){
        return new CBB(0, 0, 13.5);
    }
    else if(sheet_name == "Little_Pill"){
        return new OBB(0, 0, 7, 12);
    }
    else if(sheet_name == "Little_Circle"){
        return new CBB(0, 0, 7);
    }
    else if(sheet_name == "Pill"){
        return new OBB(0, 0, 10, 24);
    }
    else if(sheet_name == "Arrow"){
        return new OBB(0, 0, 10, 13);
    }
    else if(sheet_name == "Elec_Circle"){
        return new CBB(0, 0, 13.5);
    }
    else if(sheet_name == "Ice"){
        return new OBB(0, 0, 6, 14);
    }
    else if(sheet_name == "Little_Star"){
        return new CBB(0, 0, 5);
    }
    else if(sheet_name == "Light_Circle"){
        return new CBB(0, 0, 7);
    }
    else if(sheet_name == "Tear"){
        return new OBB(0, 0, 6, 10);
    }
    else if(sheet_name == "Tiny_Circle"){
        return new CBB(0, 0, 4);
    }



    else if(sheet_name == "Dummy"){
        return new AABB(0, 0, 0, 0);
    }
    else if(sheet_name == "Fairy_Maid"){
        return new AABB(0, 0, 21, 28);
    }
    else if(sheet_name == "Fairy"){
        return new AABB(0, 0, 21, 28);
    }
    else if(sheet_name == "Fairy_Kamikaze"){
        return new AABB(0, 0, 21, 28);
    }
    else if(sheet_name == "Fairy_Sunflower"){
        return new AABB(0, 0, 66, 52);
    }
    else if(sheet_name == "Fairy_Shield"){
        return new AABB(0, 0, 42, 42);
    }
    else if(sheet_name == "Satori"){
        return new AABB(0, 0, 39, 59);
    }
    else if(sheet_name == "!!!!"){
        return new AABB(0, 0, 10, 10);
    }
}


HitBox* Sheet::createHitBoxCorresponding(){
    return Sheet::createHitBoxBySheetName(this->name_image);
}

void Sheet::initInfo(SDL_Surface *surf_sheet, int W_Elements, int x_gap_size, int H_Elements, int y_gap_size, vector<int>& statuts, int nb_color){
    this->statuts = statuts;
    this->W_Elements = W_Elements;
    this->H_Elements = H_Elements;

    if(!this->colors_in_column){
        this->W_entity = (surf_sheet->w - ((W_Elements*nb_color)-1)*x_gap_size)/(W_Elements*nb_color);
        this->H_entity = (surf_sheet->h - (H_Elements-1)*y_gap_size)/(H_Elements);
    }
    else{
        this->W_entity = (surf_sheet->w - (W_Elements-1)*x_gap_size)/(W_Elements);
        this->H_entity = (surf_sheet->h - ((H_Elements*nb_color)-1)*y_gap_size)/(H_Elements*nb_color);
    }
    this->nb_color = nb_color;

    this->x_gap_size = x_gap_size;
    this->y_gap_size = y_gap_size;
}


Sheet::Sheet(string name_image){

    this->directional = true;
    this->name_image = name_image;
    /*if(surf_sheet == NULL){
        char error[40];
        sprintf(error, "L'image %s n'a pas ete trouve\n", nom_image);
        throw std::invalid_argument(error);
    }*/
    this->initSheet(name_image);

    int max_W;
    int max_H;
    if(this->colors_in_column){
        max_W = W_Elements;
        max_H = H_Elements*nb_color;
    }
    else{
        max_W = W_Elements*nb_color;
        max_H = H_Elements;
    }

    for(int i = 0 ; i < max_W ; i++){
        vector<SDL_Rect> column;
        for(int j = 0 ; j < max_H ; j++){
            SDL_Rect clip;
            clip.h = H_entity;
            clip.w = W_entity;
            clip.x = i*W_entity + i*x_gap_size;
            clip.y = j*H_entity + j*y_gap_size;

            /*if(name_image == "Shock_Wave"){
                cout << this->W_entity << "\t" << this->H_entity << endl;
            }*/
            column.push_back( clip );
        }
        this->clips.push_back(column);
    }
}

string Sheet::getSheetName(){
    return this->name_image;
}


//renvoie le clip correspondant au bon sprite de la feuille en fonction de ces paramètres
SDL_Rect* Sheet::getReferenceOfClip(int frame, int statut, int color){
    SDL_Rect *clip;
    if(!this->colors_in_column){
        clip = &this->clips[frame + color*this->W_Elements][statut];
    }
    else{
        clip = &this->clips[frame][color];
    }
    return clip;
}


void Sheet::updateRect(SDL_Rect* rect, double X, double Y) const{
    rect->x = X - this->W_entity/2;
    rect->y = Y - this->H_entity/2;
    rect->h = this->H_entity;
    rect->w = this->W_entity;
}



void Sheet::blitImage(SDL_Rect* rect, SDL_Rect* clip){
    SDL_Rect* temp = rect;
    temp->w *= Game::X_current_window_scale;
    temp->h *= Game::Y_current_window_scale;
    temp->x = temp->x*Game::X_current_window_scale + Game::X_current_window_offset;
    temp->y *= Game::Y_current_window_scale;
    SDL_RenderCopy(Game::renderer, this->texture_sheet ,clip , temp); // Copie du sprite grâce au SDL_Renderer

    //SDL_BlitSurface(this->surf_sheet, clip, Game::window, temp);

    if(0/*(this->W_Elements != 1)*/){
        /*SDL_Rect clip;
        clip.x = this->W_entity*this->x_current;
        clip.w = clip.x + this->W_entity;

        clip.y = this->H_entity*this->y_current;
        clip.h = clip.y + this->H_entity;

        SDL_BlitSurface(this->surf_sheet, &clip, Game::window, temp);*/
    }
}

void Sheet::blitTransformedImage(SDL_Rect* rect, SDL_Rect* clip, double angle, double scale, double alpha, bool flip){

    SDL_RendererFlip flag = (flip) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    SDL_Rect temp = *rect;

    if(scale != 1){
        temp.h = rect->h*scale;
        temp.w = rect->w*scale;
        //cout << temp.w << '\t' << rect->h << endl;
        temp.x = rect->x - (temp.w - rect->w)/2;
        temp.y = rect->y - (temp.h - rect->h)/2;
    }
    if(alpha != 0 || alpha != 255){
        SDL_SetTextureAlphaMod(this->texture_sheet, alpha);
    }

    temp.w *= Game::X_current_window_scale;
    temp.h *= Game::Y_current_window_scale;
    temp.x = temp.x*Game::X_current_window_scale + Game::X_current_window_offset;
    temp.y *= Game::Y_current_window_scale;

    double final_angle = angle;
    if(!this->directional){
        final_angle = 0;
    }



    /*if(this->name_image == "Foe_Aura"){
        cout << temp.x << '\t' << temp.y << '\t' << alpha << endl;
    }*/

    SDL_RenderCopyEx(Game::renderer, this->texture_sheet, clip, &temp, final_angle, NULL, flag);
}


//renvoie le gestionnaire d'animation correspondant à cette feuille
AnimSystem* Sheet::createAnimCorresponding(){
    if(this->H_Elements <= 1 && this->W_Elements <= 1){
        return nullptr;
    }

    float delay = 0.1;
    bool loop = false;
    if(this->name_image == "Fire_Ball"){
        delay = 0.05;
        loop = true;
    }
    else if(this->name_image == "Marisa" || this->name_image == "Alice"){
        delay = 0.1;
        loop = true;
    }
    else if(this->name_image == "Fairy_Maid"){
        delay = 0.1;
        loop = true;
    }
    else if(this->name_image == "Fairy" || this->name_image == "Fairy_Sunflower"
            || this->name_image == "Fairy_Shield" || this->name_image == "Fairy_Kamikaze"){
        delay = 0.1;
        loop = true;
    }
    else if(name_image == "Shock_Wave"){
        delay = 0.05;
        loop = false;
    }
    else if(name_image == "marisa_shoot_death"){
        delay = 0.05;
        loop = false;
    }
    else if(name_image == "Satori"){
        delay = 0.15;
        loop = true;
    }
    else if(name_image == "Foe_Aura"){
        delay = 0.1;
        loop = true;
    }
    if(this->name_image == "Elec_Circle"){
        delay = 0.05;
        loop = true;
    }

    AnimSystem* anim = new AnimSystem(this->statuts, delay, loop);
    return anim;
}


Sheet::~Sheet(){
    SDL_DestroyTexture(this->texture_sheet);
}









//---------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------LifeBar----------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------//

LifeBar::LifeBar(int max_value, string color){
    const int margin = 20;

    this->bar.x = System::GAME_X + margin;
    this->bar.y = System::GAME_Y + 10;
    this->max_value = max_value;
    this->max_width = margin + 345;
    this->bar.h = 3;   //épaisseur

    if(color == "violet"){
       this->R = 156;
       this->G = 0;
       this->B = 255;
    }
    else{
       this->R = 255;
       this->G = 255;
       this->B = 255;
    }
}


void LifeBar::Draw(int current_value){
    int current_width = (current_value*this->max_width)/this->max_value;
    this->bar.w = current_width;

    SDL_Rect temp = this->bar;
    temp.x = temp.x*Game::X_current_window_scale + Game::X_current_window_offset;;
    temp.w *= Game::X_current_window_scale;
    temp.y *= Game::Y_current_window_scale;
    temp.h *= Game::Y_current_window_scale;



    //ombre portée
    SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_Rect shadow = temp;
    shadow.x += 3;
    shadow.y += 3;
    SDL_RenderDrawRect(Game::renderer, &shadow);
    SDL_RenderFillRect(Game::renderer, &shadow);


    SDL_SetRenderDrawColor(Game::renderer, this->R, this->G, this->B, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(Game::renderer, &temp);
    SDL_RenderFillRect(Game::renderer, &temp);

    SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

