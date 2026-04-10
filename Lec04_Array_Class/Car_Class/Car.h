#pragma once
#include <cstdio>
#include <cstring>

class Car {
protected:
    int speed;
    char name[40];
public:
    int gear;

    Car() {
        speed = 0;
        gear = 0;
        name[0] = '\0';
    }
    ~Car() {}

    Car(int s, const char* n, int g) : speed(s), gear(g) {
        strcpy_s(name, sizeof(name), n);
    }

    void changeGear(int g = 4) {
        gear = g;
    }

    void speedUp() {
        speed += 5;
    }

    void display() {
        printf("Brand : %s\nGear : %d\nSpeed : %dkmph\n", name, gear, speed);
    }

    void whereAmI() {
        printf("Object Address = %p\n", this);
    }
};