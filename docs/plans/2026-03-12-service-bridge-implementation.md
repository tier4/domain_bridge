# Service Bridge Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Add dynamic (YAML-configurable) service bridging to domain_bridge by backporting GenericService/GenericClient from Rolling rclcpp into the domain_bridge package, targeting Humble.

**Architecture:** Backport `GenericService` and `GenericClient` from Rolling's rclcpp as domain_bridge-internal classes (`domain_bridge::GenericService`, `domain_bridge::GenericClient`). These inherit from Humble's `rclcpp::ServiceBase`/`rclcpp::ClientBase` and use dynamically-loaded introspection typesupport to handle arbitrary service types at runtime. A new string-based `bridge_service()` overload uses these to bridge services configured via YAML, just like topics.

**Tech Stack:** C++17, rclcpp (Humble 16.x), rcl, rosidl_typesupport_cpp, rosidl_typesupport_introspection_cpp, rcpputils, yaml-cpp

**Design Doc:** `docs/plans/2026-03-12-service-bridge-design.md`

---

### Task 1: Service Typesupport Helpers

Implement helper functions that dynamically load service typesupport from a type name string (e.g., `"example_interfaces/srv/AddTwoInts"`). This is the foundation for GenericService and GenericClient.

**Files:**
- Create: `include/domain_bridge/service_typesupport_helpers.hpp`
- Create: `src/domain_bridge/service_typesupport_helpers.cpp`

**Step 1: Create the header**

```cpp
// include/domain_bridge/service_typesupport_helpers.hpp
// Copyright 2026 Tier IV, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// ...

#ifndef DOMAIN_BRIDGE__SERVICE_TYPESUPPORT_HELPERS_HPP_
#define DOMAIN_BRIDGE__SERVICE_TYPESUPPORT_HELPERS_HPP_

#include <memory>
#include <string>
#include <tuple>

#include "rcpputils/shared_library.hpp"
#include "rosidl_typesupport_introspection_cpp/service_introspection.hpp"

#include "domain_bridge/visibility_control.hpp"

namespace domain_bridge
{

/// Load a shared library containing the typesupport for a given service type.
/// @param type Service type string, e.g. "example_interfaces/srv/AddTwoInts"
/// @param typesupport_id Typesupport identifier, e.g. "rosidl_typesupport_cpp"
/// @return Shared pointer to the loaded library
/// @throws std::runtime_error if the library cannot be loaded
DOMAIN_BRIDGE_PUBLIC
std::shared_ptr<rcpputils::SharedLibrary>
get_service_typesupport_library(
  const std::string & type,
  const std::string & typesupport_id);

/// Get the service typesupport handle from a loaded library.
/// @param type Service type string, e.g. "example_interfaces/srv/AddTwoInts"
/// @param typesupport_id Typesupport identifier, e.g. "rosidl_typesupport_cpp"
/// @param library The loaded typesupport library
/// @return Pointer to the service typesupport handle
/// @throws std::runtime_error if the symbol cannot be found
DOMAIN_BRIDGE_PUBLIC
const rosidl_service_type_support_t *
get_service_typesupport_handle(
  const std::string & type,
  const std::string & typesupport_id,
  rcpputils::SharedLibrary & library);

/// Get introspection ServiceMembers for a service type.
/// Provides access to request_members_ and response_members_ for
/// creating and destroying request/response objects at runtime.
/// @param type Service type string, e.g. "example_interfaces/srv/AddTwoInts"
/// @return Tuple of (library, ServiceMembers pointer). The library must be kept alive.
/// @throws std::runtime_error if the type cannot be found
DOMAIN_BRIDGE_PUBLIC
std::tuple<
  std::shared_ptr<rcpputils::SharedLibrary>,
  const rosidl_typesupport_introspection_cpp::ServiceMembers *>
get_service_members(const std::string & type);

}  // namespace domain_bridge

#endif  // DOMAIN_BRIDGE__SERVICE_TYPESUPPORT_HELPERS_HPP_
```

**Step 2: Create the implementation**

