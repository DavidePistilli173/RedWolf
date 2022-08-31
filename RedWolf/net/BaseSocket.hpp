#ifndef RW_NET_BASESOCKET_HPP
#define RW_NET_BASESOCKET_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/utils/Logger.hpp"

#include <string>
#include <string_view>

#ifdef _WIN32
   #include <WS2tcpip.h>
   #include <WinSock2.h>
#endif

namespace rw::net
{
   /**
    * @brief Base class for network sockets.
    */
   class RW_API BaseSocket
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
       * @brief Default constructor.
       */
      BaseSocket();

      /**
       * @brief Constructor that also opens the socket, if it is able to.
       * @param localAddress Local IP address to bind the socket to.
       * @param localPort Local port to bind the socket to.
       * @param protocol Protocol to use with this socket.
       * @param family Address family for the socket's address.
       */
      BaseSocket(
         std::string_view localAddress, std::string_view localPort, Protocol protocol = Protocol::udp, Family family = Family::ipv4);

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
       * @brief Check whether the socket is open or not.
       * @return true if the socket is open, false otherwise.
       */
      bool isOpen() const;

      /**
       * @brief Open a socket.
       * @param localAddress Local IP address to bind the socket to.
       * @param localPort Local port to bind the socket to.
       * @param protocol Protocol to use with this socket.
       * @param family Address family for the socket's address.
       * @return true on success, false otherwise.
       */
      bool open(std::string_view localAddress, std::string_view localPort, Protocol protocol = Protocol::udp, Family family = Family::ipv4);

   private:
      // Windows specific member functions
#ifdef _WIN32
      /**
       * @brief Initialise the Winsock library.
       */
      void initWinsock_();
#endif

      rw::utils::Logger* logger_{ nullptr }; /**< Logger instance. */

      bool        socketOpen_{ false };       /**< True if the socket is currently open. */
      std::string localAddress_;              /**< Socket local address. */
      std::string localPort_;                 /**< Socket local port. */
      Protocol    protocol_{ Protocol::udp }; /**< Socket protocol. */
      Family      family_{ Family::ipv4 };    /**< Socket address family. */

      // Windows specific variables.
#ifdef _WIN32
      static bool   winsockInitialised_;   /**< True only if the Winsock library is initialised. */
      static size_t winsockActiveSockets_; /**< Number of currently active sockets. */

      SOCKET socket_{ INVALID_SOCKET }; /**< Socket handle. */
#endif
   };
} // namespace rw::net

#endif