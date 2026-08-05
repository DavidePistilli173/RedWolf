#include "redwolf/engine.hpp"

#include "redwolf/logger.hpp"

rw::Engine::Engine() {
    Logger::init();
    info("Logger initialised.");
}