```cpp
// src/domain_bridge/service_typesupport_helpers.cpp

#include "domain_bridge/service_typesupport_helpers.hpp"

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>

#include "rcpputils/shared_library.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/service_introspection.hpp"

namespace domain_bridge
{

// Split "example_interfaces/srv/AddTwoInts" into ("example_interfaces", "srv", "AddTwoInts")
static std::tuple<std::string, std::string, std::string>
extract_type_parts(const std::string & type)
{
  auto sep1 = type.find('/');
  if (sep1 == std::string::npos) {
    throw std::runtime_error("Invalid service type format: " + type);
  }
  auto sep2 = type.find('/', sep1 + 1);
  if (sep2 == std::string::npos) {
    throw std::runtime_error("Invalid service type format: " + type);
  }
  return {
    type.substr(0, sep1),
    type.substr(sep1 + 1, sep2 - sep1 - 1),
    type.substr(sep2 + 1)
  };
}

// Build the library path for a typesupport library
static std::string
get_typesupport_library_path(const std::string & package_name, const std::string & typesupport_id)
{
  const char * dynamic_lib_ext = rcpputils::get_platform_library_name("").c_str();
  (void)dynamic_lib_ext;
  return rcpputils::get_platform_library_name(package_name + "__" + typesupport_id);
}

std::shared_ptr<rcpputils::SharedLibrary>
get_service_typesupport_library(
  const std::string & type,
  const std::string & typesupport_id)
{
  auto [package_name, middle, _] = extract_type_parts(type);
  (void)middle;
  (void)_;

  auto library_path = get_typesupport_library_path(package_name, typesupport_id);
  try {
    return std::make_shared<rcpputils::SharedLibrary>(library_path);
  } catch (const std::exception & e) {
    throw std::runtime_error(
      "Failed to load typesupport library for service type '" + type + "': " + e.what());
  }
}

const rosidl_service_type_support_t *
get_service_typesupport_handle(
  const std::string & type,
  const std::string & typesupport_id,
  rcpputils::SharedLibrary & library)
{
  auto [package_name, middle, type_name] = extract_type_parts(type);

  // Construct symbol name:
  // rosidl_typesupport_cpp__get_service_type_support_handle__<pkg>__<middle>__<TypeName>
  std::ostringstream symbol_name;
  symbol_name << typesupport_id
              << "__get_service_type_support_handle__"
              << package_name << "__" << middle << "__" << type_name;

  if (!library.has_symbol(symbol_name.str())) {
    throw std::runtime_error(
      "Failed to find service typesupport symbol '" + symbol_name.str() +
      "' for type '" + type + "'");
  }

  using GetServiceTypeSupportFunc =
    const rosidl_service_type_support_t * (*)();
  auto get_ts = reinterpret_cast<GetServiceTypeSupportFunc>(
    library.get_symbol(symbol_name.str()));
  return get_ts();
}

std::tuple<
  std::shared_ptr<rcpputils::SharedLibrary>,
  const rosidl_typesupport_introspection_cpp::ServiceMembers *>
get_service_members(const std::string & type)
{
  auto library = get_service_typesupport_library(
    type, "rosidl_typesupport_introspection_cpp");

  const auto * service_ts = get_service_typesupport_handle(
    type, "rosidl_typesupport_introspection_cpp", *library);

  const auto * service_members =
    static_cast<const rosidl_typesupport_introspection_cpp::ServiceMembers *>(
      service_ts->data);

  if (!service_members) {
    throw std::runtime_error(
      "Failed to get ServiceMembers for type '" + type + "'");
  }

  return {library, service_members};
}

}  // namespace domain_bridge
```

**Step 3: Add to CMakeLists.txt**

In `CMakeLists.txt`, add `rosidl_typesupport_introspection_cpp` and `rcpputils` as dependencies, and add the new source file to the library:

- Add `find_package(rosidl_typesupport_introspection_cpp REQUIRED)` and `find_package(rcpputils REQUIRED)` after existing find_package calls
- Add `src/${PROJECT_NAME}/service_typesupport_helpers.cpp` to the `add_library` sources
- Add `rosidl_typesupport_introspection_cpp` and `rcpputils` to `ament_target_dependencies`
- Add `rosidl_typesupport_introspection_cpp` and `rcpputils` to `ament_export_dependencies`

**Step 4: Build and verify**

Run: `colcon build --packages-select domain_bridge`
Expected: Build succeeds

**Step 5: Commit**

```bash
git add include/domain_bridge/service_typesupport_helpers.hpp \
        src/domain_bridge/service_typesupport_helpers.cpp \
        CMakeLists.txt
git commit -m "feat: add service typesupport helpers for dynamic type loading"
```

---

### Task 2: GenericService Class

Backport `GenericService` from Rolling's rclcpp, adapted for Humble's `ServiceBase` API. This class creates a service server from a type name string at runtime.

**Files:**
- Create: `include/domain_bridge/generic_service.hpp`
- Create: `src/domain_bridge/generic_service.cpp`

**Step 1: Create the header**

```cpp
// include/domain_bridge/generic_service.hpp

#ifndef DOMAIN_BRIDGE__GENERIC_SERVICE_HPP_
#define DOMAIN_BRIDGE__GENERIC_SERVICE_HPP_

#include <functional>
#include <memory>
#include <string>

#include "rcpputils/shared_library.hpp"
#include "rclcpp/service.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"

#include "domain_bridge/visibility_control.hpp"

namespace domain_bridge
{

class GenericService;

/// Callback type for GenericService that supports deferred responses.
/// Parameters: (service_handle, request_header, request)
using GenericServiceCallback = std::function<void(
  std::shared_ptr<GenericService>,
  std::shared_ptr<rmw_request_id_t>,
  std::shared_ptr<void>)>;

/// A type-erased service server that can be created from a service type name string.
/// Backported from Rolling rclcpp's GenericService, adapted for Humble's ServiceBase API.
// TODO(jazzy): On Jazzy+, adapt virtual method signatures to const reference parameters
// TODO(rolling): Replace with rclcpp::GenericService when available
class GenericService
  : public rclcpp::ServiceBase,
    public std::enable_shared_from_this<GenericService>
{
public:
  using SharedRequest = std::shared_ptr<void>;
  using SharedResponse = std::shared_ptr<void>;

  DOMAIN_BRIDGE_PUBLIC
  GenericService(
    std::shared_ptr<rcl_node_t> node_handle,
    const std::string & service_name,
    const std::string & service_type,
    GenericServiceCallback callback,
    rcl_service_options_t & service_options);

  DOMAIN_BRIDGE_PUBLIC
  ~GenericService() override = default;

  /// Create a new request object (allocated via introspection typesupport).
  DOMAIN_BRIDGE_PUBLIC
  std::shared_ptr<void> create_request() override;

  /// Create a new request header.
  DOMAIN_BRIDGE_PUBLIC
  std::shared_ptr<rmw_request_id_t> create_request_header() override;

  /// Handle an incoming request (called by rclcpp executor).
  // Humble signature: by-value shared_ptr parameters
  DOMAIN_BRIDGE_PUBLIC
  void handle_request(
    std::shared_ptr<rmw_request_id_t> request_header,
    std::shared_ptr<void> request) override;

  /// Create a new response object.
  DOMAIN_BRIDGE_PUBLIC
  SharedResponse create_response();

  /// Send a response back to the client.
  DOMAIN_BRIDGE_PUBLIC
  void send_response(rmw_request_id_t & req_id, SharedResponse response);

private:
  GenericServiceCallback callback_;
  std::shared_ptr<rcpputils::SharedLibrary> ts_lib_;
  std::shared_ptr<rcpputils::SharedLibrary> intro_ts_lib_;
  const rosidl_typesupport_introspection_cpp::MessageMembers * request_members_;
  const rosidl_typesupport_introspection_cpp::MessageMembers * response_members_;
};

}  // namespace domain_bridge

#endif  // DOMAIN_BRIDGE__GENERIC_SERVICE_HPP_
```

