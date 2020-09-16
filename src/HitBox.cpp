#include "HitBox.h"
#include "math.h"

#include <iostream>

#define LEFT 0
#define RIGHT 1
#define BOT 2
#define TOP 3


using namespace std;
/*HitBox::HitBox()
{
    //ctor
}*/


bool CollisionPointCircle(Point &A, CBB &C)
{
    Point center(C.x, C.y);
    int d = center.distance(A);
    if (d > C.r){
        return false;
    }
    return true;
}


bool CollisionLineCircle(Point &A, Point &B, CBB &C)
{
    Math_Vector u(A, B);
    Point center(C.x, C.y);
    Math_Vector AC(A, center);
    float numerator = fabs(u.x*AC.y - u.y*AC.x);
    float denominator  =  u.Norm();  // norme de u

    float normal = numerator / denominator;
    if (normal < C.r){
        return true;
    }
    return false;
}

bool CollisionSegmentCircle(Point &A, Point &B, CBB &C)
{
    if(!CollisionLineCircle(A, B, C)){
        return false;
    }

    Point center(C.x, C.y);
    Math_Vector AB(A, B);
    Math_Vector AC(A, center);
    Math_Vector BC(B, center);

    float dotS1 = AB.dotProduct(AC);
    float dotS2 = -AB.dotProduct(BC);
    if(dotS1 >= 0 && dotS2 >= 0){
        return true;
    }
    //A ou B peuvent être dans le cercle
    if(CollisionPointCircle(A, C)){
        return true;
    }
    if(CollisionPointCircle(B, C)){
        return true;
    }
    return false;
}








HitBox::~HitBox()
{

}

int HitBox::getXImpact(){
    return this->x_bound;
}

int HitBox::getYImpact(){
    return this->y_bound;
}


void HitBox::updateCara(double X, double Y, double angle, double scale){
    //par défaut (si on arrive pas à trouvée les coordonnées réalistes)
    this->x_bound = round(X);
    this->y_bound = round(Y);
}


bool HitBox::AABB_vs_AABB(AABB* hb1, AABB* hb2){

    if(hb2->x >= hb1->x + hb1->w){
        hb1->side = LEFT;
        hb2->side = RIGHT;
        return false;
    }
    if(hb2->x + hb2->w <= hb1->x){
        hb1->side = RIGHT;
        hb2->side = LEFT;
        return false;
    }
	if(hb2->y >= hb1->y + hb1->h){
	    hb1->side = TOP;
        hb2->side = BOT;
        return false;
	}
	if(hb2->y + hb2->h <= hb1->y){
	    hb1->side = BOT;
        hb2->side = TOP;
        return false;
	}

	if(hb1->side == BOT || hb2->side == BOT){
        hb1->y_bound = (hb2->y + hb2->h) + hb1->h/2;
        hb2->y_bound = (hb1->y + hb1->h) + hb2->h/2;
	}
	else if(hb1->side == TOP || hb2->side == TOP){
        hb1->y_bound = (hb2->y) - hb1->h/2;
        hb2->y_bound = (hb1->y) - hb2->h/2;
	}
	else if(hb1->side == RIGHT || hb2->side == RIGHT){
        /*hb1->x_bound = (hb2->x + hb2->w) + hb1->w/2;
        hb2->x_bound = (hb1->x + hb2->w) + hb2->w/2;*/
        hb1->y_bound = (hb2->y + hb2->h) + hb1->h/2;
        hb2->y_bound = (hb1->y + hb1->h) + hb2->h/2;
	}
	else if(hb1->side == LEFT || hb2->side == LEFT){
        /*hb1->x_bound = (hb2->x) + hb1->w/2;
        hb2->x_bound = (hb1->x) + hb2->w/2;*/
        hb1->y_bound = (hb2->y + hb2->h) + hb1->h/2;
        hb2->y_bound = (hb1->y + hb1->h) + hb2->h/2;
	}
    return true;
}


