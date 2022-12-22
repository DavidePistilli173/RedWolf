#include "BaseObject.hpp"

#include "RedWolf/RedWolfManager.hpp"

using namespace rw::core;

BaseObject::BaseObject(RedWolfManager& manager) :
   manager_{ manager }, logger_{ manager.logger() }, threadPool_{ manager.threadPool() }, eventHandler_{ manager.eventHandler() }
{
   eventHandler_.addObject(this);
}

BaseObject::~BaseObject()
{
   eventHandler_.removeObject(this);
}

void BaseObject::handle(const rw::events::BaseEvent& evnt, const BaseObject* sender)
{
   userHandle_(evnt, sender);
}