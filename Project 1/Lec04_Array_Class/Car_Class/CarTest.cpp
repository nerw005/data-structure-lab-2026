#include <iostream>
#include "SportsCar.h"

using namespace std;

int main() {
    Car A(120, "Avante", 5);
    SportsCar B(200, "Porsche", 6, true);

    cout << "[ Initial State ]" << endl;
    A.display();
    cout << "-----------------------" << endl;
    B.display();
    cout << endl;

	A.speedUp();
	B.speedUp();

	cout << " [ After Acceleration ]" << endl;
    A.display();
    cout << "-----------------------" << endl;
    B.display();
    cout << endl;

    return 0;
}