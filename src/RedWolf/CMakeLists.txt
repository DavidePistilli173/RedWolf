add_library(RedWolf STATIC)

target_sources(RedWolf PRIVATE
        engine/app.cpp
        engine/entry_point.cpp
        thread/thread_pool.cpp
        time/date_time.cpp
        util/log_msg.cpp
        util/logger.cpp
)

target_include_directories(RedWolf PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(RedWolf PUBLIC)