// Copyright 2021, Open Source Robotics Foundation, Inc.
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

#ifndef DOMAIN_BRIDGE__SERVICE_BRIDGE_HPP_
#define DOMAIN_BRIDGE__SERVICE_BRIDGE_HPP_

#include <cstddef>
#include <string>

namespace domain_bridge
{

struct ServiceBridge
{
  std::string service_name;
  std::string type_name;
  std::size_t from_domain_id;
  std::size_t to_domain_id;
};

}  // namespace domain_bridge

#endif  // DOMAIN_BRIDGE__SERVICE_BRIDGE_HPP_
