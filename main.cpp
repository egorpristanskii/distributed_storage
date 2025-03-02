#include "string_storage.h"

#include <iostream>
#include <memory>

int main() {
    std::unique_ptr<storage::StringStorage> storage =
        std::make_unique<storage::StringStorage>("/tmp/new_log.json");
    storage->put("123", "567");
    std::cout << "123 " << ": " << storage->get("123") << std::endl;
    return 0;
}
