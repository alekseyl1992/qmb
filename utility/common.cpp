#include "common.h"

ull_t get_now_time()
{
    auto now = std::chrono::system_clock::now();
    auto duration  = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}
