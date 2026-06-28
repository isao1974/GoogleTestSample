#include <iostream>
#include "FooA.h"

int main() {
    FooA fooA;
    fooA.print(); // Call the print function of FooA, which in turn calls the print function of FooB

    return 0;
}