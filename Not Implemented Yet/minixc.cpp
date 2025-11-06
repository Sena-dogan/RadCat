#include "minixc.hpp"
#include "App.hpp"
#include "DataIOHandler.hpp"
using namespace std;
using namespace MinixUtilities;
using namespace Utilities;



#pragma region Minix Write Methods

void MinixController::setVoltage(double voltage) {
    unsigned char tx[100]; FT_STATUS status; DWORD ret_bytes;

    if (hvOn) 
    {
        if ((voltage > HighVoltageMax) || (voltage < HighVoltageMin))
        {
        printf("voltage outside limits\n");
        return;
        }
    }

    if (voltage * currentuA > SafeWattageMW) { Debug.Error("SetVoltage: Power too high, not setting voltage."); return; }

    voltagekV = voltage;
    voltage += 0.00001;
    voltage /= HighVoltageConversionFactor; // convert from hv to adc volts

    int iVolts = (int)(voltage / VRef * DAC_ADC_Scale);
    unsigned char bhi, blo;

    //01111111 11110000
    bhi = (iVolts & 0x0ff0) >> 4;
    blo = (iVolts & 0x0f) << 4;

    //take DACS low and clock 2 bytes of data into the DAC
    int pos=0;
    tx[pos++] = 0x80;//Setup MPSSE Low byte I/O lines
    CLEAR(LowByteHiLowState, DACS);//take DACS low
    //CLEAR(LowByteHiLowState, CLKSTATE);//take CLock low
    SET(LowByteHiLowState, CLKSTATE);//take CLock low
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;
    
    tx[pos++] = CLK_FN;   //Clock out 2 bytes on positive edge, MSB first
    tx[pos++] = 0x02;//LengthL 0=1byte, 1=2bytes
    tx[pos++] = 0x00;//LengthH  
    tx[pos++] = DACA;
    tx[pos++] = bhi;
    tx[pos++] = blo;

    //take DACS back high
    tx[pos++] = 0x80;//Setup MPSSE Low byte I/O lines
    SET(LowByteHiLowState, DACS);//take DACS high
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    status = FT_Write(ftHandle, tx, pos, &ret_bytes);
    if(status != FT_OK) { Debug.Error("SetVoltage: Error writing to device."); HVSetErr = true; return; }
}

void MinixController::setCurrent(double voltage) {
    unsigned char tx[100]; FT_STATUS status; DWORD ret_bytes;

    if (hvOn)
    {
        if ((voltage > CurrentMax) || (voltage < CurrentMin))
        {
        printf("current outside limits\n");
        return;
        }
    }

    if (voltagekV * voltage > SafeWattageMW)  { Debug.Error("SetCurrent: Power too high, not setting current."); return; }

    currentuA = voltage;
    voltage += 0.00001;
    voltage /= CurrentConversionFactor;  // convert from hv to adc volts

    int iVolts = (int)(voltage / VRef * DAC_ADC_Scale);
    unsigned char bhi, blo;

    //01111111 11110000
    bhi = ((iVolts & 0x0ff0) >> 4);
    blo = ((iVolts & 0x0f) << 4);

    //take DACS low and clock 2 bytes of data into the DAC
    int pos=0;
    tx[pos++] = 0x80;//Setup MPSSE Low byte I/O lines
    CLEAR(LowByteHiLowState, DACS);//take DACS low
    //CLEAR(LowByteHiLowState, CLKSTATE);//take CLock low
    SET(LowByteHiLowState, CLKSTATE);//take CLock low
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;
    
    tx[pos++] = CLK_FN;   //Clock out 2 bytes on positive edge, MSB first
    tx[pos++] = 0x02;//LengthL 0=1byte, 1=2bytes
    tx[pos++] = 0x00;//LengthH  
    tx[pos++] = DACB;
    tx[pos++] = bhi;
    tx[pos++] = blo;

    //take DACS back high
    tx[pos++] = 0x80;//Setup MPSSE Low byte I/O lines
    SET(LowByteHiLowState, DACS);//take DACS high
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    status = FT_Write(ftHandle, tx, pos, &ret_bytes);
    if(status != FT_OK){ Debug.Error("SetCurrent: Error writing to device."); CurrentSetErr = true; }
}

