#pragma once

#include <string>
#include <unordered_map>

namespace storage {
template <typename T>
class Storage {
   public:
    virtual bool put(const std::string& key, const T& value) = 0;
    virtual bool remove(const std::string& key) = 0;
    virtual T get(const std::string& key) = 0;

    virtual ~Storage() = default;

   protected:
    std::unordered_map<std::string, T> storage_;
};
}  // namespace storage
