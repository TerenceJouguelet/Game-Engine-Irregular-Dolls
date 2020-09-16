#ifndef SYSTEM_H
#define SYSTEM_H

#include <random>

#define MY_PI 3.141
#define radToDeg(angle) angle*(180/MY_PI)
#define degToRad(angle) angle*(MY_PI/180)
#define timeToFrame(time) (time*System::FPS)
#define frameToTime(frame) frame/(float)System::FPS
//#define SCALE_ELEMENT 1.5

using namespace std;



class System
{
    public:
        //elements statiques
        static int FPS; //nombre d'image par secondes
        static int GAME_WIDTH;  //proporion de l'écran de jeu
        static int GAME_HEIGHT; //

        static int GAME_X;  //prosition e l'écran de jeu par rapport à la fenêtre
        static int GAME_Y; //

        static int WINDOW_WIDTH;    //proportion de la fenêtre
        static int WINDOW_HEIGHT;   //

        static double X_GAME_MIDDLE;    //le milieu
        static double Y_GAME_MIDDLE;   //

        static double GAME_RIGHT;    //le bord droit
        static double GAME_BOT;   //le bord bas

        static std::mt19937 gen; //Standard mersenne_twister_engine seeded with rd()
        static double getRandomNumber(float a, float b); //entre a et b


        static bool isRight(double X, double x_margin);
        static bool isLeft(double X, double x_margin);
        static bool isUp(double Y, double y_margin);
        static bool isDown(double Y, double y_margin);
        static bool isOut(double X, double Y, double x_margin, double y_margin);

        static void replaceIfOut(double *X, double *Y, double width, double height);

    public:
        System();
        virtual ~System();

};

#endif // SYSTEM_H
