#include "BaseObject.hpp"

#include "RedWolf/RedWolfManager.hpp"

using namespace rw::lif::vlk;

BaseObject::BaseObject(RedWolfManager& manager) :
   manager_{ manager }, logger_{ manager.logger() }, vulkanInterface_{ manager.vulkanInterface() }, vulkanInstance_{
      manager.vulkanInstance()
   }
{
}