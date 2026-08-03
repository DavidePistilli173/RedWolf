#include <redwolf/boot/entry_point.hpp>

#include <iostream>

void rw_user::init_modules()
{
    std::cout << "Hello from RedWolf!" << std::endl;
} 