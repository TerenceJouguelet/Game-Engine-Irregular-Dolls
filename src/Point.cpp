#include "Point.h"
#include <iostream>
//#include "System.h"


Point::Point() {
    this->x = 0 ; this->y = 0;
}
Point::Point(double x, double y) {
    this->x = x;
    this->y = y;
}

//La distance entre deux points
double Point::distance(Point other) {
    double xd = x - other.x;
    double yd = y - other.y;
    return sqrt(xd*xd + yd*yd);
}

//opérateurs
Point Point::operator+(Point b)
{
    return Point(x + b.x, y + b.y);
}
Point Point::operator-(Point b)
{
    return Point(x - b.x, y - b.y);
}

Point Point::operator*(double l)
{
    return Point(x*l, y*l);
}

//appliquer une rotation p/r au point O (pivot)
Point Point::rotation(double angle, Point O){
    Point P(0, 0);
    double theta = angle*(3.141/180);
    P.x = O.x+(x-O.x)*cos(theta)-(y-O.y)*sin(theta);
    P.y = O.y+(x-O.x)*sin(theta)+(y-O.y)*cos(theta);
    return P;
}




Math_Vector::Math_Vector(): x(0), y(0){

}
Math_Vector::Math_Vector(double x, double y){
    this->x = x;
    this->y = y;
}

Math_Vector::Math_Vector(Point A, Point B){
    this->x = B.x - A.x;
    this->y = B.y - A.y;
}

//opérateurs
Math_Vector Math_Vector::operator+(Math_Vector b){
    return Math_Vector(x + b.x, y + b.y);
}
Math_Vector Math_Vector::operator-(Math_Vector b){
    return Math_Vector(x - b.x, y - b.y);
}
Math_Vector Math_Vector::operator*(double l){
    return Math_Vector(x*l, y*l);
}

//produit scalaire
double Math_Vector::dotProduct(Math_Vector V){
    return this->x*V.x + this->y*V.y;
}

//norme
double Math_Vector::Norm(){
    return sqrt(x*x + y*y);
}


ostream& operator<<(ostream &strm, Math_Vector V)
{
        strm << "(" << V.x << "," << V.y << ")";
        return strm;
}
