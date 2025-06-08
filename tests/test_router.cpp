#include "network/http_codes.h"
#include "network/response.h"
#include "network/router.h"
#include "network/types.h"

#include <gtest/gtest.h>

namespace {
constexpr char kTestInput[] = R"({"test":"empty"})";
}

class TestHandler {
   public:
    network::Response ping(const nlohmann::json&) {  // NOLINT
        return network::Response(network::HTTPCode::Accepted, "pong");
    }
};

TEST(TestRouter, TestPing) {
    auto test_router = router::Router();
    auto test_handler = std::make_shared<TestHandler>();
    test_router.addRoute(network::HTTPMethod::GET, "ping", test_handler,
                         &TestHandler::ping);
    network::Response result = test_router.handleRoute(
        network::HTTPMethod::GET, "ping", nlohmann::json::parse(kTestInput));
    ASSERT_EQ(result.status_code, network::HTTPCode::Accepted);
    ASSERT_STREQ(result.response_data.c_str(), "pong");
    ASSERT_STREQ(result.toString().c_str(),
                 "HTTP/1.1 202 Accepted\r\n"
                 "Content-Type: application/json\r\n"
                 "Content-Length: 4\r\n\r\n"
                 "pong");
}
