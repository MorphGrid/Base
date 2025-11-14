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

#include <about.hpp>
#include <controllers/operations_controller.hpp>
#include <framework/controller.hpp>

namespace controllers {
vector_of<http_verb> operations_controller::verbs() {
  return vector_of{
      http_verb::get,
  };
}

shared_controller operations_controller::make() {
  return std::make_shared<controller>([](const shared_state state, const request_type request, const params_type params,
                                         const shared_auth auth) -> async_of<response_type> {
    response_type _response{http_status::ok, request.version()};
    boost::json::array _operations;
    _operations.reserve(about::operations::operations.size());
    for (auto _operation : about::operations::operations) {
      _operations.push_back(boost::json::value{std::string(_operation)});
    }
    _response.body() = serialize(object{
        {"data", _operations},
    });
    _response.prepare_payload();
    co_return _response;
  });
}
}  // namespace controllers