**Step 2: Create the implementation**

```cpp
// src/domain_bridge/generic_service.cpp

#include "domain_bridge/generic_service.hpp"

#include <memory>
#include <string>

#include "rcl/service.h"
#include "rcpputils/shared_library.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"

#include "domain_bridge/service_typesupport_helpers.hpp"

namespace domain_bridge
{

GenericService::GenericService(
  std::shared_ptr<rcl_node_t> node_handle,
  const std::string & service_name,
  const std::string & service_type,
  GenericServiceCallback callback,
  rcl_service_options_t & service_options)
: rclcpp::ServiceBase(node_handle),
  callback_(std::move(callback))
{
  // Load the typesupport for rcl_service_init
  ts_lib_ = get_service_typesupport_library(service_type, "rosidl_typesupport_cpp");
  const auto * service_ts = get_service_typesupport_handle(
    service_type, "rosidl_typesupport_cpp", *ts_lib_);

  // Load introspection typesupport for request/response allocation
  auto [intro_lib, service_members] = get_service_members(service_type);
  intro_ts_lib_ = intro_lib;
  request_members_ = service_members->request_members_;
  response_members_ = service_members->response_members_;

  // Initialize the rcl service
  rcl_ret_t ret = rcl_service_init(
    get_service_handle().get(),
    node_handle.get(),
    service_ts,
    service_name.c_str(),
    &service_options);
  if (ret != RCL_RET_OK) {
    rclcpp::exceptions::throw_from_rcl_error(ret, "Failed to create service");
  }
}

std::shared_ptr<void>
GenericService::create_request()
{
  // Allocate and zero-initialize a request message using introspection
  auto request = std::shared_ptr<void>(
    new uint8_t[request_members_->size_of_],
    [this](void * ptr) {
      request_members_->fini_function(ptr);
      delete[] static_cast<uint8_t *>(ptr);
    });
  request_members_->init_function(request.get(), rosidl_runtime_cpp::MessageInitialization::ZERO);
  return request;
}

std::shared_ptr<rmw_request_id_t>
GenericService::create_request_header()
{
  return std::make_shared<rmw_request_id_t>();
}

void
GenericService::handle_request(
  std::shared_ptr<rmw_request_id_t> request_header,
  std::shared_ptr<void> request)
{
  callback_(shared_from_this(), std::move(request_header), std::move(request));
}

GenericService::SharedResponse
GenericService::create_response()
{
  auto response = std::shared_ptr<void>(
    new uint8_t[response_members_->size_of_],
    [this](void * ptr) {
      response_members_->fini_function(ptr);
      delete[] static_cast<uint8_t *>(ptr);
    });
  response_members_->init_function(
    response.get(), rosidl_runtime_cpp::MessageInitialization::ZERO);
  return response;
}

void
GenericService::send_response(rmw_request_id_t & req_id, SharedResponse response)
{
  rcl_ret_t ret = rcl_send_response(
    get_service_handle().get(),
    &req_id,
    response.get());
  if (ret != RCL_RET_OK) {
    rclcpp::exceptions::throw_from_rcl_error(ret, "Failed to send service response");
  }
}

}  // namespace domain_bridge
```

**Step 3: Add to CMakeLists.txt**

Add `src/${PROJECT_NAME}/generic_service.cpp` to the `add_library` sources.

**Step 4: Build and verify**

Run: `colcon build --packages-select domain_bridge`
Expected: Build succeeds

**Step 5: Commit**

```bash
git add include/domain_bridge/generic_service.hpp \
        src/domain_bridge/generic_service.cpp \
        CMakeLists.txt
git commit -m "feat: add GenericService backported from Rolling rclcpp"
```

---

### Task 3: GenericClient Class

Backport `GenericClient` from Rolling's rclcpp, adapted for Humble's `ClientBase` API.

**Files:**
- Create: `include/domain_bridge/generic_client.hpp`
- Create: `src/domain_bridge/generic_client.cpp`

**Step 1: Create the header**

