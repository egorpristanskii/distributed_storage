#include <iostream>
#include <memory>
#include "string_storage.h"

int main() {
    std::unique_ptr<storage::StringStorage> storage =
        std::make_unique<storage::StringStorage>();
    storage->put("123", "567");
    std::cout << "123 " << ": " << storage->get("123") << std::endl;
    return 0;
}
