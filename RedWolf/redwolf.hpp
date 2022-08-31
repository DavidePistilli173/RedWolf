/** @file redwolf.hpp File that includes all library headers. Use this only when you are REALLY lazy. */

#ifndef RW_REDWOLF_HPP
#define RW_REDWOLF_HPP

#include "common.hpp"

// alg
#include "RedWolf/alg/Sorting.hpp"

// cont
#include "RedWolf/cont/ConstexprMap.hpp"

// core
#include "RedWolf/core/BaseApplication.hpp"
#include "RedWolf/core/BaseObject.hpp"

// dat
#include "RedWolf/dat/KVPair.hpp"
#include "RedWolf/dat/LogMsg.hpp"
#include "RedWolf/dat/MVPair.hpp"
#include "RedWolf/dat/SettingsNode.hpp"

// events
#include "RedWolf/events/BaseEvent.hpp"
#include "RedWolf/events/ButtonPressEvent.hpp"
#include "RedWolf/events/TimeoutEvent.hpp"
#include "RedWolf/events/aliases.hpp"
#include "RedWolf/events/concepts.hpp"

// io
#include "RedWolf/io/File.hpp"
#include "RedWolf/io/concepts.hpp"

// net
#include "RedWolf/net/BaseSocket.hpp"

// text
#include "RedWolf/text/string_utils.hpp"

// thread
#include "RedWolf/thread/ThreadPool.hpp"

// time
#include "RedWolf/time/DateTime.hpp"
#include "RedWolf/time/Timer.hpp"
#include "RedWolf/time/concepts.hpp"
#include "RedWolf/time/constants.hpp"

// utils
#include "RedWolf/utils/Logger.hpp"
#include "RedWolf/utils/Settings.hpp"

#endif