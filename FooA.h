#ifndef FOOA_H
#define FOOA_H

#include "FooB.h"

// ========================================
// ICalculator インターフェース
// ========================================
class ICalculator {
public:
    virtual ~ICalculator() = default;
    virtual int add(int a, int b) = 0;
    virtual int multiply(int a, int b) = 0;
};

class FooA
{
friend class FooA_Test; // テスト用フレンドクラス

public:
    FooA();
    ~FooA();
    void print();
    int add(int a, int b);
    int multiply(int a, int b);

private:
    FooB m_B;
    ICalculator* m_calculator;

};

#endif
