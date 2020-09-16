#ifndef VECTOR2D_H
#define VECTOR2D_H

class Vector2D{
    public:
        double lambda;
        double theta;

        Vector2D();
        Vector2D(double lambda, double theta);
        void changeVal(double lambda, double theta);

        //renvoie les coordonnées cathésiennes
        double getCarthX();
        double getCarthY();
        ~Vector2D();
};

#endif // VECTOR2D_H