```cpp
// include/domain_bridge/generic_client.hpp

#ifndef DOMAIN_BRIDGE__GENERIC_CLIENT_HPP_
#define DOMAIN_BRIDGE__GENERIC_CLIENT_HPP_

#include <functional>
#include <future>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>

#include "rcpputils/shared_library.hpp"
#include "rclcpp/client.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"

#include "domain_bridge/visibility_control.hpp"

namespace domain_bridge
{

/// A type-erased service client that can be created from a service type name string.
/// Backported from Rolling rclcpp's GenericClient, adapted for Humble's ClientBase API.
// TODO(jazzy): On Jazzy+, use rclcpp::GenericClient directly (available natively)
// TODO(rolling): Replace with rclcpp::GenericClient when available
class GenericClient : public rclcpp::ClientBase
{
public:
  using SharedRequest = std::shared_ptr<void>;
  using SharedResponse = std::shared_ptr<void>;
  using SharedFuture = std::shared_future<SharedResponse>;
  using ResponseCallback = std::function<void(SharedFuture)>;

  DOMAIN_BRIDGE_PUBLIC
  GenericClient(
    rclcpp::node_interfaces::NodeBaseInterface * node_base,
    rclcpp::node_interfaces::NodeGraphInterface::SharedPtr node_graph,
    const std::string & service_name,
    const std::string & service_type,
    rcl_client_options_t & client_options);

  DOMAIN_BRIDGE_PUBLIC
  ~GenericClient() override = default;

  /// Create a new response object.
  DOMAIN_BRIDGE_PUBLIC
  std::shared_ptr<void> create_response() override;

  /// Create a new request header.
  DOMAIN_BRIDGE_PUBLIC
  std::shared_ptr<rmw_request_id_t> create_request_header() override;

  /// Handle a response (called by rclcpp executor).
  // Humble signature: by-value shared_ptr parameters
  DOMAIN_BRIDGE_PUBLIC
  void handle_response(
    std::shared_ptr<rmw_request_id_t> request_header,
    std::shared_ptr<void> response) override;

  /// Create a new request object.
  DOMAIN_BRIDGE_PUBLIC
  SharedRequest create_request();

  /// Send a request asynchronously with a callback for the response.
  DOMAIN_BRIDGE_PUBLIC
  SharedFuture async_send_request(SharedRequest request, ResponseCallback callback);

  /// Send a request asynchronously, returning a future.
  DOMAIN_BRIDGE_PUBLIC
  SharedFuture async_send_request(SharedRequest request);

private:
  std::shared_ptr<rcpputils::SharedLibrary> ts_lib_;
  std::shared_ptr<rcpputils::SharedLibrary> intro_ts_lib_;
  const rosidl_typesupport_introspection_cpp::MessageMembers * request_members_;
  const rosidl_typesupport_introspection_cpp::MessageMembers * response_members_;

  std::mutex pending_requests_mutex_;
  std::map<int64_t, std::pair<std::promise<SharedResponse>, ResponseCallback>> pending_requests_;
};

}  // namespace domain_bridge

#endif  // DOMAIN_BRIDGE__GENERIC_CLIENT_HPP_
```

**Step 2: Create the implementation**

```cpp
// src/domain_bridge/generic_client.cpp

#include "domain_bridge/generic_client.hpp"

#include <memory>
#include <string>
#include <utility>

#include "rcl/client.h"
#include "rcpputils/shared_library.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"

#include "domain_bridge/service_typesupport_helpers.hpp"

namespace domain_bridge
{

GenericClient::GenericClient(
  rclcpp::node_interfaces::NodeBaseInterface * node_base,
  rclcpp::node_interfaces::NodeGraphInterface::SharedPtr node_graph,
  const std::string & service_name,
  const std::string & service_type,
  rcl_client_options_t & client_options)
: rclcpp::ClientBase(node_base, std::move(node_graph))
{
  // Load typesupport for rcl_client_init
  ts_lib_ = get_service_typesupport_library(service_type, "rosidl_typesupport_cpp");
  const auto * service_ts = get_service_typesupport_handle(
    service_type, "rosidl_typesupport_cpp", *ts_lib_);

  // Load introspection typesupport for request/response allocation
  auto [intro_lib, service_members] = get_service_members(service_type);
  intro_ts_lib_ = intro_lib;
  request_members_ = service_members->request_members_;
  response_members_ = service_members->response_members_;

  // Initialize the rcl client
  rcl_ret_t ret = rcl_client_init(
    get_client_handle().get(),
    node_base->get_rcl_node_handle(),
    service_ts,
    service_name.c_str(),
    &client_options);
  if (ret != RCL_RET_OK) {
    rclcpp::exceptions::throw_from_rcl_error(ret, "Failed to create client");
  }
}

std::shared_ptr<void>
GenericClient::create_response()
{
  auto response = std::shared_ptr<void>(
    new uint8_t[response_members_->size_of_],
    [this](void * ptr) {
      response_members_->fini_function(ptr);
      delete[] static_cast<uint8_t *>(ptr);
    });
  response_members_->init_function(
    response.get(), rosidl_runtime_cpp::MessageInitialization::ZERO);
  return response;
}

std::shared_ptr<rmw_request_id_t>
GenericClient::create_request_header()
{
  return std::make_shared<rmw_request_id_t>();
}

void
GenericClient::handle_response(
  std::shared_ptr<rmw_request_id_t> request_header,
  std::shared_ptr<void> response)
{
  std::lock_guard<std::mutex> lock(pending_requests_mutex_);
  auto it = pending_requests_.find(request_header->sequence_number);
  if (it == pending_requests_.end()) {
    return;
  }
  auto & [promise, callback] = it->second;
  promise.set_value(std::move(response));
  if (callback) {
    auto future = promise.get_future().share();
    callback(future);
  }
  pending_requests_.erase(it);
}

GenericClient::SharedRequest
GenericClient::create_request()
{
  auto request = std::shared_ptr<void>(
    new uint8_t[request_members_->size_of_],
    [this](void * ptr) {
      request_members_->fini_function(ptr);
      delete[] static_cast<uint8_t *>(ptr);
    });
  request_members_->init_function(request.get(), rosidl_runtime_cpp::MessageInitialization::ZERO);
  return request;
}

GenericClient::SharedFuture
GenericClient::async_send_request(SharedRequest request, ResponseCallback callback)
{
  int64_t sequence_number;
  rcl_ret_t ret = rcl_send_request(
    get_client_handle().get(),
    request.get(),
    &sequence_number);
  if (ret != RCL_RET_OK) {
    rclcpp::exceptions::throw_from_rcl_error(ret, "Failed to send service request");
  }

  std::lock_guard<std::mutex> lock(pending_requests_mutex_);
  auto & entry = pending_requests_[sequence_number];
  entry.second = std::move(callback);
  return entry.first.get_future().share();
}

GenericClient::SharedFuture
GenericClient::async_send_request(SharedRequest request)
{
  return async_send_request(std::move(request), nullptr);
}

}  // namespace domain_bridge
```

