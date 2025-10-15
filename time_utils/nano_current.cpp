#include "nano_current.hpp"
#include <chrono>

using namespace timeutils;

long long timeutils::get_current_nano_time(){
    auto now = std::chrono::system_clock::now();
    auto nanoseconds_since_epoch = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
    return nanoseconds_since_epoch.count();
}