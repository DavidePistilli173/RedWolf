#ifndef RW_NET_BASESOCKET_HPP
#define RW_NET_BASESOCKET_HPP

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/common.hpp"
#include "RedWolf/core/BaseObject.hpp"
#include "RedWolf/utils/Logger.hpp"

#include <mutex>
#include <optional>
#include <shared_mutex>
#include <string>
#include <string_view>

#ifdef _WIN32
   #include <WS2tcpip.h>
   #include <WinSock2.h>
   #undef min // Incompatible with the usage of std::min
#endif

namespace rw::net
{
   /**
    * @brief Base class for network sockets.
    */
   class RW_API BaseSocket : public rw::core::BaseObject
   {
   public:
      /**
       * @brief Possible address families.
       */
      enum class Family
      {
         ipv4,
         ipv6
      };

      /**
       * @brief Supported protocols.
       */
      enum class Protocol
      {
         tcp,
         udp
      };

      /**
       * @brief Construct an empty socket.
       * @param manager RedWolf library manager.
       * @param parent Parent of the object.
       */
      BaseSocket(RedWolfManager& manager, BaseObject* parent = nullptr);

      /**
       * @brief Construct a socket and also open it, if possible.
       * @param manager RedWolf library manager.
       * @param localAddress Local IP address to bind the socket to.
       * @param localPort Local port to bind the socket to.
       * @param protocol Protocol to use with this socket.
       * @param family Address family for the socket's address.
       * @param parent Parent of the socket.
       */
      BaseSocket(
         RedWolfManager&  manager,
         std::string_view localAddress,
         std::string_view localPort,
         Protocol         protocol,
         Family           family,
         BaseObject*      parent = nullptr);

      /**
       * @brief Destructor
       */
      virtual ~BaseSocket();

      /**
       * @brief Sockets cannot be copied.
       */
      BaseSocket(const BaseSocket&) = delete;

      /**
       * @brief Move constructor.
       */
      BaseSocket(BaseSocket&&) = default;

      /**
       * @brief Sockets cannot be copied.
       */
      BaseSocket& operator=(const BaseSocket&) = delete;

      /**
       * @brief Move assignment operator.
       */
      BaseSocket& operator=(BaseSocket&&) = default;

      /**
       * @brief Close any currently open socket.
       * @return true on success, false otherwise.
       */
      bool close();

      /**
       * @brief Get the address family of the socket.
       * @return Address family of the socket.
       */
      Family family() const;

      /**
       * @brief Check whether the socket is open or not.
       * @return true if the socket is open, false otherwise.
       */
      bool isOpen() const;

      /**
       * @brief Get the currently set local address.
       * @return Current local address.
       */
      std::string localAddress() const;

      /**
       * @brief Get the currently set local port.
       * @return Current local port.
       */
      std::string localPort() const;

      /**
       * @brief Open a socket.
       * @param localAddress Local IP address to bind the socket to.
       * @param localPort Local port to bind the socket to.
       * @param protocol Protocol to use with this socket.
       * @param family Address family for the socket's address.
       * @return true on success, false otherwise.
       */
      bool open(std::string_view localAddress, std::string_view localPort, Protocol protocol = Protocol::udp, Family family = Family::ipv4);

   protected:
      /**
       * @brief Implementation of the userHandle_ function from BaseObject.
       * @param evnt Event.
       * @param sender Sender of the event.
       */
      virtual void userHandle_(const rw::events::BaseEvent& evnt, const rw::core::BaseObject* sender) override;

      mutable std::shared_mutex socketMutex_; /**< Mutex for protecting socket operations. */

#ifdef _WIN32
      SOCKET socket_{ INVALID_SOCKET }; /**< Socket handle. */
#endif

   private:
      // Windows specific member functions
#ifdef _WIN32
      /**
       * @brief Initialise the Winsock library.
       */
      void initWinsock_();
#endif

      rw::utils::Logger& logger_; /**< Logger instance. */

      bool        socketOpen_{ false };       /**< True if the socket is currently open. */
      std::string localAddress_;              /**< Socket local address. */
      std::string localPort_;                 /**< Socket local port. */
      Protocol    protocol_{ Protocol::udp }; /**< Socket protocol. */
      Family      family_{ Family::ipv4 };    /**< Socket address family. */

      // Windows specific variables.
#ifdef _WIN32
      static bool       winsockInitialised_;   /**< True only if the Winsock library is initialised. */
      static size_t     winsockActiveSockets_; /**< Number of currently active sockets. */
      static std::mutex winsockMutex_;         /**< Mutex for protecting winsock variables. */
#endif
   };
} // namespace rw::net

#endif