**Step 3: Add to CMakeLists.txt**

Add `src/${PROJECT_NAME}/generic_client.cpp` to the `add_library` sources.

**Step 4: Build and verify**

Run: `colcon build --packages-select domain_bridge`
Expected: Build succeeds

**Step 5: Commit**

```bash
git add include/domain_bridge/generic_client.hpp \
        src/domain_bridge/generic_client.cpp \
        CMakeLists.txt
git commit -m "feat: add GenericClient backported from Rolling rclcpp"
```

---

### Task 4: ServiceBridge Struct and DomainBridgeConfig Extension

Add a `ServiceBridge` struct (similar to `TopicBridge`) and extend `DomainBridgeConfig` to hold service bridge entries.

**Files:**
- Create: `include/domain_bridge/service_bridge.hpp`
- Modify: `include/domain_bridge/domain_bridge_config.hpp`

**Step 1: Create ServiceBridge struct**

Create `include/domain_bridge/service_bridge.hpp` following the same pattern as `include/domain_bridge/topic_bridge.hpp`:

```cpp
// include/domain_bridge/service_bridge.hpp

#ifndef DOMAIN_BRIDGE__SERVICE_BRIDGE_HPP_
#define DOMAIN_BRIDGE__SERVICE_BRIDGE_HPP_

#include <cstddef>
#include <string>

namespace domain_bridge
{

/// Info about a service bridge
struct ServiceBridge
{
  std::string service_name;
  std::string type_name;  // e.g. "example_interfaces/srv/AddTwoInts"
  std::size_t from_domain_id;
  std::size_t to_domain_id;
};

}  // namespace domain_bridge

#endif  // DOMAIN_BRIDGE__SERVICE_BRIDGE_HPP_
```

**Step 2: Extend DomainBridgeConfig**

Modify `include/domain_bridge/domain_bridge_config.hpp`:
- Add `#include "domain_bridge/service_bridge.hpp"` and `#include "domain_bridge/service_bridge_options.hpp"`
- Add `std::vector<std::pair<ServiceBridge, ServiceBridgeOptions>> services;` member to `DomainBridgeConfig`

**Step 3: Build and verify**

Run: `colcon build --packages-select domain_bridge`
Expected: Build succeeds

**Step 4: Commit**

```bash
git add include/domain_bridge/service_bridge.hpp \
        include/domain_bridge/domain_bridge_config.hpp
git commit -m "feat: add ServiceBridge struct and extend DomainBridgeConfig"
```

---

### Task 5: YAML Parser Extension for Services

Extend the YAML parser to handle the `services:` section.

**Files:**
- Modify: `src/domain_bridge/parse_domain_bridge_yaml_config.cpp`

**Step 1: Add services parsing**

In `src/domain_bridge/parse_domain_bridge_yaml_config.cpp`, in the `update_domain_bridge_config_from_yaml()` function, after the `topics` parsing block (after line 292), add a `services` parsing block:

```cpp
  if (config["services"]) {
    if (config["services"].Type() != YAML::NodeType::Map) {
      throw YamlParsingError(file_path, "expected map value for 'services'");
    }
    for (const auto & service_node : config["services"]) {
      const std::string service = service_node.first.as<std::string>();

      auto service_info = service_node.second;
      if (service_info.Type() != YAML::NodeType::Map) {
        throw YamlParsingError(file_path, "expected map value for each service");
      }

      if (!service_info["type"]) {
        throw YamlParsingError(file_path, "missing 'type' for service '" + service + "'");
      }
      const std::string type = service_info["type"].as<std::string>();

      std::size_t from_domain_id = default_from_domain;
      if (service_info["from_domain"]) {
        from_domain_id = service_info["from_domain"].as<std::size_t>();
      } else {
        if (!is_default_from_domain) {
          throw YamlParsingError(file_path, "missing 'from_domain' for service '" + service + "'");
        }
      }

      std::size_t to_domain_id = default_to_domain;
      if (service_info["to_domain"]) {
        to_domain_id = service_info["to_domain"].as<std::size_t>();
      } else {
        if (!is_default_to_domain) {
          throw YamlParsingError(file_path, "missing 'to_domain' for service '" + service + "'");
        }
      }

      ServiceBridgeOptions options;
      if (service_info["remap"]) {
        options.remap_name(service_info["remap"].as<std::string>());
      }

      domain_bridge_config.services.push_back(
        {{service, type, from_domain_id, to_domain_id}, options});
    }
  }
```

Also add `#include "domain_bridge/service_bridge.hpp"` and `#include "domain_bridge/service_bridge_options.hpp"` at the top of the file.

**Step 2: Build and verify**

Run: `colcon build --packages-select domain_bridge`
Expected: Build succeeds

**Step 3: Commit**

```bash
git add src/domain_bridge/parse_domain_bridge_yaml_config.cpp
git commit -m "feat: extend YAML parser to handle services section"
```

---

### Task 6: String-based bridge_service and Config-driven Service Bridging

Add the new string-based `bridge_service()` method and wire up config-driven service bridging.

**Files:**
- Modify: `include/domain_bridge/domain_bridge.hpp`
- Modify: `src/domain_bridge/domain_bridge.cpp`

