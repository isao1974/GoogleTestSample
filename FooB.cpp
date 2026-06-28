#include "FooB.h"
#include <iostream>

FooB::FooB()
{

}

FooB::~FooB()
{

}

void FooB::print()
{
    std::cout << "Hello from FooB!" << std::endl;
}

int FooB::add(int a, int b)
{
    print();
    return a + b;
}

int FooB::multiply(int a, int b)
{
    std::cout << "Hello from FooB!" << std::endl;
    return a * b;
}
