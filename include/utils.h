#pragma once

#include <chrono>
#include <cstdint>

namespace storage {
inline std::int64_t getUTCTimeStamp() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
}
}  // namespace storage