**Step 1: Add string-based bridge_service declaration**

In `include/domain_bridge/domain_bridge.hpp`, after the existing template `bridge_service` (line 149), add:

```cpp
  /// Bridge a service from one domain to another using a service type name string.
  /**
   * Uses GenericService/GenericClient to bridge the service without compile-time type information.
   *
   * \param service: Name of the service to be bridged.
   * \param type: Service type name (e.g. "example_interfaces/srv/AddTwoInts").
   * \param from_domain_id: Domain id where the actual service server exists.
   * \param to_domain_id: Domain id where a proxy service server will be created.
   * \param options: Options for bridging the service.
   */
  DOMAIN_BRIDGE_PUBLIC
  void bridge_service(
    const std::string & service,
    const std::string & type,
    size_t from_domain_id,
    size_t to_domain_id,
    const ServiceBridgeOptions & options = ServiceBridgeOptions());
```

**Step 2: Implement bridge_service in domain_bridge.cpp**

In `src/domain_bridge/domain_bridge.cpp`:

1. Add includes at the top:
   ```cpp
   #include "domain_bridge/generic_service.hpp"
   #include "domain_bridge/generic_client.hpp"
   #include "domain_bridge/service_typesupport_helpers.hpp"
   #include "domain_bridge/service_bridge.hpp"
   ```

2. Add the `bridge_service` implementation before `DomainBridge::get_bridged_topics()`:
   ```cpp
   void DomainBridge::bridge_service(
     const std::string & service_name,
     const std::string & type,
     std::size_t from_domain_id,
     std::size_t to_domain_id,
     const ServiceBridgeOptions & options)
   {
     const auto & node_name = impl_->options_.name();
     const std::string & resolved_service_name = rclcpp::expand_topic_or_service_name(
       service_name, node_name, "/", true);

     std::string service_remapped = resolved_service_name;
     if (!options.remap_name().empty()) {
       service_remapped = rclcpp::expand_topic_or_service_name(
         options.remap_name(), node_name, "/", true);
     }

     detail::ServiceBridge service_bridge = {
       resolved_service_name,
       from_domain_id,
       to_domain_id
     };

     if (impl_->is_bridging_service(service_bridge)) {
       std::cerr << "Service '" << resolved_service_name << "'"
                 << " already bridged from domain " << std::to_string(from_domain_id)
                 << " to domain " << std::to_string(to_domain_id)
                 << ", ignoring" << std::endl;
       return;
     }

     rclcpp::Node::SharedPtr from_domain_node =
       impl_->get_node_for_domain(from_domain_id);
     rclcpp::Node::SharedPtr to_domain_node =
       impl_->get_node_for_domain(to_domain_id);

     // Create a generic client for the 'from_domain'
     rcl_client_options_t client_options = rcl_client_get_default_options();
     auto client = std::make_shared<domain_bridge::GenericClient>(
       to_domain_node->get_node_base_interface().get(),
       to_domain_node->get_node_graph_interface(),
       resolved_service_name,
       type,
       client_options);

     // Create a generic service factory for the 'to_domain'
     auto create_service_cb = [
       from_domain_node,
       service_remapped,
       type,
       client]()
       -> std::shared_ptr<rclcpp::ServiceBase>
     {
       rcl_service_options_t service_options = rcl_service_get_default_options();
       auto handle_request =
         [client](
           std::shared_ptr<domain_bridge::GenericService> me,
           std::shared_ptr<rmw_request_id_t> request_header,
           std::shared_ptr<void> request) -> void
         {
           client->async_send_request(
             std::move(request),
             [me, request_header](domain_bridge::GenericClient::SharedFuture future_response)
             {
               auto response = future_response.get();
               me->send_response(*request_header, response);
             });
         };

       return std::make_shared<domain_bridge::GenericService>(
         from_domain_node->get_node_base_interface()->get_shared_rcl_node_handle(),
         service_remapped,
         type,
         handle_request,
         service_options);
     };

     impl_->add_service_bridge(
       to_domain_node, service_bridge, create_service_cb, client);
   }
   ```

   **Important note on domain semantics:**
   - `from_domain` = where the actual service server lives → we create GenericClient here to call it
   - `to_domain` = where we expose a proxy service server → we create GenericService here
   - Wait for the actual server on `from_domain` using the client, then create the proxy on `to_domain`

   Wait — looking at the existing template implementation in `service_bridge_impl.inc` more carefully:
   - `from_domain_node->create_client<ServiceT>(...)` — client on `from_domain`
   - `to_domain_node->create_service<ServiceT>(...)` — service on `to_domain`
   - `add_service_bridge` uses the client to wait for server availability on `from_domain`

   So we need:
   - GenericClient on `from_domain_node` (to call the actual server)
   - GenericService on `to_domain_node` (to receive proxied requests)

   The `add_service_bridge` function receives the `from_domain_node` and `client` to monitor for server availability. Let me correct the implementation:

   ```cpp
   // Create a generic client on from_domain
   auto client = std::make_shared<domain_bridge::GenericClient>(
     from_domain_node->get_node_base_interface().get(),
     from_domain_node->get_node_graph_interface(),
     resolved_service_name,
     type,
     client_options);

   // Factory to create GenericService on to_domain when server is found
   auto create_service_cb = [
     to_domain_node,
     service_remapped,
     type,
     client]()
     -> std::shared_ptr<rclcpp::ServiceBase>
   {
     // ... (create GenericService on to_domain_node)
     return std::make_shared<domain_bridge::GenericService>(
       to_domain_node->get_node_base_interface()->get_shared_rcl_node_handle(),
       service_remapped,
       type,
       handle_request,
       service_options);
   };

   impl_->add_service_bridge(
     from_domain_node, service_bridge, create_service_cb, client);
   ```

