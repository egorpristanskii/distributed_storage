#include "network/router.h"

#include <gtest/gtest.h>

namespace {
constexpr char kTestInput[] = R"({"test":"empty"})";
}

class TestHandler {
   public:
    std::string ping(const nlohmann::json&) { return "pong"; }  // NOLINT
};

TEST(TestRouter, TestPing) {
    auto test_router = router::Router();
    auto test_handler = TestHandler();
    test_router.addRoute("ping", test_handler, &TestHandler::ping);
    std::string result =
        test_router.handleRoute("ping", nlohmann::json::parse(kTestInput));
    ASSERT_STREQ(result.c_str(), "pong");
}
