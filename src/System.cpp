#include "System.h"


#define SIZE_FACTOR 1.0

//----------------------------------------
//  Initialisation des valeurs statiques
//----------------------------------------
int System::FPS = 60;

int System::GAME_X = 38*SIZE_FACTOR;
int System::GAME_Y = 19*SIZE_FACTOR;
/*
int System::GAME_WIDTH = 593;
int System::GAME_HEIGHT = 690;
int System::WINDOW_WIDTH = 950;
int System::WINDOW_HEIGHT = 690;*/

int System::GAME_WIDTH = 458*SIZE_FACTOR;
int System::GAME_HEIGHT = 536*SIZE_FACTOR;

int System::WINDOW_WIDTH = 765*SIZE_FACTOR;
int System::WINDOW_HEIGHT = 572*SIZE_FACTOR;

double System::X_GAME_MIDDLE = System::GAME_X + (System::GAME_WIDTH/2);
double System::Y_GAME_MIDDLE = System::GAME_Y + (System::GAME_HEIGHT/2);

double System::GAME_RIGHT = System::GAME_X + System::GAME_WIDTH;
double System::GAME_BOT = System::GAME_Y + System::GAME_HEIGHT;




typedef std::mt19937 MyRNG;
uint32_t seed_val;

MyRNG rng;

void initialize()
{
  rng.seed(seed_val);
}


double System::getRandomNumber(float a, float b){
    std::uniform_real_distribution<double> dis(a, b);
    return dis(rng);
}





//vrai si l'objet de coordonné X, Y est en dehors du jeu
bool System::isOut(double X, double Y, double x_margin, double y_margin){
    if(     isRight(X, x_margin) || isLeft(X, x_margin) || isDown(Y, y_margin) || isUp(Y, y_margin)     )
    {
        return true;
    }
    return false;
}




//vrai si il est a droite
bool System::isRight(double X, double x_margin){
    if(X > System::GAME_WIDTH + System::GAME_X + x_margin){
        return true;
    }
    return false;
}

//etc...
bool System::isLeft(double X, double x_margin){
    if(X < 0 + System::GAME_X - x_margin){
        return true;
    }
    return false;
}
bool System::isDown(double Y, double y_margin){
    if(Y > System::GAME_HEIGHT + System::GAME_Y + y_margin){
        return true;
    }
    return false;
}
bool System::isUp(double Y, double y_margin){
    if(Y < 0 + System::GAME_Y - y_margin){
        return true;
    }
    return false;
}



void System::replaceIfOut(double *X, double *Y, double width, double height){
    if(System::isLeft(*X, -width/2)){
        *X = System::GAME_X + 0 + width/2;
    }
    else if(System::isRight(*X, -width/2)){
        *X = System::GAME_X + System::GAME_WIDTH - width/2;
        //Y += height/2;
    }
    else if(System::isUp(*Y, -height/2)){
        *Y = System::GAME_Y + 0 + height/2;
        //X += width;
    }
    else if(System::isDown(*Y, -height/2)){
        *Y = System::GAME_Y + System::GAME_HEIGHT - height/2;
    }
}




System::System(){

}

System::~System(){

}
