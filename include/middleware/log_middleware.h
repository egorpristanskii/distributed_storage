#include "middleware/middleware.h"

namespace middleware {
class LogMiddleware final : public IMiddleware {
   private:
    void preprocess(const network::Request& request) const final;

    void postprocess(const network::Response& response) const final;
};
}  // namespace middleware
