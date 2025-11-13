#include <framework/router.hpp>
#include <framework/server.hpp>
#include <framework/state.hpp>

using namespace framework;

int main() {
  const auto _server = std::make_shared<server>();
  shared_router _router = _server->get_state()->get_router();
  _server->start(9000);
  return 0;
}