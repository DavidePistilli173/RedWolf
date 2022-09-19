#include "MainApplication.hpp"

#include <RedWolf/RedWolfManager.hpp>
#include <RedWolf/core/BaseObject.hpp>
#include <RedWolf/utils/Logger.hpp>
#include <iostream>

using namespace rw::utils;
using LogLevel = Logger::Level;

int main(int argc, char** argv)
{
   try
   {
      rw::RedWolfManager::Options rwOptions;
      rwOptions.appName = "RedWolf Sandbox";
      rwOptions.appVersion.major = 0;
      rwOptions.appVersion.minor = 5;
      rwOptions.appVersion.patch = 0;

      rw::RedWolfManager rw{ rwOptions };

      Logger& logger{ rw.logger() };
      logger.setDebugLevel(LogLevel::trace);
      logger.setReleaseLevel(LogLevel::info);

      logger.trace("Welcome to: {}!", rwOptions.appName);

      MainApplication app(rw, argc, argv);
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