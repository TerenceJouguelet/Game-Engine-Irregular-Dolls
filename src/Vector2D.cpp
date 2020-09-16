#include "Vector2D.h"
#include "math.h"
#include "System.h"

Vector2D::Vector2D(){
    this->lambda = 0;
    this->theta = 0;
}
Vector2D::Vector2D(double lambda, double theta){
    this->lambda = lambda;
    this->theta = theta;
}

void Vector2D::changeVal(double lambda, double theta){
    this->lambda = lambda;
    this->theta = theta;
}


double Vector2D::getCarthX(){
    return this->lambda*cos(degToRad(this->theta));
}

double Vector2D::getCarthY(){
    return this->lambda*sin(degToRad(this->theta));
}


Vector2D::~Vector2D()
{

}
