#include <iostream>
#include "baseStorage.h"

int main() {
    storage::BaseStorage storage;
    storage.put("123", "567");
    std::cout << "123 " << ": " << storage.get("123") << std::endl;
    return 0;
}