3. In the `DomainBridge::DomainBridge(const DomainBridgeConfig & config)` constructor, after the topic bridging loop, add service bridging:

   ```cpp
   for (const auto & service_bridge_pair : config.services) {
     const auto & sb = service_bridge_pair.first;
     bridge_service(
       sb.service_name, sb.type_name,
       sb.from_domain_id, sb.to_domain_id,
       service_bridge_pair.second);
   }
   ```

**Step 3: Build and verify**

Run: `colcon build --packages-select domain_bridge`
Expected: Build succeeds

**Step 4: Commit**

```bash
git add include/domain_bridge/domain_bridge.hpp \
        src/domain_bridge/domain_bridge.cpp
git commit -m "feat: add string-based bridge_service with GenericService/GenericClient"
```

---

### Task 7: End-to-End Service Bridge Test

Write an end-to-end test that verifies dynamic service bridging works.

**Files:**
- Create: `test/domain_bridge/test_generic_service_bridge.cpp`
- Modify: `test/CMakeLists.txt`

**Step 1: Write the test**

Create `test/domain_bridge/test_generic_service_bridge.cpp`. Model it after the existing `test_domain_bridge_services.cpp` but use the string-based API and `example_interfaces/srv/AddTwoInts` for a meaningful request/response test:

```cpp
// test/domain_bridge/test_generic_service_bridge.cpp

#include <gtest/gtest.h>

#include <atomic>
#include <future>
#include <memory>
#include <string>
#include <thread>
#include <utility>

#include "rclcpp/context.hpp"
#include "rclcpp/executors.hpp"
#include "rclcpp/node.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

#include "domain_bridge/domain_bridge.hpp"

static constexpr std::size_t kDomain1{1u};
static constexpr std::size_t kDomain2{2u};

using namespace std::chrono_literals;

class TestGenericServiceBridge : public ::testing::Test
{
protected:
  void SetUp() override
  {
    rclcpp::InitOptions context_options;

    context_1_ = std::make_shared<rclcpp::Context>();
    context_options.auto_initialize_logging(true).set_domain_id(kDomain1);
    context_1_->init(0, nullptr, context_options);

    context_2_ = std::make_shared<rclcpp::Context>();
    context_options.auto_initialize_logging(false).set_domain_id(kDomain2);
    context_2_->init(0, nullptr, context_options);

    rclcpp::NodeOptions node_options;

    node_options.context(context_1_);
    node_1_ = std::make_shared<rclcpp::Node>("node_1", node_options);

    node_options.context(context_2_);
    node_2_ = std::make_shared<rclcpp::Node>("node_2", node_options);
  }

  std::shared_ptr<rclcpp::Context> context_1_;
  std::shared_ptr<rclcpp::Context> context_2_;
  std::shared_ptr<rclcpp::Node> node_1_;
  std::shared_ptr<rclcpp::Node> node_2_;
};

static bool
poll_condition(std::function<bool()> condition, std::chrono::seconds timeout)
{
  auto start = std::chrono::system_clock::now();
  while (!condition() && (start + timeout > std::chrono::system_clock::now())) {
    std::this_thread::sleep_for(50ms);
  }
  return condition();
}

class ScopedAsyncSpinner
{
public:
  explicit ScopedAsyncSpinner(std::shared_ptr<rclcpp::Context> context)
  : executor_{get_executor_options_with_context(std::move(context))},
    thread_{[this, stop_token = promise_.get_future()] {
      executor_.spin_until_future_complete(stop_token);
    }}
  {}

  ~ScopedAsyncSpinner()
  {
    promise_.set_value();
    executor_.cancel();
    thread_.join();
  }

  rclcpp::Executor & get_executor() {return executor_;}

private:
  static rclcpp::ExecutorOptions
  get_executor_options_with_context(std::shared_ptr<rclcpp::Context> context)
  {
    rclcpp::ExecutorOptions ret;
    ret.context = std::move(context);
    return ret;
  }

  rclcpp::executors::SingleThreadedExecutor executor_;
  std::promise<void> promise_;
  std::thread thread_;
};

TEST_F(TestGenericServiceBridge, bridge_service_string_api)
{
  // Create a real service server on domain 1
  auto srv = node_1_->create_service<example_interfaces::srv::AddTwoInts>(
    "add_two_ints",
    [](
      const std::shared_ptr<example_interfaces::srv::AddTwoInts::Request> request,
      std::shared_ptr<example_interfaces::srv::AddTwoInts::Response> response)
    {
      response->sum = request->a + request->b;
    });

  // Create a typed client on domain 2
  auto cli = node_2_->create_client<example_interfaces::srv::AddTwoInts>("add_two_ints");

  // Bridge the service from domain 1 to domain 2 using string-based API
  domain_bridge::DomainBridge bridge;
  bridge.bridge_service(
    "add_two_ints",
    "example_interfaces/srv/AddTwoInts",
    kDomain1, kDomain2);

  // Wait for the bridge to create the proxy service
  EXPECT_TRUE(poll_condition([cli]() {return cli->service_is_ready();}, 5s));

  // Send a request through the bridge
  auto request = std::make_shared<example_interfaces::srv::AddTwoInts::Request>();
  request->a = 3;
  request->b = 5;
  auto future = cli->async_send_request(request);

  ScopedAsyncSpinner spinner{context_1_};
  spinner.get_executor().add_node(node_1_);
  spinner.get_executor().add_node(node_2_);
  bridge.add_to_executor(spinner.get_executor());

  // Verify response
  ASSERT_EQ(future.wait_for(5s), std::future_status::ready);
  auto response = future.get();
  EXPECT_EQ(response->sum, 8);
}
```

