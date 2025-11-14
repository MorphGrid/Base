// Copyright (C) 2025 Ian Torres <iantorres@outlook.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include <gtest/gtest.h>

#include <controllers/operations_controller.hpp>
#include <framework/controller.hpp>
#include <framework/jwt.hpp>
#include <framework/metrics.hpp>
#include <framework/queue.hpp>
#include <framework/route.hpp>
#include <framework/router.hpp>
#include <framework/server.hpp>
#include <framework/state.hpp>
#include <framework/support.hpp>
#include <framework/task_group.hpp>

using namespace framework;

class test_api : public testing::Test {
 public:
  shared_of<server> server_;
  shared_of<std::jthread> thread_;

 protected:
  void SetUp() override {
    server_ = std::make_shared<server>();

    auto _router = server_->get_state()->get_router();

    _router->add(std::make_shared<route>(controllers::operations_controller::verbs(), "/api/operations",
                                         controllers::operations_controller::make()));

    thread_ = std::make_shared<std::jthread>([this]() {
      server_->start();
      server_->get_state()->set_running(false);
    });

    thread_->detach();

    while (server_->get_state()->get_running() == false) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }

  void TearDown() override {
    server_->get_task_group()->emit(boost::asio::cancellation_type::total);
    while (server_->get_state()->get_running() == true) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    server_->get_state()->ioc().stop();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
};

TEST_F(test_api, can_handle_get_operations_request) {
  boost::asio::io_context _client_ioc;
  resolver _resolver(_client_ioc);
  const std::string _host = "127.0.0.1";
  const unsigned short int _port = server_->get_state()->get_port();
  auto const _tcp_resolver_results = _resolver.resolve(_host, std::to_string(_port));
  tcp_stream _stream(_client_ioc);
  _stream.connect(_tcp_resolver_results);

  auto _id = boost::uuids::random_generator()();
  auto _jwt = jwt::make(_id, server_->get_state()->get_key());
  request_type _request{http_verb::get, "/api/operations", 11};
  _request.set(http_field::host, _host);
  _request.set(http_field::user_agent, "Client");
  _request.set(http_field::authorization, _jwt->as_string());
  _request.prepare_payload();

  write(_stream, _request);
  flat_buffer _buffer;

  response_type _response;
  read(_stream, _buffer, _response);

  ASSERT_GT(_response.body().size(), 0);
  ASSERT_EQ(_response.result_int(), 200);

  boost::system::error_code _parse_ec;
  auto _result = boost::json::parse(_response.body(), _parse_ec);

  ASSERT_EQ(_parse_ec, boost::beast::errc::success);
  ASSERT_TRUE(_result.is_object());
  ASSERT_TRUE(_result.as_object().contains("data"));
  ASSERT_TRUE(_result.as_object().at("data").is_array());
  ASSERT_TRUE(_result.as_object().at("data").as_array().size() == 1);
  ASSERT_TRUE(_result.as_object().at("data").as_array().at(0).is_string());
  ASSERT_EQ(_result.as_object().at("data").as_array().at(0).as_string(), "about");

  boost::beast::error_code _ec;
  _stream.socket().shutdown(socket::shutdown_both, _ec);
  ASSERT_EQ(_ec, boost::beast::errc::success);
}