bool HitBox::AABB_vs_CBB(AABB* rect, CBB* circle){
    int distX = fabs(circle->x - rect->x-rect->w/2);
    int distY = fabs(circle->y - rect->y-rect->h/2);

    if (distX > (rect->w/2 + circle->r)){
        return false;
    }
    if (distY > (rect->h/2 + circle->r)){
        return false;
    }

    if (distX <= (rect->w/2)){
        return true;
    }
    if (distY <= (rect->h/2)){
        return true;
    }

    int dx = distX - rect->w/2;
    int dy = distY - rect->h/2;
    return( dx*dx + dy*dy <= (circle->r*circle->r) );
}


bool HitBox::CBB_CBB(CBB* hb1, CBB* hb2){
    float d = sqrt((hb1->x-hb2->x)*(hb1->x-hb2->x) + (hb1->y-hb2->y)*(hb1->y-hb2->y));      //le distance entre les rayons
    if (d > (hb1->r + hb2->r)){
        return false;
    }

    return true;
}




//----------------------------------------AABB----------------------------------------//
AABB::AABB(int x, int y, int w, int h): x(x), y(y), w(w), h(h){
}

void AABB::updateCara(double X, double Y, double angle, double scale){
    HitBox::updateCara(X, Y, 0, 1);
    this->x = round(X) - this->w/2;
    this->y = round(Y) - this->h/2;
}

bool AABB::overlap(HitBox* b){
    return b->overlap(this);
}
bool AABB::overlap(AABB* b){
    AABB_vs_AABB(this,b);
}
bool AABB::overlap(CBB* b){
    AABB_vs_CBB(this,b);
};

AABB* AABB::getACopy(){
    return new AABB(*this);
}





//----------------------------------------CBB----------------------------------------//
CBB::CBB(int x, int y, int r): x(x), y(y), r(r), base_r(r){
}

void CBB::updateCara(double X, double Y, double angle, double scale){
    HitBox::updateCara(X, Y, 0, 1);
    this->x = round(X);
    this->y = round(Y);
    this->r = this->base_r*scale;
}

bool CBB::overlap(HitBox* b){
    return b->overlap(this);
}
bool CBB::overlap(AABB* b){
    AABB_vs_CBB(b,this);
}
bool CBB::overlap(CBB* b){
    CBB_CBB(this,b);
}

bool CBB::overlap(OBB* b){
    CBB_vs_OBB(this,b);
}

CBB* CBB::getACopy(){
    CBB* h = new CBB(*this);
    //cout << h->r << endl;
    return h;
}




//----------------------------------------OBB----------------------------------------//
OBB::OBB(int x, int y, int w, int h): x(x), y(y), w(w), h(h){
    this->max_size = sqrt(w*w + h*h) + 1;
}

void OBB::updateCara(double X, double Y, double angle, double scale){
    HitBox::updateCara(X, Y, 0, 1);
    O = Point(X, Y);
    /*this->x = X;
    this->y = Y;*/
    this->angle = angle;
}

bool OBB::overlap(HitBox* b){
    return b->overlap(this);
}

bool OBB::overlap(CBB* b){
    CBB_vs_OBB(b, this);
}

bool OBB::overlap(AABB* b){
    cout << "Pas implémenté !" << endl;
    return false;
}

OBB* OBB::getACopy(){
    OBB* h = new OBB(*this);
    return h;
}





bool HitBox::CBB_vs_OBB(CBB* circle, OBB* obb){

    int X = obb->O.x; int Y = obb->O.y;

    //optimisation (cf. HitBox.h)
    AABB rect_test(X - obb->max_size/2, Y - obb->max_size/2, obb->max_size, obb->max_size);
    if(HitBox::AABB_vs_CBB(&rect_test, circle) == false){
        //cout << "true" << endl;
        return false;
    }
    //cout << "false" << endl;
    Point A(X - obb->w/2, Y - obb->h/2);
    Point B(X - obb->w/2, Y + obb->h/2);
    Point C(X + obb->w/2, Y + obb->h/2);
    Point D(X + obb->w/2, Y - obb->h/2);

    A = A.rotation(obb->angle, obb->O);
    B = B.rotation(obb->angle, obb->O);
    C = C.rotation(obb->angle, obb->O);
    D = D.rotation(obb->angle, obb->O);

    if( CollisionSegmentCircle(A, B, *circle) || CollisionSegmentCircle(B, C, *circle)
       || CollisionSegmentCircle(C, D, *circle) || CollisionSegmentCircle(D, A, *circle) ){
        return true;
    }
    return false;
}





