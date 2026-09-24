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

#ifndef DOMAIN_BRIDGE__SERVICE_TYPESUPPORT_HELPERS_HPP_
#define DOMAIN_BRIDGE__SERVICE_TYPESUPPORT_HELPERS_HPP_

#include <memory>
#include <string>
#include <tuple>

#include "rcpputils/shared_library.hpp"
#include "rosidl_runtime_c/service_type_support_struct.h"
#include "rosidl_typesupport_introspection_cpp/service_introspection.hpp"

#include "domain_bridge/visibility_control.hpp"

namespace domain_bridge
{

/// Load the shared library containing a service's typesupport.
/**
 * \param type Service type string, e.g. "example_interfaces/srv/AddTwoInts"
 * \param typesupport_identifier Typesupport identifier, e.g. "rosidl_typesupport_cpp"
 * \return Shared pointer to the loaded library (must be kept alive while handles are in use)
 * \throws std::runtime_error if the library cannot be loaded
 */
DOMAIN_BRIDGE_PUBLIC
std::shared_ptr<rcpputils::SharedLibrary>
get_service_typesupport_library(
  const std::string & type,
  const std::string & typesupport_identifier);

/// Get the service typesupport handle from an already-loaded library.
/**
 * \param type Service type string, e.g. "example_interfaces/srv/AddTwoInts"
 * \param typesupport_identifier Typesupport identifier, e.g. "rosidl_typesupport_cpp"
 * \param library Shared library previously loaded via get_service_typesupport_library()
 * \return Pointer to the service typesupport handle
 * \throws std::runtime_error if the symbol is not found in the library
 */
DOMAIN_BRIDGE_PUBLIC
const rosidl_service_type_support_t *
get_service_typesupport_handle(
  const std::string & type,
  const std::string & typesupport_identifier,
  std::shared_ptr<rcpputils::SharedLibrary> library);

/// Get the introspection ServiceMembers for a service type.
/**
 * Loads the introspection typesupport library and returns the ServiceMembers
 * struct, which provides request_members_ and response_members_ for
 * allocating request/response message objects at runtime.
 *
 * \param type Service type string, e.g. "example_interfaces/srv/AddTwoInts"
 * \return Tuple of (library, ServiceMembers pointer); the library must be kept alive
 * \throws std::runtime_error if the typesupport cannot be loaded
 */
DOMAIN_BRIDGE_PUBLIC
std::tuple<
  std::shared_ptr<rcpputils::SharedLibrary>,
  const rosidl_typesupport_introspection_cpp::ServiceMembers *>
get_service_members(const std::string & type);

}  // namespace domain_bridge

#endif  // DOMAIN_BRIDGE__SERVICE_TYPESUPPORT_HELPERS_HPP_
