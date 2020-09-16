#ifndef TIMER_H
#define TIMER_H

class Timer
{
    private:
        int ticks;  //le temps depuis que l'on a lanc� le timer (en nombre de frame)
        int delay;  //on doit attendre se delai
        bool start; //vrai si le timer � commenc�
        bool finish; //vrai si le temps d'attente est �coul�

    public:
        Timer(float time);
        Timer();
        void launch();   //pour lancer le timer
        bool isLaunched();  //renvoie vrai si l'on a lanc� le compte � rebours
        void pass();    //pour faire passer le temps
        float getTime();    //renvoie le temps qui s'est �coul� jusque l� (en seconde)
        float getRemainingTime();    //renvoie le temps qu'il reste avant la fin du timer (en seconde)
        bool isTicksMultipleBy(int multiple);   //vrai le les ticks (voir plus haut) sont divisibles par l'entier "multiple"
        bool isTimeUnder(float time);   //vrai le le temps en cours est plus petit que la valeur de "time"
        bool isFinished();  //renvoie vrai si le temps d'attente est �coul�
        virtual ~Timer();
};

#endif // TIMER_H
