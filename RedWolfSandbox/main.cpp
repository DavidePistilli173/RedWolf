#include "MainApplication.hpp"

#include <RedWolf/core/BaseObject.hpp>
#include <RedWolf/net/BaseSocket.hpp>
#include <RedWolf/utils/Logger.hpp>
#include <iostream>

using namespace rw::utils;
using LogLevel = Logger::Level;

int main(int argc, char** argv)
{
   try
   {
      Logger* logger{ Logger::instance() };
      logger->setDebugLevel(LogLevel::trace);
      logger->setReleaseLevel(LogLevel::info);

      if (argc > 0)
      {
         logger->trace("Welcome to: {}", argv[0]);
      }
      else
      {
         logger->trace("Welcome to: RedWolfSandbox!");
      }

      rw::net::BaseSocket socket{ "10.0.0.1", "3545" };

      MainApplication app(argc, argv);
      app.init();

      app.run();
   }
   catch (const std::exception& e)
   {
      std::cout << "Exception: " << e.what() << '\n';
      return 1;
   }
   catch (...)
   {
      std::cout << "Unknown exception\n";
      return 1;
   }

   return 0;
}