#ifndef RWSANDBOX_MAINAPPLICATION_HPP
#define RWSANDBOX_MAINAPPLICATION_HPP

#include "MainWindow.hpp"

#include <RedWolf/RedWolfManager.hpp>
#include <RedWolf/core/BaseGUIApplication.hpp>
#include <RedWolf/net/UdpSocket.hpp>
#include <RedWolf/time/Timer.hpp>
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
   virtual void userHandle_(const rw::events::BaseEvent& evnt, const BaseObject* sender) override;

   /**
    * @brief User initialisation function.
    */
   virtual void userInit_() override;

   /**
    * @brief User run function for custom operations that will be executed at each cycle by the main thread.
    * @details To be implemented in derived classes.
    */
   virtual void userRun_() override;

private:
   rw::RedWolfManager& rw_;     /**< RedWolf library manager. */
   rw::utils::Logger&  logger_; /**< Application logger. */

   size_t mainLoopIteration_{ 0U }; /**< Number of iterations of the main loop. */
   size_t totalIterations_{ 0U };   /**< Total number of iterations of the main loop. */

   std::unique_ptr<rw::time::Timer> timer_;            /**< Timer. */
   std::atomic<size_t>              timerCount_{ 0U }; /**< Number of times the timer was called since the last main loop iteration. */

   std::unique_ptr<rw::net::UdpSocket> socket_;            /**< UDP socket. */
   std::atomic<size_t>                 packetCount_{ 0U }; /**< Number of received packets. */
   std::mutex                          packetSenderMutex_; /**< Mutex for protecting the packet sender information. */
   std::string                         senderAddress_;     /**< IP address of the packet sender. */
   std::string                         senderPort_;        /**< Port of the packet sender. */

   MainWindow mainWindow_; /**< Main window of the application. */
};

#endif