**Step 2: Add test to test/CMakeLists.txt**

Inside the `add_communication_tests()` function in `test/CMakeLists.txt`, add after the existing `test_domain_bridge_services` block:

```cmake
  ament_add_gmock(test_generic_service_bridge${target_suffix}
    domain_bridge/test_generic_service_bridge.cpp
    ENV ${rmw_implementation_env}
  )
  if(TARGET test_generic_service_bridge${target_suffix})
    ament_target_dependencies(test_generic_service_bridge${target_suffix}
      "rclcpp"
      "example_interfaces"
    )
    target_link_libraries(test_generic_service_bridge${target_suffix} ${PROJECT_NAME}_lib)
  endif()
```

Also add `find_package(example_interfaces REQUIRED)` at the top of `test/CMakeLists.txt`.

**Step 3: Build and run the test**

Run: `colcon build --packages-select domain_bridge`
Run: `colcon test --packages-select domain_bridge --ctest-args -R test_generic_service_bridge`
Expected: Test passes

**Step 4: Commit**

```bash
git add test/domain_bridge/test_generic_service_bridge.cpp \
        test/CMakeLists.txt
git commit -m "test: add end-to-end test for generic service bridge"
```

---

### Task 8: YAML Parsing Test for Services

Add a test for the YAML parsing of the `services:` section.

**Files:**
- Create: `test/domain_bridge/config/services.yaml`
- Modify: `test/domain_bridge/test_parse_domain_bridge_yaml_config.cpp`

**Step 1: Create test YAML config**

Create `test/domain_bridge/config/services.yaml`:

```yaml
name: test_service_bridge
from_domain: 0
to_domain: 1

services:
  add_two_ints:
    type: example_interfaces/srv/AddTwoInts
  my_service:
    type: test_msgs/srv/Empty
    from_domain: 2
    to_domain: 3
    remap: my_renamed_service
```

**Step 2: Add test cases**

In `test/domain_bridge/test_parse_domain_bridge_yaml_config.cpp`, add test cases for services parsing. Find the existing test class and add:

```cpp
TEST(TestParseDomainBridgeYamlConfig, services)
{
  auto config = domain_bridge::parse_domain_bridge_yaml_config("config/services.yaml");

  ASSERT_EQ(config.services.size(), 2u);

  // First service uses defaults
  auto & [sb1, opts1] = config.services[0];
  EXPECT_EQ(sb1.service_name, "add_two_ints");
  EXPECT_EQ(sb1.type_name, "example_interfaces/srv/AddTwoInts");
  EXPECT_EQ(sb1.from_domain_id, 0u);
  EXPECT_EQ(sb1.to_domain_id, 1u);
  EXPECT_TRUE(opts1.remap_name().empty());

  // Second service with overrides
  auto & [sb2, opts2] = config.services[1];
  EXPECT_EQ(sb2.service_name, "my_service");
  EXPECT_EQ(sb2.type_name, "test_msgs/srv/Empty");
  EXPECT_EQ(sb2.from_domain_id, 2u);
  EXPECT_EQ(sb2.to_domain_id, 3u);
  EXPECT_EQ(opts2.remap_name(), "my_renamed_service");
}
```

**Step 3: Build and run the test**

Run: `colcon build --packages-select domain_bridge`
Run: `colcon test --packages-select domain_bridge --ctest-args -R test_parse_domain_bridge_yaml_config`
Expected: Test passes

**Step 4: Commit**

```bash
git add test/domain_bridge/config/services.yaml \
        test/domain_bridge/test_parse_domain_bridge_yaml_config.cpp
git commit -m "test: add YAML parsing test for services section"
```

---

### Task 9: Update package.xml and Example Config

Add missing dependencies and update the example YAML config to show service bridging.

**Files:**
- Modify: `package.xml`
- Modify: `examples/example_bridge_config.yaml`

**Step 1: Update package.xml**

Add `rcpputils` and `rosidl_typesupport_introspection_cpp` as dependencies:

```xml
<depend>rcpputils</depend>
<depend>rosidl_typesupport_introspection_cpp</depend>
```

**Step 2: Update example YAML config**

Add a `services` section to `examples/example_bridge_config.yaml`:

```yaml
services:
  add_two_ints:
    type: example_interfaces/srv/AddTwoInts
```

**Step 3: Build and verify**

Run: `colcon build --packages-select domain_bridge`
Expected: Build succeeds

**Step 4: Commit**

```bash
git add package.xml examples/example_bridge_config.yaml
git commit -m "chore: add dependencies and update example config for service bridging"
```

---

### Task 10: Final Verification

Run the complete test suite to make sure nothing is broken.

**Step 1: Full build**

Run: `colcon build --packages-select domain_bridge`
Expected: Build succeeds with no warnings

**Step 2: Run all tests**

Run: `colcon test --packages-select domain_bridge`
Run: `colcon test-result --all --verbose`
Expected: All tests pass

**Step 3: Manual smoke test (optional)**

Run two terminals:
```bash
# Terminal 1: Start a service on domain 0
ROS_DOMAIN_ID=0 ros2 run demo_nodes_cpp add_two_ints_server

# Terminal 2: Start domain bridge
ros2 run domain_bridge domain_bridge -- --config path/to/config.yaml

# Terminal 3: Call the service from domain 1
ROS_DOMAIN_ID=1 ros2 service call /add_two_ints example_interfaces/srv/AddTwoInts "{a: 2, b: 3}"
```

Expected: Response with `sum: 5`
