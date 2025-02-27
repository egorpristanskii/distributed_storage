#pragma once

#include <string>

namespace storage {
class Storage {
   public:
    virtual bool put(const std::string& key, const std::string& value) = 0;
    virtual bool remove(const std::string& key) = 0;
    virtual std::string get(const std::string& key) = 0;

    virtual ~Storage() = default;
};
}  // namespace storage
