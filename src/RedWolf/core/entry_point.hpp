//
// Created by cjm036653 on 10/08/25.
//

#ifndef ENTRY_POINT_HPP
#define ENTRY_POINT_HPP

#include "app.hpp"

#include <memory>

extern std::unique_ptr<rw::core::App> rw::core::create_app();

/**
 * @brief Entry point for any application using the RedWolf engine.
 */
int main();

#endif // ENTRY_POINT_HPP
