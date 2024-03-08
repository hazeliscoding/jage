#include <iostream>

#include "jage/engine.h"

int main()
{
    std::cout << "Hello, World!" << std::endl;

    jage::GetInfo();

    int a = jage::Add(5, 3);
    int b = jage::Sub(5, 3);

    std::cout << "5 + 3 = " << a << std::endl;
    std::cout << "5 - 3 = " << b << std::endl;

    return 0;
}