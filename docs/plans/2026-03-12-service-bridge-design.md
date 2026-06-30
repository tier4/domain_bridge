# Service Bridge Design for domain_bridge (Humble)

## Summary

Add dynamic (YAML-configurable) service bridging to domain_bridge by backporting
`GenericService` and `GenericClient` from ROS 2 Rolling's rclcpp into the
domain_bridge package itself, adapted for Humble's rclcpp API.

## Background

### Current State

- domain_bridge supports **topic bridging** via YAML configuration using
  `rclcpp::GenericPublisher` / `rclcpp::GenericSubscription` (type-agnostic).
- **Service bridging** exists only as a compile-time template API:
  `bridge_service<ServiceT>(name, from, to)`. It cannot be configured from YAML
  because the service type must be known at compile time.
- The upstream design doc (`doc/design.md`) acknowledges this limitation.

### Generic Service Support Across ROS 2 Distributions

| Feature | Humble (16.x) | Jazzy (28.x) | Rolling (30.x) |
|---------|---------------|--------------|-----------------|
| `GenericClient` | No | Yes | Yes |
| `GenericClient` callback | No | No | Yes |
| `GenericService` | No | No | Yes |
| `get_service_typesupport_handle()` | No | Yes | Yes |

### Alternative Approaches Considered

