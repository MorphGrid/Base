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

#ifndef ABOUT_HPP
#define ABOUT_HPP

#include <framework/support.hpp>

using namespace framework;

namespace about {
namespace program {
inline std::string name = "Base";
inline std::string version = "1.0.0";
    std::string get_version();
}  // namespace program

namespace operations {
inline vector_of<std::string> operations = {
    "about",
};
}
}  // namespace about

#endif  // ABOUT_HPP
