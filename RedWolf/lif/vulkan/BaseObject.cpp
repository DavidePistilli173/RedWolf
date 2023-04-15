#include "BaseObject.hpp"

#include "RedWolf/RedWolfManager.hpp"

using namespace rw::lif::vlk;

BaseObject::BaseObject(RedWolfManager& manager) :
   manager_{ manager }, logger_{ manager.logger() }, vulkanInstance_{ manager.vulkanInstance() }
{
}