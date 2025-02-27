#pragma once

#include "storage.h"

#include <shared_mutex>

namespace storage {
class StringStorage : public Storage<std::string> {
   public:
    bool put(const std::string& key, const std::string& value) override;
    bool remove(const std::string& key) override;
    std::string get(const std::string& key) override;

    ~StringStorage() override = default;

   protected:
    std::shared_mutex mtx_;
};
}  // namespace storage
