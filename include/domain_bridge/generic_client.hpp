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

#ifndef DOMAIN_BRIDGE__GENERIC_CLIENT_HPP_
#define DOMAIN_BRIDGE__GENERIC_CLIENT_HPP_

#include <functional>
#include <future>
#include <map>
#include <memory>
#include <mutex>
#include <string>

#include "rcl/client.h"
#include "rcpputils/shared_library.hpp"
#include "rclcpp/client.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"

#include "domain_bridge/visibility_control.hpp"

namespace domain_bridge
{

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

  DOMAIN_BRIDGE_PUBLIC
  std::shared_ptr<void> create_response() override;

  DOMAIN_BRIDGE_PUBLIC
  std::shared_ptr<rmw_request_id_t> create_request_header() override;

  DOMAIN_BRIDGE_PUBLIC
  void handle_response(
    std::shared_ptr<rmw_request_id_t> request_header,
    std::shared_ptr<void> response) override;

  DOMAIN_BRIDGE_PUBLIC
  SharedRequest create_request();

  DOMAIN_BRIDGE_PUBLIC
  SharedFuture async_send_request(SharedRequest request, ResponseCallback callback);

  DOMAIN_BRIDGE_PUBLIC
  SharedFuture async_send_request(SharedRequest request);

private:
  struct PendingRequest
  {
    std::promise<SharedResponse> promise;
    SharedFuture future;
    ResponseCallback callback;
  };

  std::shared_ptr<rcpputils::SharedLibrary> ts_lib_;
  std::shared_ptr<rcpputils::SharedLibrary> intro_ts_lib_;
  const rosidl_typesupport_introspection_cpp::MessageMembers * request_members_;
  const rosidl_typesupport_introspection_cpp::MessageMembers * response_members_;
  std::mutex pending_requests_mutex_;
  std::map<int64_t, PendingRequest> pending_requests_;
};

}  // namespace domain_bridge

#endif  // DOMAIN_BRIDGE__GENERIC_CLIENT_HPP_
