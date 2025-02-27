#include <iostream>
#include <memory>
#include "base_storage.h"

int main() {
    std::unique_ptr<storage::BaseStorage> storage =
        std::make_unique<storage::BaseStorage>();
    storage->put("123", "567");
    std::cout << "123 " << ": " << storage->get("123") << std::endl;
    return 0;
}
