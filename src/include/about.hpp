//
// Created by ian on 13/11/25.
//

#ifndef ABOUT_HPP
#define ABOUT_HPP

#include <framework/support.hpp>

using namespace framework;

namespace about {
namespace program {
inline std::string name = "Base";
inline std::string version = "1.0.0";
}  // namespace program

namespace operations {
inline vector_of<std::string> operations = {
    "about",
};
}
}  // namespace about

#endif  // ABOUT_HPP
