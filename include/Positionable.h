#ifndef POSITIONABLE_H
#define POSITIONABLE_H


class Positionable
{
    protected:
        double X;
        double Y;
    public:
        Positionable(double X, double Y);
        /*bool isOut(int margin);*/
        virtual ~Positionable();
};

#endif // POSITIONABLE_H
