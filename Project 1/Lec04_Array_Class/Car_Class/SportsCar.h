#pragma once
#include "Car.h" 

class SportsCar : public Car
{
public:
    bool bTurbo;

    SportsCar() { bTurbo = false; }

    SportsCar(int s, const char* n, int g, bool bTur) : Car(s, n, g) {
        bTurbo = bTur;
    }

    void setTurbo(bool bTur) {
        bTurbo = bTur;
    }

    void speedUp() {
        if (bTurbo)
            speed += 20;
        else
            Car::speedUp();
    }
    void display() {

        Car::display();

        if (bTurbo)
            printf("Turbo : ON\n");
        else
            printf("Turbo : OFF\n");
    }
};