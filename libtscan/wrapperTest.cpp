//
// Created by 79933 on 2025/12/18.
//

#include "src/flexraywrapper.cpp"

void wTest(bool isTure) {
    //findChannelTest
    char str[2000];
    int32_t chnCount;
    fr_find_all_channels(str, &chnCount);
    std::cout << "channel count: " << chnCount << std::endl;
    std::cout << str;

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



    //sendAndReceiveTest
    void *channel0;
    void *channel1;
    uint16_t chnConfig0[] = {3, 2, 18, 0x31, 32, 6, 18, 0x1, 32, 70, 1, 0x89, 32};
    uint16_t chnConfig1[] = {3, 1, 1, 0x31, 32, 3, 4, 0x1, 32, 87, 1, 0x89, 32};
    if (isTure) {
        fr_set_channel_config(&channel0, 0, 0, chnConfig0, &clusterConfig, &ecuConfig);
        fr_open_channel(channel0);
        fr_set_channel_config(&channel1, 0, 1, chnConfig1, &clusterConfig, &ecuConfig);
        fr_open_channel(channel1);
    }

    uint8_t data[32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
    for (uint32_t i = 0; i < 10; i++) {
        std::cout << "send round " << i << std::endl;
        fr_set_message(channel0, 2, 18, 32, data, 100);
        fr_set_message(channel0, 6, 18, 32, data, 100);
        fr_set_message(channel0, 70, 1, 32, data, 100);
        fr_set_message(channel1, 1, 1, 32, data, 100);
        fr_set_message(channel1, 3, 4, 32, data, 100);
        fr_set_message(channel1, 87, 1, 32, data, 100);
        data[0]++;
#ifdef _WIN32
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
#else
        usleep(100000);
#endif
    }

    uint16_t slot = 0;
    uint8_t cycle = 0;
    uint8_t dataLength = 0;
    uint8_t data_r[254];

    for (uint32_t i = 0; i < 1000; i++) {
        fr_get_message(channel0, &slot, &cycle, &dataLength, data_r, 100);
        if (slot > 60 && slot < 129) {
            std::cout << "chnIndex: " << static_cast<int>(cycle) << std::endl;
//            std::cout << "slot: " << slot << " cycle: " << static_cast<int>(cycle) << " dataLength: " << static_cast<int>(dataLength) << std::endl;
//            std::cout << "data: ";
//            for (int j = 0; j < dataLength; j++)
//                std::cout << static_cast<int>(data_r[j]) << ' ';
            std::cout << std::endl;
        }
    }
    for (uint32_t i = 0; i < 1000; i++) {
        fr_get_message(channel1, &slot, &cycle, &dataLength, data_r, 100);
        if (slot > 60 && slot < 129) {
            std::cout << "chnIndex: " << static_cast<int>(cycle) << std::endl;
//            std::cout << "slot: " << slot << " cycle: " << static_cast<int>(cycle) << " dataLength: " << static_cast<int>(dataLength) << std::endl;
//            std::cout << "data: ";
//            for (int j = 0; j < dataLength; j++)
//                std::cout << static_cast<int>(data_r[j]) << ' ';
            std::cout << std::endl;
        }
    }


    fr_close_channel(channel0);
    fr_close_channel(channel1);
}

int main() {
    char str[2000];
    int32_t chnCount;
    fr_find_all_channels(str, &chnCount);
    std::cout << "channel count: " << chnCount << std::endl;
    std::cout << str;

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

    void *channel0;
    void *channel1;
    uint16_t chnConfig0[] = {3, 2, 18, 0x31, 32, 6, 18, 0x1, 32, 70, 1, 0x89, 32};
    uint16_t chnConfig1[] = {3, 1, 1, 0x31, 32, 3, 4, 0x1, 32, 87, 1, 0x89, 32};
    fr_set_channel_config(&channel0, 0, 0, chnConfig0, &clusterConfig, &ecuConfig);
    fr_set_channel_config(&channel1, 0, 1, chnConfig1, &clusterConfig, &ecuConfig);
    fr_open_channel(channel0);
    fr_open_channel(channel1);

    uint8_t data[32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
    for (uint32_t i = 0; i < 10; i++) {
        std::cout << "send round " << i << std::endl;
        fr_set_message(channel0, 2, 18, 32, data, 100);
        fr_set_message(channel0, 6, 18, 32, data, 100);
        fr_set_message(channel0, 70, 1, 32, data, 100);
        fr_set_message(channel1, 1, 1, 32, data, 100);
        fr_set_message(channel1, 3, 4, 32, data, 100);
        fr_set_message(channel1, 87, 1, 32, data, 100);
        data[0]++;
#ifdef _WIN32
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
#else
        usleep(100000);
#endif
    }

    uint16_t slot = 0;
    uint8_t cycle = 0;
    uint8_t dataLength = 0;
    uint8_t data_r[254];

    std::cout << "receive chn0 data: " << std::endl;
    for (uint32_t i = 0; i < 1000; i++) {
        fr_get_message(channel0, &slot, &cycle, &dataLength, data_r, 100);
        if (slot > 60 && slot < 129) {
            std::cout << "slot: " << slot << " cycle: " << static_cast<int>(cycle) << " dataLength: " << static_cast<int>(dataLength) << std::endl;
            std::cout << "data: ";
            for (int j = 0; j < dataLength; j++)
                std::cout << static_cast<int>(data_r[j]) << ' ';
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
    std::cout << "receive chn1 data: " << std::endl;
    for (uint32_t i = 0; i < 1000; i++) {
        fr_get_message(channel1, &slot, &cycle, &dataLength, data_r, 100);
        if (slot > 60 && slot < 129) {
            std::cout << "slot: " << slot << " cycle: " << static_cast<int>(cycle) << " dataLength: " << static_cast<int>(dataLength) << std::endl;
            std::cout << "data: ";
            for (int j = 0; j < dataLength; j++)
                std::cout << static_cast<int>(data_r[j]) << ' ';
            std::cout << std::endl;
        }
    }

    fr_close_channel(channel0);
    fr_close_channel(channel1);
}