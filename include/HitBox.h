#ifndef HITBOX_H
#define HITBOX_H

#include <math.h>
#include <iostream>
#include "Point.h"



class AABB;
class CBB;
class OBB;

class HitBox
{
    private:
        //les coordonn�es que devrait avoir l'entit� si la collision �tait r�aliste (sans chevauchement)
        int x_bound;
        int y_bound;
        //on a donc aussi besoin de connaitre de quel c�t� vient l'entit�
        int side;

    public:
        //HitBox();
        ~HitBox();

        int getXImpact();
        int getYImpact();


        virtual void updateCara(double X, double Y, double scale, double angle);
        virtual bool overlap(HitBox*)=0;

        virtual bool overlap(AABB*)=0;
        virtual bool overlap(CBB*)=0;

        virtual HitBox* getACopy()=0;

    protected:
        static bool AABB_vs_AABB(AABB* a, AABB* b);
        static bool AABB_vs_CBB(AABB* a, CBB* b);
        static bool CBB_CBB(CBB* a, CBB* b);
        static bool CBB_vs_OBB(CBB* a, OBB* b);

};

class AABB : public HitBox
{
    public:
        int x;
        int y;
        int w;
        int h;
    public:
        AABB(int x, int y, int w, int h);
        void updateCara(double X, double Y, double angle, double scale) override;

        virtual bool overlap(HitBox* b);
        virtual bool overlap(AABB* b);
        virtual bool overlap(CBB* b);

        AABB* getACopy();
};

class CBB : public HitBox
{
    public:
        int x;
        int y;

        int base_r;
        int r;
    public:
        CBB(int x, int y, int r);
        void updateCara(double X, double Y, double angle, double scale) override;

        virtual bool overlap(HitBox* b);
        virtual bool overlap(AABB* b);
        virtual bool overlap(CBB* b);
        virtual bool overlap(OBB* b);

        CBB* getACopy();
};


class OBB : public HitBox
{
    public:
        int x;
        int y;
        int w;
        int h;
        Point O;
        double angle;

        //optimisation: avant de faire tout les tests qui suivent, on va d'abord consid�rer la OBB comme une AABB et voir si il y a colision
        //si il n'y en a pas, alors �a ne serre � rien de tester le reste
        //on recherche donc la plus petite AABB qui contient notre OBB, peut importe son orientation
        //elle � pour hauteur et largeur la longueur des diagonales de l'OBB (voir constructeur)
        int max_size;

    public:
        OBB(int x, int y, int w, int h);
        void updateCara(double X, double Y, double angle, double scale) override;

        virtual bool overlap(HitBox* b);
        virtual bool overlap(AABB* b);
        virtual bool overlap(CBB* b);

        OBB* getACopy();
};



#endif // HITBOX_H
