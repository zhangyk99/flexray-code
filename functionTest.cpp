//
// Created by 79933 on 2025/12/15.
//

#include "src/tosunflexray.h"
#include <iostream>


int main(){
    std::vector<size_t> handleList;
    std::vector<int32_t> channelCount;
    TosunFlexRay::Init(handleList, channelCount);
    for(uint32_t i = 0; i < handleList.size(); i++)
        std::cout << "device handle" << i << ": " << handleList[i] << std::endl;
    if(handleList.size() < 2)
        return 0;

    ClusterConfig clusterConfig{};
    clusterConfig.ChannelAConnectedNode = 1;
    clusterConfig.ChannelBConnectedNode = 1;
    clusterConfig.ChannelASymbolTransmitted = 1;
    clusterConfig.ChannelBSymbolTransmitted = 1;
    clusterConfig.Speed = 10000000;
    clusterConfig.ColdStartAttempts = 10;
    clusterConfig.ActionPointOffset = 9;
    clusterConfig.DynamicSlotIdlePhase = 0;
    clusterConfig.MiniSlot = 10;
    clusterConfig.MiniSlotActionPointOffset = 3;
    clusterConfig.NetworkIdleTime = 44;
    clusterConfig.SampleClockPeriod = 0.012500;
    clusterConfig.StaticSlot = 61;
    clusterConfig.SymbolWindow = 0;
    clusterConfig.TimeSymbolStateTransmitter = 9;

    clusterConfig.WakeUpSymbolRXIdle = 59;
    clusterConfig.WakeUpSymbolRXLow = 55;
    clusterConfig.WakeUpSymbolRXWindow = 301;
    clusterConfig.WakeUpSymbolTXIdle = 180;
    clusterConfig.WakeUpSymbolTXLow = 60;

    clusterConfig.ListenNoise = 2;
    clusterConfig.MacroPerCycle = 5000;
    clusterConfig.MacroTick = 1.375000;
    clusterConfig.MaxInitializationError = 2.648000;
    clusterConfig.MaxWithoutClockCorrectionFatal = 14;
    clusterConfig.MaxWithoutClockCorrectionPassive = 10;
    clusterConfig.NetworkManagementVectorLength = 8;
    clusterConfig.NumberOfMiniSlots = 129;
    clusterConfig.NumberOfStaticSlots = 60;
    clusterConfig.OffsetCorrectionStart = 4981;
    clusterConfig.PayloadLengthStatic = 16;
    clusterConfig.SyncNodeMax = 8;
    clusterConfig.CasRXLowMax = 87;
    clusterConfig.CasRXLowMin = 29;
    clusterConfig.Bit = 0.100000;
    clusterConfig.Cycle = 5000;
    clusterConfig.ClusterDriftDamping = 2;
    clusterConfig.OffsetCorrectionMax = 3.150000;

    ECUConfig ecuConfig{};
    ecuConfig.StartUpSync = 5;
    ecuConfig.MaxDynamicPayloadLength = 127;
    ecuConfig.ClusterDriftDamping = 2;
    ecuConfig.DecodingCorrection = 48;
    ecuConfig.ListenTimeout = 401202;
    ecuConfig.MaxDrift = 601;
    ecuConfig.ExternOffsetCorrection = 0;
    ecuConfig.ExternRateCorrection = 0;
    ecuConfig.LatestTX = 124;
    ecuConfig.MicroPerCycle = 200000;
    ecuConfig.OffsetCorrectionOut = 378;
    ecuConfig.RateCorrectionOut = 601;
    ecuConfig.SamplesPerMicroTick = 2;
    ecuConfig.DelayCompensationA = 1;
    ecuConfig.DelayCompensationB = 1;
    ecuConfig.WakeUpPattern = 50;
    ecuConfig.AllowHaltDueToClock = true;
    ecuConfig.AllowPassiveToAction = 2;
    ecuConfig.AcceptedStartUpRange = 212;
    ecuConfig.MacroInitialOffsetA = 11;
    ecuConfig.MacroInitialOffsetB = 11;
    ecuConfig.MicroInitialOffsetA = 31;
    ecuConfig.MircoInitialOffsetB = 31;
    ecuConfig.SingleSlotEnabled = false;
    ecuConfig.MicroTick = 0.025000;

    uint16_t chn0Config[] = {3, 2, 18, 0x31, 32, 6, 18, 0x1, 32, 70, 1, 0x89, 32};
    uint16_t chn1Config[] = {3, 1, 1, 0x31, 32, 3, 4, 0x1, 32, 87, 1, 0x89, 32};

//    std::cout << static_cast<int>(clusterConfig.ChannelBConnectedNode) << std::endl;
//    std::cout << static_cast<int>(ecuConfig.MicroInitialOffsetA) << std::endl;

    auto chn0 = new TosunFlexRay::FlexRayChannel(handleList[0], 0, chn0Config, &clusterConfig, &ecuConfig);
    chn0->OpenChannel();
    auto chn1 = new TosunFlexRay::FlexRayChannel(handleList[1], 1, chn1Config, &clusterConfig, &ecuConfig);
    chn1->OpenChannel();

    uint8_t data[32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
    for(int i = 0; i < 10; i++){
        chn0->SetMessage(2, 18, 32, data, 100);
        chn0->SetMessage(6, 18, 32, data, 100);
        chn0->SetMessage(70, 1, 32, data, 100);
        chn1->SetMessage(1, 1, 32, data, 100);
        chn1->SetMessage(3, 4, 32, data, 100);
        chn1->SetMessage(87, 1, 32, data, 100);
#ifdef _WIN32
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
#else
        usleep(100000);
#endif
    }

    std::cout << std::endl;
    uint16_t slot = 87;
    uint8_t cycle = 0;
    uint8_t dataLength = 0;
    uint8_t data_r[254];
    for(uint32_t k = 0; k < 100; k++){
        chn0->GetMessage_(&slot, &cycle, &dataLength, data_r, 100);
        if(slot > 60 && slot < 129){
            std::cout << "slot: " << slot << " cycle: " << static_cast<int>(cycle) << " dataLength: " << static_cast<int>(dataLength) << std::endl;
            std::cout << "data: ";
            for (int i = 0; i < dataLength; i++)
                std::cout << static_cast<int>(data_r[i]) << ' ';
            std::cout << std::endl;
        }
    }

    delete chn0;
    delete chn1;
}