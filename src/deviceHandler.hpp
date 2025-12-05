#pragma once
#include <vector>
#include <memory>
#include "deviceCore.hpp"


class DeviceHandler {
public:
    std::vector<std::unique_ptr<EmptyDevice>> activeDevices;
    std::vector<const DeviceRegistry::RegistryEntry*> foundDevices;
    
    const static bool debug = false;

    void ftdiScan();
    void libUsbScan();
    void deviceScan();
    void deviceLogicUpdate();
    
    
};