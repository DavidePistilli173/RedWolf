#ifndef RW_NET_UDPSOCKET_HPP
#define RW_NET_UDPSOCKET_HPP

#include "RedWolf/RedWolfManager.hpp"
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
       * @param manager RedWolf library manager.
       * @param parent Parent of the current object.
       */
      UdpSocket(RedWolfManager& manager, BaseObject* parent = nullptr);

      /**
       * @brief Constructor that also opens the socket, if it is able to.
       * @param manager RedWolf library manager.
       * @param localAddress Local IP address to bind the socket to.
       * @param localPort Local port to bind the socket to.
       * @param family Address family for the socket's address.
       * @param parent Parent of the socket.
       */
      UdpSocket(
         RedWolfManager&  manager,
         std::string_view localAddress,
         std::string_view localPort,
         Family           family = Family::ipv4,
         BaseObject*      parent = nullptr);

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

      /**
       * @brief Send a string to a remote IP address.
       * @param destinationAddress IP address to send the data to.
       * @param destinationPort IP port to send the data to.
       * @param string String to send.
       * @return true on success, false otherwise.
       */
      bool send(std::string_view destinationAddress, std::string_view destinationPort, std::string_view string);

      /**
       * @brief Send an object to a remote IP address.
       * @tparam T Type of object to send.
       * @param destinationAddress IP address to send the data to.
       * @param destinationPort IP port to send the data to.
       * @param object Object to send.
       * @return true on success, false otherwise.
       */
      template<typename T>
      bool send(std::string_view destinationAddress, std::string_view destinationPort, const T& object)
      {
         return sendInternal_(destinationAddress, destinationPort, reinterpret_cast<const char*>(&object), sizeof(T));
      }

      /**
       * @brief Send a vector of objects to a remote IP address.
       * @tparam T Type of objects to send.
       * @param destinationAddress IP address to send the data to.
       * @param destinationPort IP port to send the data to.
       * @param data Raw data to send.
       * @return true on success, false otherwise.
       */
      template<typename T>
      bool send(std::string_view destinationAddress, std::string_view destinationPort, const std::vector<T>& data)
      {
         return sendInternal_(destinationAddress, destinationPort, reinterpret_cast<const char*>(data.data()), data.size() * sizeof(T));
      }

   protected:
      /**
       * @brief Implementation of the userHandle_ function from BaseObject.
       * @param evnt Event.
       * @param sender Sender of the event.
       */
      virtual void userHandle_(const rw::events::BaseEvent& evnt, const rw::core::BaseObject* sender) override;

   private:
      /**
       * @brief Internal function for sending data to a remote IP address.
       * @param destinationAddress IP address to send the data to.
       * @param destinationPort IP port to send the data to.
       * @param buff Pointer to the data buffer.
       * @param buffLen Length in bytes of the buffer.
       * @return true on success, false otherwise.
       */
      bool sendInternal_(std::string_view destinationAddress, std::string_view destinationPort, const char* buff, int buffLen);

      /**
       * @brief Utility function to start the worker thread of the socket.
       */
      void startWorkerThread_();

      rw::utils::Logger& logger_; /**< Logger instance. */

      std::thread workerThread_;           /**< Worker thread for receiving messages. */
      bool        workerRunning_{ false }; /**< True if the worker thread is running. */
   };
} // namespace rw::net

#endif