#include "FooA.h"
#include <iostream>

FooA::FooA()
    : m_calculator(nullptr)
{
}

FooA::~FooA()
{
}

void FooA::print()
{
    int result = m_B.add(5, 3);
    std::cout << "Result of add: " << result << std::endl;
}

int FooA::add(int a, int b)
{
    // テスト時は m_calculator (モック) を使用、通常は m_B を使用
    if (m_calculator) {
        return m_calculator->add(a, b);
    }
    return m_B.add(a, b);
}

int FooA::multiply(int a, int b)
{
    // テスト時は m_calculator (モック) を使用、通常は m_B を使用
    if (m_calculator) {
        return m_calculator->multiply(a, b);
    }
    return m_B.multiply(a, b);
}
