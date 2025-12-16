#include "DeviceRegistry.hpp"
#include "DeviceCore.hpp"

// Definition of the static registry map
std::map<std::string, DeviceRegistry::RegistryEntry> &
DeviceRegistry::registry() {
  static std::map<std::string, RegistryEntry> instance;
  return instance;
}

std::unique_ptr<EmptyDevice>
DeviceRegistry::createFromName(const std::string &deviceName) {
  auto it = registry().find(deviceName);
  if (it != registry().end()) {
    return it->second.creator();
  }
  return nullptr;
}

bool DeviceRegistry::isKnownDevice(const std::string &deviceName) {
  return registry().find(deviceName) != registry().end();
}

std::vector<std::string> DeviceRegistry::getRegisteredDeviceNames() {
  std::vector<std::string> names;
  for (const auto &[name, entry] : registry()) {
    names.push_back(name);
  }
  return names;
}

DeviceRegistry::RegistryEntry::DeviceInfo
DeviceRegistry::getDeviceInfo(const std::string &deviceName) {
  auto it = registry().find(deviceName);
  if (it != registry().end()) {
    return it->second.deviceInfo;
  }
  return {};
}

std::vector<std::pair<std::string, DeviceRegistry::RegistryEntry::DeviceInfo>>
DeviceRegistry::getAllDeviceInfo() {
  std::vector<std::pair<std::string, DeviceRegistry::RegistryEntry::DeviceInfo>>
      result;
  for (const auto &[name, entry] : registry()) {
    result.emplace_back(name, entry.deviceInfo);
  }
  return result;
}
