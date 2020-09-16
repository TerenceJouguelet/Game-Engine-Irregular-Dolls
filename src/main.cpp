#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <SDL2/SDL.h>
/*#include <SDL/SDL_getenv.h>*/
#include <iostream>
#include <stdio.h>
#include "Game.h"
#include "System.h"

//#define SDL_LoadFile(file) SDL_LoadFile_RW(SDL_RWFromFile(file, "rb"), 1)

using namespace std;

int main ( int argc, char** argv )
{



    Game *game = new Game();
    game->initialization("Touhou : Irregular Dolls", 500, 200, false);


    //boucle principale
    while(game->isRunning()){
        int tempsPrec = SDL_GetTicks();

        //------------------------------------------------------------------------------------------------
        //                                          TRAITEMENT
        //------------------------------------------------------------------------------------------------
        //cout <<(SDL_GetTicks() - tempsPrec) << '\t';
        game->manageEvents();
        game->update();

        //------------------------------------------------------------------------------------------------
        //                                           DESSIN
        //------------------------------------------------------------------------------------------------

        game->render();


        //------------------------------------
        // Gestion du framerate (à améliorer)
        //------------------------------------
		int	tempsActu = SDL_GetTicks();
		if (tempsActu - tempsPrec < 1000/System::FPS)
        {
            if(System::FPS - (tempsActu - tempsPrec) >= 0){
                //On stoppe le programe pendant un certain temps pour ne pas "surcharger" le processeur
                SDL_Delay(1000/System::FPS - (tempsActu - tempsPrec));
            }
            else{
                SDL_Delay(1000/System::FPS - (1));
            }
        }
        //printf("%d\n", FPS - (tempsActu - tempsPrec));
        //cout << System::FPS - (tempsActu - tempsPrec) << endl;
		tempsPrec = SDL_GetTicks();


    }

    game->clean();
    delete game;

    return 2;
}

