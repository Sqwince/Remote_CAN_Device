// File: example.h
#ifndef EXAMPLE_H
#define EXAMPLE_H

class Example {
public:
    void sayHello();
};

#endif // EXAMPLE_H

/*################################################*/

// File: example.cpp
#include <iostream>
#include "example.h"

void Example::sayHello() {
    std::cout << "Hello, World!" << std::endl;
}