void MinixController::setHVOnOff(bool on) {
    unsigned char tx[6]; DWORD ret_bytes; int pos=0; FT_STATUS status;

    if(on){
        tx[pos++] = 0x80;//Setup MPSSE Low byte I/O lines
        SET(LowByteHiLowState, CTRL_HV_EN_A); //HVEN A enabled
        SET(LowByteHiLowState, CTRL_HV_EN_B); //HVEN B enabled
        tx[pos++] = LowByteHiLowState;
        tx[pos++] = OUTPUTMODE;
    }else{
        tx[pos++] = 0x80;//Setup MPSSE Low byte I/O lines
        CLEAR(LowByteHiLowState, CTRL_HV_EN_A); //HVEN A disabled
        CLEAR(LowByteHiLowState, CTRL_HV_EN_B); //HVEN B disabled
        tx[pos++] = LowByteHiLowState;
        tx[pos++] = OUTPUTMODE;
        setVoltage(0.0);
        sleepMs(1000);
        setCurrent(0.0);
        sleepMs(1000);
    }

    status = FT_Write(ftHandle, tx, pos, &ret_bytes);
    if(status != FT_OK) { Debug.Error("SetHVOnOff: Error writing to device."); HVOffErr = true; return; }
    hvOn = on;
}

#pragma endregion




#pragma region MinixSetup and close Methods

bool MinixController::disconnectMiniX(){
    if constexpr (minixDebug) Debug.Log("Disconnecting from MiniX...");
    if(closeDevice()) return true;
    return false;
}

bool MinixController::closeDevice() {
    if (isDeviceOpen) {
        purgeBuffers(ftHandle);
        FT_ResetDevice(ftHandle); // Reset device before closing
        if (FT_Close(ftHandle) != FT_OK) {Debug.Error("Error closing device.");return false;}
        isDeviceOpen = false;
        if constexpr (minixDebug) Debug.Log("Device closed.");
    } else {
        if constexpr (minixDebug) Debug.Log("No device is open.");
    }
    return true;
}

int MinixController::getConnectedDevices() {
    if (isDeviceOpen) {Debug.Warn("Device is already open. Not fatal but a bug. Probably from UI.");return -1;}
    FT_STATUS status; DWORD numDevs;

    // Get the number of connected devices
    status = FT_CreateDeviceInfoList(&numDevs);
    if (status != FT_OK) {Debug.Error("Error getting device list: " , status);return -1;}

    if constexpr (minixDebug) {
        Debug.Log("Number of connected devices: " , numDevs);
        for (DWORD i = 0; i < numDevs; i++) {
            FT_DEVICE_LIST_INFO_NODE devInfo;
            status = FT_GetDeviceInfoDetail(i, &devInfo.Flags, &devInfo.Type, &devInfo.ID,
                                            &devInfo.LocId, devInfo.SerialNumber,
                                            devInfo.Description, &devInfo.ftHandle);
            if (status == FT_OK) {
                Debug.Log("Device:" + std::to_string(i) + " - " + devInfo.Description);
                Debug.Log("Serial Number:", devInfo.SerialNumber);
                Debug.Log("ID:", std::to_string(devInfo.ID));
                Debug.Log("Location ID:", std::to_string(devInfo.LocId));
            } else {
                Debug.Error("Error getting device info for device " + std::to_string(i) + ": " , status);
                return -1;
            }
        }
    }

    return numDevs;
}


bool MinixController::initializeMiniX() {
    SetParametersToDefault();

    if (!setupTemperatureSensor()) { Debug.Error("Failed to setup temperature sensor."); return false; }

    if (!setupClockDivisor()) { Debug.Error("Failed to set clock divisor."); return false; }

    //setVoltage(0.0);
    //sleepMs(200);
    //setCurrent(0.0);

    if constexpr (minixDebug) Debug.Log("MiniX initialization completed successfully.");
    return true;
}

#pragma endregion


#pragma region Interface Methods


bool MinixController::safetyChecks(){
    if(!connectionChecks()){ Debug.Error("Connection checks failed.");
        return false;
    }

    return true;
}
#pragma endregion


