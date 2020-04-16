#include "RedWolfEditor/editor.hpp"

std::unique_ptr<rw::App> rw::createApp()
{
    return std::make_unique<rw::App>(Editor{});
}