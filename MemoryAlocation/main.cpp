#include <iostream>
#include <math.h>

int main ()
{
    using namespace std;

    int * p_number1{new int};// Dynamically allocating memory for an integer

    cout << "Address of p_number1: " << p_number1 << endl;

    delete p_number1; // Deleting uninitialized pointer (undefined behavior)
    
    p_number1 = nullptr; // Setting pointer to nullptr after deletion
    
    return 0;
}


