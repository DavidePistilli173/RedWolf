#include "BaseSocket.hpp"

namespace rw::net
{
#ifdef WIN32
   bool       BaseSocket::winsockInitialised_{ false };
   size_t     BaseSocket::winsockActiveSockets_{ 0U };
   std::mutex BaseSocket::winsockMutex_;
#endif

   BaseSocket::BaseSocket(RedWolfManager& manager, BaseObject* parent) : BaseObject(manager, parent), logger_{ manager.logger() }
   {
#ifdef _WIN32
      initWinsock_();
#endif
   }

   BaseSocket::BaseSocket(
      RedWolfManager&  manager,
      std::string_view localAddress,
      std::string_view localPort,
      Protocol         protocol,
      Family           family,
      BaseObject*      parent) :
      BaseObject(manager, parent),
      logger_{ manager.logger() }
   {
#ifdef _WIN32
      initWinsock_();
#endif

      open(localAddress, localPort, protocol, family);
   }

   BaseSocket::~BaseSocket()
   {
      close();

#ifdef WIN32
      std::scoped_lock lck{ winsockMutex_ };
      --winsockActiveSockets_;
      if (winsockActiveSockets_ == 0U)
      {
         if (WSACleanup() != 0)
         {
            logger_.relErr("Failed to clean-up Winsock.");
         }
         else
         {
            logger_.relInfo("Winsock library cleaned-up.");
            winsockInitialised_ = false;
         }
      }
#endif
   }

   bool BaseSocket::close()
   {
      std::scoped_lock lck{ socketMutex_ };
      if (isOpen())
      {
#ifdef WIN32
         int result{ closesocket(socket_) };
         if (result == SOCKET_ERROR)
         {
            logger_.relErr("Failed to close socket {}:{}, error code {}.", localAddress_, localPort_, WSAGetLastError());
            return false;
         }
#endif

         socketOpen_ = false;
      }

      return true;
   }

   BaseSocket::Family BaseSocket::family() const
   {
      return family_;
   }

   bool BaseSocket::isOpen() const
   {
      return socketOpen_;
   }

   std::string BaseSocket::localAddress() const
   {
      std::shared_lock lck{ socketMutex_ };
      return localAddress_;
   }

   std::string BaseSocket::localPort() const
   {
      std::shared_lock lck{ socketMutex_ };
      return localPort_;
   }

   bool BaseSocket::open(std::string_view localAddress, std::string_view localPort, Protocol protocol, Family family)
   {
      std::scoped_lock lck{ socketMutex_ };
      if (isOpen())
      {
         logger_.relWarn("Socket already open on {}:{}, cannot open again on {}:{}.", localAddress_, localPort_, localAddress, localPort);
         return false;
      }

      localAddress_ = localAddress;
      localPort_ = localPort;
      protocol_ = protocol;
      family_ = family;

      // Winsock socket open.
#ifdef _WIN32
      addrinfo* address{ nullptr };

      // Prepare address info.
      addrinfo hints;
      memset(&hints, 0, sizeof(addrinfo));
      hints.ai_flags = AI_PASSIVE;

      switch (family_)
      {
      case Family::ipv4:
         hints.ai_family = AF_INET;
         break;
      case Family::ipv6:
         hints.ai_family = AF_INET6;
         break;
      }

      switch (protocol_)
      {
      case Protocol::tcp:
         hints.ai_socktype = SOCK_STREAM;
         hints.ai_protocol = IPPROTO_TCP;
         break;
      case Protocol::udp:
         hints.ai_socktype = SOCK_DGRAM;
         hints.ai_protocol = IPPROTO_UDP;
         break;
      }

      // Resolve the address.
      int result{ getaddrinfo(localAddress_.c_str(), localPort_.c_str(), &hints, &address) };
      if (result != 0)
      {
         logger_.relErr("Failed to get socket information for {}:{}, error code {}.", localAddress_, localPort_, result);
         return false;
      }

      // Create the socket.
      socket_ = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
      if (socket_ == INVALID_SOCKET)
      {
         logger_.relErr("Failed to create socket {}:{}, error code {}.", localAddress_, localPort_, WSAGetLastError());
         freeaddrinfo(address);
         return false;
      }

      // Bind the socket.
      result = bind(socket_, address->ai_addr, static_cast<int>(address->ai_addrlen));
      if (result == SOCKET_ERROR)
      {
         logger_.relErr("Failed to bind socket {}:{}, error code {}.", localAddress_, localPort_, WSAGetLastError());
         freeaddrinfo(address);
         closesocket(socket_);
         return false;
      }

      freeaddrinfo(address);
#endif

      socketOpen_ = true;
      return true;
   }

   void BaseSocket::userHandle_(const rw::events::BaseEvent&, const rw::core::BaseObject*) {}

#ifdef _WIN32
   void BaseSocket::initWinsock_()
   {
      std::scoped_lock lck{ winsockMutex_ };

      if (!winsockInitialised_)
      {
         WSADATA wsaData;
         int     initResult{ WSAStartup(MAKEWORD(2, 2), &wsaData) };
         if (initResult != 0)
         {
            logger_.relFatal("Failed to initialise Winsock library.");
         }

         winsockInitialised_ = true;
         logger_.relInfo("Winsock library initialised.");
      }
      ++winsockActiveSockets_;
   }
#endif
} // namespace rw::net