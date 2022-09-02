#include "UdpSocket.hpp"

#include "RedWolf/events/DataReadyEvent.hpp"

#include <vector>

namespace rw::net
{
   UdpSocket::UdpSocket(BaseObject* parent) : BaseSocket(parent), logger_{ rw::utils::Logger::instance() } {}

   UdpSocket::UdpSocket(std::string_view localAddress, std::string_view localPort, Family family, BaseObject* parent) :
      BaseSocket(localAddress, localPort, BaseSocket::Protocol::udp, family, parent), logger_{ rw::utils::Logger::instance() }
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
            logger_->relErr(
               "Failed to set socket {}:{} as non-blocking, error code {}.", this->localAddress(), this->localPort(), WSAGetLastError());
         }
#endif
         startWorkerThread_();
         return true;
      }

      return false;
   }

   void UdpSocket::userHandle_(const rw::events::BaseEvent&, const rw::core::BaseObject*) {}

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
               int      senderAddressLen{ sizeof(sockaddr) };
               memset(&senderAddress, 0, sizeof(sockaddr));

               {
                  std::scoped_lock lck{ socketMutex_ };
                  numBytes =
                     recvfrom(socket_, reinterpret_cast<char*>(readBuff.data()), read_buff_size, 0, &senderAddress, &senderAddressLen);
               }
#endif

#ifdef _WIN32
               if (int errorCode{ WSAGetLastError() }; numBytes < 0 && errorCode != WSAETIMEDOUT)
               {
                  logger_->relErr("Read error on socket {}:{}, error code {}.", localAddress(), localPort(), errorCode);
               }
               else if (numBytes > 0)
               {
                  generateEvent(rw::events::DataReadyEvent(readBuff, localAddress(), localPort()));
               }
#endif
            }
         });
   }
} // namespace rw::net