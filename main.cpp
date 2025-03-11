#include "storage.h"
#include "value.h"

#include <iostream>
#include <memory>

int main() {
    auto storage = std::make_unique<storage::Storage>(
        std::move(std::string("/tmp/new_log.json")));
    storage->put("123", std::make_unique<storage::IntData>(567));
    std::cout << "123 " << ": " << storage->get("123")->toString() << std::endl;
    return 0;
}
