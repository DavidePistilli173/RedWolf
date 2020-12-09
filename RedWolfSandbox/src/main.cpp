#include "RedWolfSandbox/MainWindow.hpp"

#include <RedWolf/RedWolf.hpp>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
   rw::SubsystemsHandler initialiser{};
   rwsandbox::MainWindow win{};
   win.run();
   return 0;
}