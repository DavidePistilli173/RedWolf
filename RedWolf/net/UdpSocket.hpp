#ifndef RW_NET_UDPSOCKET_HPP
#define RW_NET_UDPSOCKET_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/net/BaseSocket.hpp"
#include "RedWolf/utils/Logger.hpp"

#include <string_view>
#include <thread>

namespace rw::net
{
   class RW_API UdpSocket : public BaseSocket
   {
   public:
      using Family = BaseSocket::Family;

#ifdef _WIN32
      static constexpr int read_timeout{ 1000U }; /**< Timeout on each read call [ms]. */
#else
      static constexpr timeval read_timeout{ 0U, 1000U }; /**< Timeout on each read call. */
#endif
      static constexpr size_t read_buff_size{ 65535U }; /**< Size of the read buffer in bytes. */

      /**
       * @brief Construct an empty UDP socket.
       */
      UdpSocket(BaseObject* parent = nullptr);

      /**
       * @brief Constructor that also opens the socket, if it is able to.
       * @param localAddress Local IP address to bind the socket to.
       * @param localPort Local port to bind the socket to.
       * @param family Address family for the socket's address.
       * @param parent Parent of the socket.
       */
      UdpSocket(std::string_view localAddress, std::string_view localPort, Family family = Family::ipv4, BaseObject* parent = nullptr);

      /**
       * @brief Destructor.
       */
      virtual ~UdpSocket();

      /**
       * @brief Close any currently open socket.
       * @return true on success, false otherwise.
       */
      bool close();

      /**
       * @brief Open a socket.
       * @param localAddress Local IP address to bind the socket to.
       * @param localPort Local port to bind the socket to.
       * @param family Address family for the socket's address.
       * @return true on success, false otherwise.
       */
      bool open(std::string_view localAddress, std::string_view localPort, Family family = Family::ipv4);

   protected:
      /**
       * @brief Implementation of the userHandle_ function from BaseObject.
       * @param evnt Event.
       * @param sender Sender of the event.
       */
      virtual void userHandle_(const rw::events::BaseEvent& evnt, const rw::core::BaseObject* sender) override;

   private:
      /**
       * @brief Utility function to start the worker thread of the socket.
       */
      void startWorkerThread_();

      rw::utils::Logger* logger_{ nullptr }; /**< Logger instance. */

      std::thread workerThread_;           /**< Worker thread for receiving messages. */
      bool        workerRunning_{ false }; /**< True if the worker thread is running. */
   };
} // namespace rw::net

#endif