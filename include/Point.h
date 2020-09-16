#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <math.h>

using namespace std;

class Point{
    public:
        double x, y;
    public:
        Point();
        Point(double x, double y);

        //La distance entre deux points
        double distance(Point other);

        //opérateurs
        Point operator+(Point b);
        Point operator-(Point b);
        Point operator*(double l);

        //appliquer une rotation p/r au point O (pivot)
        Point rotation(double angle, Point O);

        //pour l'afficher
        friend ostream& operator<<(ostream &strm, const Point& P)
        {
            strm << "(" << P.x << "," << P.y << ")";
            return strm;
        }
};


class Math_Vector{
    public:
        double x, y;
    public:
        Math_Vector();
        Math_Vector(double x, double y);
        Math_Vector(Point A, Point B);

        //opérateurs
        Math_Vector operator+(Math_Vector b);
        Math_Vector operator-(Math_Vector b);
        Math_Vector operator*(double l);

        //produit scalaire
        double dotProduct(Math_Vector);

        //norme
        double Norm();

        //pour l'afficher
        friend ostream& operator<<(ostream &strm, const Math_Vector& V)
        {
            strm << "(" << V.x << "," << V.y << ")";
            return strm;
        }
};

#endif // POINT_H
