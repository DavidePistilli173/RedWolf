#include "UdpSocket.hpp"

#include "RedWolf/evt/DataReadyEvent.hpp"

#include <vector>

using namespace rw::net;

UdpSocket::UdpSocket(rw::util::Logger& logger) : BaseSocket(logger), logger_{ logger } {}

UdpSocket::UdpSocket(rw::util::Logger& logger, std::string_view localAddress, std::string_view localPort, Family family) :
   BaseSocket(logger, localAddress, localPort, BaseSocket::Protocol::udp, family), logger_{ logger }
{
}

UdpSocket::~UdpSocket()
{
   workerRunning_ = false;
   if (workerThread_.joinable())
   {
      workerThread_.join();
   }
}

bool UdpSocket::close()
{
   workerRunning_ = false;
   return BaseSocket::close();
}

bool UdpSocket::open(std::string_view localAddress, std::string_view localPort, Family family)
{
   if (BaseSocket::open(localAddress, localPort, BaseSocket::Protocol::udp, family))
   {
#ifdef _WIN32
      // Set the socket as non-blocking.
      if (setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&read_timeout), sizeof(read_timeout)) != 0)
      {
         logger_.relErr(
            "Failed to set socket {}:{} as non-blocking, error code {}.", this->localAddress(), this->localPort(), WSAGetLastError());
      }
#endif
      startWorkerThread_();
      return true;
   }

   return false;
}

bool UdpSocket::send(std::string_view destinationAddress, std::string_view destinationPort, std::string_view string)
{
   return sendInternal_(destinationAddress, destinationPort, string.data(), static_cast<int>(string.length()));
}

void UdpSocket::userHandle_(const BaseObject* generator, const rw::evt::Event& evt) {}

bool UdpSocket::sendInternal_(std::string_view destinationAddress, std::string_view destinationPort, const char* buff, int buffLen)
{
#ifdef _WIN32
   sockaddr_storage destinationSocket{};
   sockaddr_in*     ipv4Data{ reinterpret_cast<sockaddr_in*>(&destinationSocket) };
   sockaddr_in6*    ipv6Data{ reinterpret_cast<sockaddr_in6*>(&destinationSocket) };
   socklen_t        addrLen{ 0U };

   switch (family())
   {
   case Family::ipv4:
      destinationSocket.ss_family = AF_INET;
      ipv4Data->sin_family = AF_INET;
      ipv4Data->sin_port = htons(static_cast<unsigned short>(atoi(destinationPort.data())));
      inet_pton(AF_INET, destinationAddress.data(), &ipv4Data->sin_addr);
      addrLen = sizeof(sockaddr_in);
      break;
   case Family::ipv6:
      destinationSocket.ss_family = AF_INET6;
      ipv6Data->sin6_family = AF_INET6;
      ipv6Data->sin6_port = htons(static_cast<unsigned short>(atoi(destinationPort.data())));
      inet_pton(AF_INET6, destinationAddress.data(), &ipv6Data->sin6_addr);
      addrLen = sizeof(sockaddr_in6);
      break;
   }

   int numBytes;

   {
      std::scoped_lock lck{ socketMutex_ };
      numBytes = sendto(socket_, buff, buffLen, 0, reinterpret_cast<sockaddr*>(&destinationSocket), addrLen);
   }

   if (numBytes < 0)
   {
      logger_.relErr(
         "Failed to send data from {}:{} to {}:{}, error code {}.",
         localAddress(),
         localPort(),
         destinationAddress,
         destinationPort,
         WSAGetLastError());
      return false;
   }
#endif

   return true;
}

void UdpSocket::startWorkerThread_()
{
   workerRunning_ = true;
   workerThread_ = std::thread(
      [this]()
      {
         std::vector<std::byte> readBuff;

         while (workerRunning_)
         {
            readBuff.clear();
            readBuff.resize(read_buff_size, std::byte(0));

            int numBytes;
#ifdef _WIN32
            sockaddr senderAddress;
            i32      senderAddressLen{ sizeof(sockaddr) };
            memset(&senderAddress, 0, sizeof(sockaddr));

            {
               std::scoped_lock lck{ socketMutex_ };
               numBytes = recvfrom(socket_, reinterpret_cast<char*>(readBuff.data()), read_buff_size, 0, &senderAddress, &senderAddressLen);
            }
#endif

#ifdef _WIN32
            if (i32 errorCode{ WSAGetLastError() }; numBytes < 0 && errorCode != WSAETIMEDOUT)
            {
               logger_.relErr("Read error on socket {}:{}, error code {}.", localAddress(), localPort(), errorCode);
            }
            else if (numBytes > 0)
            {
               rw::evt::DataReadyEvent evnt;

               switch (senderAddress.sa_family)
               {
               case AF_INET:
               {
                  auto* ipv4Data{ reinterpret_cast<sockaddr_in*>(&senderAddress) };
                  char  senderIp[INET_ADDRSTRLEN];
                  inet_ntop(AF_INET, &ipv4Data->sin_addr, senderIp, sizeof(senderIp));
                  uint16_t port{ ntohs(ipv4Data->sin_port) };
                  evnt.address = std::string(senderIp);
                  evnt.port = std::to_string(port);
               }
               break;
               case AF_INET6:
               {
                  auto* ipv6Data{ reinterpret_cast<sockaddr_in6*>(&senderAddress) };
                  char  senderIp[INET6_ADDRSTRLEN];
                  inet_ntop(AF_INET6, &ipv6Data->sin6_addr, senderIp, sizeof(senderIp));
                  uint16_t port{ ntohs(ipv6Data->sin6_port) };
                  evnt.address = std::string(senderIp);
                  evnt.port = std::to_string(port);
               }
               break;
               }

               evnt.data = readBuff;
               generateEvent(evnt);
            }
#endif
         }
      });
}