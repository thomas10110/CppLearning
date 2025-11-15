#include <iostream>

int main ()
{
    using namespace std;

int *p_number {new int {67}};

int number {55};

p_number = &number; // memory leak occurs here
cout << "Value of p_number: " << *p_number << endl;

    return 0;
}