1. **Code generation** (like PR #82 / ros1_bridge): Generate template
   instantiations for all service types at build time. Proven pattern but
   increases build time significantly and requires rebuilds when new types are
   added.
2. **Selective code generation**: User specifies which service types to include.
   Lighter but less convenient.
3. **GenericService backport** (chosen): Port GenericService/GenericClient from
   Rolling into domain_bridge. Same UX as topic bridging, no rebuilds needed.

## Design

### Architecture Overview

```
[YAML config]
  | parse
  v
[DomainBridgeConfig] -- service entries added
  |
  v
[DomainBridge::bridge_service(name, type, from, to)]  -- new string-based overload
  |
  v
[service_typesupport_helpers]  -- dynamic typesupport loading
  |
  v
[GenericService] (to_domain)  <-->  [GenericClient] (from_domain)
  |                                    |
  receive request -> forward via client -> return response
```

### New Files (6 files)

| File | Purpose |
|------|---------|
| `include/domain_bridge/generic_service.hpp` | GenericService class definition |
| `include/domain_bridge/generic_client.hpp` | GenericClient class definition |
| `include/domain_bridge/service_typesupport_helpers.hpp` | Dynamic service typesupport loading helpers |
| `src/domain_bridge/generic_service.cpp` | GenericService implementation |
| `src/domain_bridge/generic_client.cpp` | GenericClient implementation |
| `src/domain_bridge/service_typesupport_helpers.cpp` | Typesupport helpers implementation |

### Modified Files (4 files)

| File | Change |
|------|--------|
| `include/domain_bridge/domain_bridge.hpp` | Add `bridge_service(name, type, from, to)` string-based overload |
| `src/domain_bridge/domain_bridge.cpp` | Implement string-based service bridge + config-driven service bridge creation |
| `src/domain_bridge/parse_domain_bridge_yaml_config.cpp` | Parse `services:` section |
| `CMakeLists.txt` | Add new source files |

### Component Details

#### service_typesupport_helpers

Dynamically loads service typesupport from a type name string. This is the
foundation for GenericService/GenericClient.

```cpp
namespace domain_bridge {

// Load a service typesupport shared library
std::shared_ptr<rcpputils::SharedLibrary>
get_service_typesupport_library(
  const std::string & type,
  const std::string & typesupport_id);

// Get the typesupport handle from a loaded library
const rosidl_service_type_support_t *
get_service_typesupport_handle(
  const std::string & type,
  const std::string & typesupport_id,
  rcpputils::SharedLibrary & library);

// Get introspection ServiceMembers for request/response member access
const rosidl_typesupport_introspection_cpp::ServiceMembers *
get_service_members(const std::string & type);

}  // namespace domain_bridge
```

Symbol name construction logic:
- Input: `"example_interfaces/srv/AddTwoInts"`
- Decompose: package=`example_interfaces`, name=`AddTwoInts`
- Symbol: `rosidl_typesupport_cpp__get_service_type_support_handle__example_interfaces__srv__AddTwoInts`

#### GenericService

Inherits from `rclcpp::ServiceBase`. Creates a service server from a type name
string at runtime.

Key design points:
- Constructor takes `service_type` string instead of template parameter
- Uses introspection typesupport to allocate/deallocate request/response objects
- Virtual method signatures match Humble's `ServiceBase` (by-value parameters)
- Callback receives `shared_ptr<GenericService>` for deferred response support

#### GenericClient

Inherits from `rclcpp::ClientBase`. Creates a service client from a type name
string at runtime.

Key design points:
- Constructor takes `service_type` string instead of template parameter
- `async_send_request()` supports callback for response handling
- Manages pending requests with `std::map<int64_t, ...>`
- Virtual method signatures match Humble's `ClientBase` (by-value parameters)

### Humble-Specific Adaptations from Rolling

| Item | Rolling | Humble Adaptation |
|------|---------|-------------------|
| `ServiceBase::handle_request()` | `const shared_ptr<...> &` | `shared_ptr<...>` (by-value) |
| `ClientBase::handle_response()` | `const shared_ptr<...> &` | `shared_ptr<...>` (by-value) |
| `get_service_typesupport_handle()` | In rclcpp | Self-implemented in domain_bridge |
| `service_ts->request_typesupport` | Direct field access | Via introspection `ServiceMembers::request_members_` |
| `TRACETOOLS_TRACEPOINT` | Rolling macro | `TRACEPOINT` (Humble macro) |
| `InvalidServiceTypeError` | In rclcpp | Self-defined or `std::runtime_error` |
| Service event introspection | Supported | Not supported (rcl API absent) |

### YAML Configuration Format

```yaml
name: my_bridge
from_domain: 0
to_domain: 1

topics:
  chatter:
    type: std_msgs/msg/String

services:
  add_two_ints:
    type: example_interfaces/srv/AddTwoInts     # required
    from_domain: 2                               # optional (defaults to top-level)
    to_domain: 3                                 # optional (defaults to top-level)
    remap: add_two_numbers                       # optional (service name remapping)
```

Design decisions:
- `bidirectional` is **not supported** — services are inherently directional
- `qos` is **not supported** — ROS 2 services use fixed default QoS
- `reversed` is unnecessary — use `from_domain`/`to_domain` directly

### DomainBridgeConfig Extension

```cpp
struct ServiceBridge {
  std::string service_name;
  std::string service_type;  // "package/srv/Type" format
  size_t from_domain_id;
  size_t to_domain_id;
};

// Added to DomainBridgeConfig
std::vector<std::pair<ServiceBridge, ServiceBridgeOptions>> services;
```

### Error Handling

| Error Case | Handling |
|------------|----------|
| Missing `type` in YAML | Exception at parse time |
| Invalid type name (package not found) | Exception at typesupport load + log warning, skip service |
| Service server not available on `from_domain` | Wait indefinitely via `WaitForGraphEvents` (same as topics) |
| Service call timeout | Handled by rclcpp future mechanism |
| Service server disappears | Log warning. Auto-reconnect is out of scope for initial version |

### Logging

```
[INFO] Service bridge created: 'add_two_ints' [example_interfaces/srv/AddTwoInts] domain 0 -> domain 1
[WARN] Service type 'unknown_pkg/srv/Unknown' not found, skipping service 'my_service'
[INFO] Waiting for service server 'add_two_ints' on domain 0...
[INFO] Service server 'add_two_ints' found on domain 0, creating bridge service on domain 1
```

## Testing

| Test | Content | Priority |
|------|---------|----------|
| `test_service_typesupport_helpers` | Verify dynamic typesupport loading and error handling | Required |
| `test_service_bridge_end_to_end` | Full service bridge test with `AddTwoInts` across domains | Required |
| `test_parse_service_yaml_config` | YAML `services:` section parsing validation | Nice to have |

Test service type: `example_interfaces/srv/AddTwoInts` (already in test dependencies).

## Scope and Constraints

### Out of Scope

- Action bridging
- Service auto-remove (topic bridge has this feature)
- Service message compression
- Service event introspection (Humble rcl lacks the API)

### Future Extension Points

Code comments will indicate where Jazzy/Rolling adaptations should be made:

```cpp
// TODO(jazzy): Use rclcpp::GenericClient on Jazzy+ (available natively)
// TODO(jazzy): Adapt to const reference signatures for ServiceBase/ClientBase
// TODO(rolling): Use rclcpp::GenericService and GenericClient directly,
//                removing the backported implementations from domain_bridge
// TODO(future): Add auto_remove support for service bridges
```
