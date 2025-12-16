#pragma once
#include <string>
#include <tuple>
#include <map>
#include <memory>
#include <functional>
#include <vector>
#include <typeindex>
#include <algorithm>


// Forward declaration
class EmptyDevice;

class DeviceRegistry {
public:
  struct RegistryEntry {
    std::function<std::unique_ptr<EmptyDevice>()> creator;
    std::vector<std::type_index> componentTypes;
    struct DeviceInfo {
      std::string deviceName = "Unset";
      uint16_t vid = 0;
      uint16_t pid = 0;
      std::string serialNumber = "Unset";
      std::string model = "Unset";
      std::string firmwareVersion = "Unset";
    } deviceInfo;
  };

  static std::map<std::string, RegistryEntry> &registry();

  static std::unique_ptr<EmptyDevice>
  createFromName(const std::string &deviceName);

  static bool isKnownDevice(const std::string &deviceName);

  static std::vector<std::string> getRegisteredDeviceNames();

  static DeviceRegistry::RegistryEntry::DeviceInfo
  getDeviceInfo(const std::string &deviceName);

  static std::vector<
      std::pair<std::string, DeviceRegistry::RegistryEntry::DeviceInfo>>
  getAllDeviceInfo();

  template <typename... QueryComponents>
  static std::vector<std::string> getRegisteredDeviceNamesWithComponents() {
    std::vector<std::string> out;
    std::vector<std::type_index> needed = {
        std::type_index(typeid(QueryComponents))...};
    for (const auto &[name, entry] : registry()) {
      bool allFound = true;
      for (const auto &need : needed) {
        if (std::find(entry.componentTypes.begin(), entry.componentTypes.end(),
                      need) == entry.componentTypes.end()) {
          allFound = false;
          break;
        }
      }
      if (allFound)
        out.push_back(name);
    }
    return out;
  }

  template <typename... QueryComponents>
  static std::vector<const RegistryEntry *>
  getRegisteredDevicesWithComponents() {
    std::vector<const RegistryEntry *> out;
    std::vector<std::type_index> needed = {
        std::type_index(typeid(QueryComponents))...};
    for (const auto &[name, entry] : registry()) {
      bool allFound = true;
      for (const auto &need : needed) {
        if (std::find(entry.componentTypes.begin(), entry.componentTypes.end(),
                      need) == entry.componentTypes.end()) {
          allFound = false;
          break;
        }
      }
      if (allFound)
        out.emplace_back(&entry);
    }
    return out;
  }
};