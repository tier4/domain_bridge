// Copyright 2026 TIER IV, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <gtest/gtest.h>

#include <filesystem>
#include <string>
#include <vector>

#include "domain_bridge/process_cmd_line_arguments.hpp"

class TestProcessCmdLineArguments : public ::testing::Test
{
protected:
  void SetUp() override
  {
    yaml_path_ =
      (std::filesystem::current_path() / std::filesystem::path{"config"} /
      std::filesystem::path{"services.yaml"}).string();
  }

  std::string yaml_path_;
};

TEST_F(TestProcessCmdLineArguments, services_keep_yaml_domain_ids_without_override)
{
  const std::vector<std::string> args{"domain_bridge", yaml_path_};

  auto config_rc_pair = domain_bridge::process_cmd_line_arguments(args);
  ASSERT_EQ(config_rc_pair.second, 0);
  ASSERT_TRUE(config_rc_pair.first.has_value());

  const auto & services = config_rc_pair.first->services;
  ASSERT_EQ(services.size(), 2u);
  EXPECT_EQ(services[0].first.from_domain_id, 0u);
  EXPECT_EQ(services[0].first.to_domain_id, 1u);
  EXPECT_EQ(services[1].first.from_domain_id, 2u);
  EXPECT_EQ(services[1].first.to_domain_id, 3u);
}

TEST_F(TestProcessCmdLineArguments, services_honor_from_and_to_overrides)
{
  const std::vector<std::string> args{"domain_bridge", "--from", "7", "--to", "8", yaml_path_};

  auto config_rc_pair = domain_bridge::process_cmd_line_arguments(args);
  ASSERT_EQ(config_rc_pair.second, 0);
  ASSERT_TRUE(config_rc_pair.first.has_value());

  const auto & services = config_rc_pair.first->services;
  ASSERT_EQ(services.size(), 2u);
  for (const auto & service_option_pair : services) {
    EXPECT_EQ(service_option_pair.first.from_domain_id, 7u);
    EXPECT_EQ(service_option_pair.first.to_domain_id, 8u);
  }
}
