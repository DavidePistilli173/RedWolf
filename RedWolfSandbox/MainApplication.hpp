#ifndef RWSANDBOX_MAINAPPLICATION_HPP
#define RWSANDBOX_MAINAPPLICATION_HPP

#include "MainWindow.hpp"

#include <RedWolf/RedWolfManager.hpp>
#include <RedWolf/core/BaseGUIApplication.hpp>
#include <RedWolf/utils/Logger.hpp>
#include <memory>

/**
 * @brief Main application of the RedWolfSandbox program.
 */
class MainApplication : public rw::core::BaseGUIApplication
{
public:
   /**
    * @brief Constructor.
    * @param rw Manager for the RedWolf library.
    * @param argc Number of input arguments.
    * @param argv Input arguments.
    */
   explicit MainApplication(rw::RedWolfManager& rw, int argc, char** argv);

protected:
   /**
    * @brief User handle function for incoming events.
    * @param evnt Event data.
    * @param sender Sender of the event.
    */
   void userHandle_(const rw::events::BaseEvent& evnt, const BaseObject* sender) override;

   /**
    * @brief User initialisation function.
    */
   void userInit_() override;

   /**
    * @brief User run function for custom operations that will be executed at each cycle by the main thread.
    * @details To be implemented in derived classes.
    */
   void userRun_() override;

private:
   rw::RedWolfManager& rw_;     /**< RedWolf library manager. */
   rw::utils::Logger&  logger_; /**< Application logger. */

   size_t mainLoopIteration_{ 0U }; /**< Number of iterations of the main loop. */
   size_t totalIterations_{ 0U };   /**< Total number of iterations of the main loop. */

   MainWindow* mainWindow_; /**< Main window of the application. */
};

#endif