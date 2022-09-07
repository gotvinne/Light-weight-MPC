// Copyright (C) Geir Ola Tvinnereim 2022

#include <iostream>

#include "../include/formatting.h"

int main() {

    json data = parse("../scenarios/test.json");
    std::cout << data.dump() << std::endl;
    return 0;
}