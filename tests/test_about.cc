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

#include <about.hpp>

TEST(test_about, contains_program_name) { ASSERT_EQ(about::program::name, "Base"); }
TEST(test_about, contains_program_version) { ASSERT_EQ(about::program::version, "1.0.0"); }
TEST(test_about, contains_program_version_as_method) { ASSERT_EQ(about::program::get_version(), "1.0.0"); }