#include "Positionable.h"
#include "System.h"

Positionable::Positionable(double X, double Y): X(X), Y(Y){
}

/*bool Positionable::isOut(int margin){
    return System::isOut(this->X, this->Y, margin);
}*/


Positionable::~Positionable(){
}
