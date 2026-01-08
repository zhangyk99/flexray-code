//
// Created by 79933 on 2025/12/15.
//

#ifndef TSFLEXRAY_TOSUNFLEXRAY_H
#define TSFLEXRAY_TOSUNFLEXRAY_H

#include <cstdint>
#include <vector>
#include <map>
#include <queue>
#include <mutex>
#include <atomic>
#include <cstring>
#include <algorithm>

#include "flexrayerror.h"
#include "flexraylog.h"
#include "flexrayconfig.h"

#ifdef WIN32

#include "libTSCAN.h"
#include <thread>

#define FlexRayConfig _TLIBFlexray_controller_config
#define FlexRayFrameConfig _TLIBFlexRay
#define FlexRayTrigger _TLIBTrigger_def
#else

#include <unistd.h>
#include <cstring>
#include "TSCANDef.hpp"

#define FlexRayConfig TLibFlexray_controller_config
#define FlexRayFrameConfig TLibFlexRay
#define FlexRayTrigger TLibTrigger_def
#endif

namespace TosunFlexRay {
    struct Frame {
        uint16_t slot;
        uint8_t cycle;
        uint8_t dataLength;
        uint8_t data[254];
    };

    bool isLog = true;
    std::mutex mtx;
    std::map<uint16_t, std::queue<Frame>> receiveMap;
    uint32_t queueSize = 100;

    int32_t Init(std::vector<size_t> &handleList, std::vector<int32_t> &channelCount) {
        initialize_lib_tscan(true, true, true);

        uint32_t deviceCount = 0;
        if (tscan_scan_devices(&deviceCount)) {
            Log(isLog, "error", "Init", "tscan_scan_devices");
            return FLEXRAY_ERROR_GET_DEVICE_COUNT;
        }
        if (deviceCount == 0) {
            Log(isLog, "info", "Init", "there is no device");
            return FLEXRAY_ERROR_NO_DEVICE;
        }
        char manufacturer[100];
        char product[100];
        char serial[100];
        char *pManufacturer = manufacturer;
        char *pProduct = product;
        char *pSerial = serial;
        size_t handle;
        for (uint32_t i = 0; i < deviceCount; i++) {
            if (tscan_get_device_info(i, &pManufacturer, &pProduct, &pSerial)) {
                Log(isLog, "error", "Init", "tscan_get_device_info", "device number is:", i);
                return FLEXRAY_ERROR_GET_DEVICE_INFO;
            }
            if (tscan_connect(pSerial, &handle)) {
                Log(isLog, "errpr", "Init", "tscan_connect", "device number is:", i);
                return FLEXRAY_ERROR_DEVICE_CONNECT;
            }
            int32_t count = 0;
            if (tscan_get_flexray_channel_count(handle, &count)) {
                Log(isLog, "error", "Init", "tscan_get_flexray_channel_count");
                return FLEXRAY_ERROR_GET_FLEXARY_CHANNEL;
            }
            if (count) {
                channelCount.emplace_back(count);
                handleList.push_back(handle);
            } else {
                if (tscan_disconnect_by_handle(handle)) {
                    Log(isLog, "error", "Init", "tscan_disconnect_by_handle");
                    return FLEXRAY_ERROR_DEVICE_DISCONNECT;
                }
            }
        }
        Log(isLog, "info", "Init", "finished");
        return FLEXRAY_OK;
    }

    int32_t Finalize() {
        if (tscan_disconnect_all_devices()) {
            Log(isLog, "error", "finalize", "tscan_disconnect_all_devices");
            return FLEXRAY_ERROR_DEVICE_DISCONNECT;
        }
        finalize_lib_tscan();
        Log(isLog, "info", "Finalize", "finished");
        return FLEXRAY_OK;
    }

    void SetBufferSize(uint32_t size) {
        queueSize = size;
    }

    class FlexRayChannel {
        size_t handle;
        int32_t chnIndex;
        int32_t errorCode;

//        static void
//#ifdef _WIN32
//        __stdcall
//#endif
//        flexray_rx_event(size_t *obj, const PLibFlexRay AData) {
//            std::lock_guard<std::mutex> lock(mtx);
//            if (AData->FDir != 0)return;
//            uint8_t data[254]{};
//            if (AData->FSlotId > 60 && AData->FFrameType == 0) {
//                Frame frame;
//                frame.slot = AData->FSlotId;
//                frame.cycle = AData->FCycleNumber;
//                frame.dataLength = AData->FActualPayloadLength;
//                memcpy(frame.data, AData->FData, AData->FActualPayloadLength);
//                receiveMap[AData->FSlotId].push(frame);
//                if (receiveMap[AData->FSlotId].size() > queueSize)
//                    receiveMap[AData->FSlotId].pop();
////                std::cout << "handle: " << *obj << std::endl;
////                printf("Received dynamic frames: chn:%d slotID:%d cycle:%d dataLen:%d\n", AData->FIdxChn, AData->FSlotId, AData->FCycleNumber, AData->FActualPayloadLength);
////                printf("data:");
////                for (int i = 0; i < AData->FActualPayloadLength; i++)
////                    printf("%X ", AData->FData[i]);
////                printf("\n");
//            }
//        }

    public:
        FlexRayChannel(size_t handle, int32_t chnIndex, uint16_t *config, ClusterConfig *clusterConfig, ECUConfig *ecuConfig) : handle(handle), chnIndex(chnIndex) {
            FlexRayConfig f;
            //clusterConfig
            f.channelAConnectedNode = clusterConfig->ChannelAConnectedNode;
            f.channelBConnectedNode = clusterConfig->ChannelBConnectedNode;
            f.channelASymbolTransmitted = clusterConfig->ChannelASymbolTransmitted;
            f.channelBSymbolTransmitted = clusterConfig->ChannelBSymbolTransmitted;
            if (clusterConfig->Speed == 10000000) {
                f.SPEED = 0;
            } else if(clusterConfig->Speed == 5000000) {
                f.SPEED = 1;
            } else if(clusterConfig->Speed == 2500000) {
                f.SPEED = 2;
            }
            f.COLD_START_ATTEMPTS = clusterConfig->ColdStartAttempts;
            f.ACTION_POINT_OFFSET = clusterConfig->ActionPointOffset;
            f.DYNAMIC_SLOT_IDLE_PHASE = clusterConfig->DynamicSlotIdlePhase;
            f.MINISLOT = clusterConfig->MiniSlot;
            f.MINISLOT_ACTION_POINT_OFFSET = clusterConfig->MiniSlotActionPointOffset;
            f.N_I_T = clusterConfig->NetworkIdleTime;
//            f.SampleClockPeriod = 0.012500;
            f.STATIC_SLOT = clusterConfig->StaticSlot;
//            f.SymbolWindow = 0;
            f.T_S_S_TRANSMITTER = clusterConfig->TimeSymbolStateTransmitter;

            f.WAKE_UP_SYMBOL_RX_IDLE = clusterConfig->WakeUpSymbolRXIdle;
            f.WAKE_UP_SYMBOL_RX_LOW = clusterConfig->WakeUpSymbolRXLow;
            f.WAKE_UP_SYMBOL_RX_WINDOW = clusterConfig->WakeUpSymbolRXWindow;
            f.WAKE_UP_SYMBOL_TX_IDLE = clusterConfig->WakeUpSymbolTXIdle;
            f.WAKE_UP_SYMBOL_TX_LOW = clusterConfig->WakeUpSymbolTXLow;

            f.LISTEN_NOISE = clusterConfig->ListenNoise;
            f.Macro_Per_Cycle = clusterConfig->MacroPerCycle;
//            f.MacroTick = 1.375000;
//            f.MaxInitializationError = 2.648000;
            f.MAX_WITHOUT_CLOCK_CORRECTION_FATAL = clusterConfig->MaxWithoutClockCorrectionFatal;
            f.MAX_WITHOUT_CLOCK_CORRECTION_PASSIVE = clusterConfig->MaxWithoutClockCorrectionPassive;
            f.NETWORK_MANAGEMENT_VECTOR_LENGTH = clusterConfig->NetworkManagementVectorLength;
            f.NUMBER_OF_MINISLOTS = clusterConfig->NumberOfMiniSlots;
            f.NUMBER_OF_STATIC_SLOTS = clusterConfig->NumberOfStaticSlots;
            f.OFFSET_CORRECTION_START = clusterConfig->OffsetCorrectionStart;
            f.PAYLOAD_LENGTH_STATIC = clusterConfig->PayloadLengthStatic;
            f.SYNC_NODE_MAX = clusterConfig->SyncNodeMax;
            f.CAS_RX_LOW_MAX = clusterConfig->CasRXLowMax;
//            f.CasRXLowMin = 29;
//            f.Bit = 0.100000;
//            f.Cycle = 5000;
            f.CLUSTER_DRIFT_DAMPING = clusterConfig->ClusterDriftDamping;
//            f.OffsetCorrectionMax = 3.150000;

            //ECUConfig
            f.wake_up_idx = 0;                        //set target value
            f.synchFrameTransmitted = 1;              //set target value
            f.startupFrameTransmitted = 1;            //set target value
//            f.MaxDynamicPayloadLength = 127;
//            f.ClusterDriftDamping = ecuConfig->ClusterDriftDamping;
            f.DECODING_CORRECTION = ecuConfig->DecodingCorrection;
            f.LISTEN_TIMEOUT = ecuConfig->ListenTimeout;
            f.MAX_DRIFT = ecuConfig->MaxDrift;
            f.EXTERN_OFFSET_CORRECTION = ecuConfig->ExternOffsetCorrection;
            f.EXTERN_RATE_CORRECTION = ecuConfig->ExternRateCorrection;
            f.LATEST_TX = ecuConfig->LatestTX;
            f.MICRO_PER_CYCLE = ecuConfig->MicroPerCycle;
            f.OFFSET_CORRECTION_OUT = ecuConfig->OffsetCorrectionOut;
            f.RATE_CORRECTION_OUT = ecuConfig->RateCorrectionOut;
//            f.SamplesPerMicroTick = 2;
            f.DELAY_COMPENSATION_A = ecuConfig->DelayCompensationA;
            f.DELAY_COMPENSATION_B = ecuConfig->DelayCompensationB;
            f.WAKE_UP_PATTERN = ecuConfig->WakeUpPattern;
            f.ALLOW_HALT_DUE_TO_CLOCK = ecuConfig->AllowHaltDueToClock;
            f.ALLOW_PASSIVE_TO_ACTIVE = ecuConfig->AllowPassiveToAction;
            f.ACCEPTED_STARTUP_RANGE = ecuConfig->AcceptedStartUpRange;
            f.MACRO_INITIAL_OFFSET_A = ecuConfig->MacroInitialOffsetA;
            f.MACRO_INITIAL_OFFSET_B = ecuConfig->MacroInitialOffsetB;
            f.MICRO_INITIAL_OFFSET_A = ecuConfig->MicroInitialOffsetA;
            f.MICRO_INITIAL_OFFSET_B = ecuConfig->MircoInitialOffsetB;
            f.SINGLE_SLOT_ENABLED = ecuConfig->SingleSlotEnabled;
//            f.MicroTick = 0.025000;

            f.config_byte = 0x3c;                                   //set target value
            f.config_byte = f.config_byte | 0x1 | 0x2;     //set target value

//            FlexRayConfig fr_config;
//
//            fr_config.NetworkManagementVectorLength = 8;
//            fr_config.PayloadLengthStatic = 16;
//            fr_config.LatestTX = 124;
//
//            fr_config.TimeSymbolStateTransmitter = 9;
//            fr_config.CasRXLowMax = 87;
//            fr_config.Speed = 0;                                 //param speed
//            fr_config.WakeUpSymbolRXWindow = 301;
//            fr_config.WakeUpPattern = 50;                      //param channelIndex set 43/0 42/1
//            fr_config.WakeUpSymbolRXIdle = 59;
//            fr_config.WakeUpSymbolRXLow = 55;
//            fr_config.WakeUpSymbolTXIdle = 180;
//            fr_config.WakeUpSymbolTXLow = 60;
//
//            fr_config.ChannelAConnectedNode = 1;
//            fr_config.ChannelBConnectedNode = 0;
//            fr_config.ChannelASymbolTransmitted = 1;
//            fr_config.ChannelBSymbolTransmitted = 1;
//            fr_config.AllowHaltDueToClock = 1;
//            fr_config.SingleSlotEnabled = 0;
//            fr_config.wake_up_idx = 0;                                                     //no set
//            fr_config.AllowPassiveToAction = 2;
//            fr_config.ColdStartAttempts = 10;
//            fr_config.synchFrameTransmitted = 1;                                           //no set
//            fr_config.startupFrameTransmitted = 1;                                         //no set
//
//            fr_config.ListenTimeout = 401202;
//            fr_config.ListenNoise = 2;
//
//            fr_config.MaxWithoutClockCorrectionPassive = 10;
//            fr_config.MaxWithoutClockCorrectionFatal = 14;
//
//            fr_config.MicroPerCycle = 200000;
//            fr_config.MacroPerCycle = 5000;
//            fr_config.SyncNodeMax = 8;
//
//            fr_config.MicroInitialOffsetA = 31;
//            fr_config.MircoInitialOffsetB = 31;
//            fr_config.MacroInitialOffsetA = 11;
//            fr_config.MacroInitialOffsetB = 11;
//            fr_config.N_I_T = 44;
//            fr_config.OffsetCorrectionStart = 4981;
//            fr_config.DelayCompensationA = 1;
//            fr_config.DelayCompensationB = 1;
//            fr_config.ClusterDriftDamping = 2;
//            fr_config.DecodingCorrection = 48;
//            fr_config.AcceptedStartUpRange = 212;
//
//            fr_config.MaxDrift = 601;
//            fr_config.StaticSlot = 61;
//            fr_config.NumberOfStaticSlots = 60;
//            fr_config.MiniSlot = 10;
//            fr_config.NumberOfMiniSlots = 129;
//            fr_config.DynamicSlotIdlePhase = 0;
//            fr_config.ActionPointOffset = 9;
//            fr_config.MiniSlotActionPointOffset = 3;
//            fr_config.OffsetCorrectionOut = 378;
//            fr_config.RateCorrectionOut = 601;
//            fr_config.ExternOffsetCorrection = 0;
//            fr_config.ExternRateCorrection = 0;
//            fr_config.config_byte = 0x3c;
//            fr_config.config_byte = (byte) (fr_config.config_byte | 0x1 | 0x2); // lose param: config byte

            std::vector<FlexRayTrigger> fr_trigger;
            std::vector<int> FrameLengthArray;

            for (uint16_t i = 0; i < config[0]; i++) {
                FlexRayTrigger ft;
                ft.frame_idx = i;
                ft.slot_id = config[i * 4 + 1];
                ft.cycle_code = config[i * 4 + 2];
                ft.config_byte = config[i * 4 + 3];
                ft.rev = 0;
                FrameLengthArray.emplace_back(config[i * 4 + 4]);
                fr_trigger.push_back(ft);
            }

            if (tsflexray_set_controller_frametrigger(handle, chnIndex, &f, FrameLengthArray.data(), static_cast<int>(FrameLengthArray.size()), fr_trigger.data(), static_cast<int>(fr_trigger.size()), 3000)) {
                Log(isLog, "error", "FlexRayChannel", "tsflexray_set_controller_frametrigger", "channel index: ", chnIndex);
                errorCode = FLEXRAY_ERROR_CHANNEL_CONFIG;
                return;
            }
//            if (isRead) {
//                if (tsflexray_register_event_flexray_whandle(handle, &flexray_rx_event)) {
//                    Log(isLog, "error", "FlexRayChannel", "tsflexray_register_event_flexray_whandle", "channel index: ", chnIndex);
//                    errorCode = FLEXRAY_ERROR_CHANNEL_RECEIVE_EVENT;
//                    return;
//                }
//            }
            Log(isLog, "info", "FlexRayChannel", "finished");
        }

        ~FlexRayChannel() {
            if (tscan_disconnect_by_handle(handle)) {
                Log(isLog, "error", "FlexRayChannelChannel", "tscan_disconnect_by_handle", "channel index: ", chnIndex);
                errorCode = FLEXRAY_ERROR_DEVICE_DISCONNECT;
                return;
            }
            Log(isLog, "info", "~FlexRayChannel", "finished");
        }

        int32_t OpenChannel() {
            if (tsflexray_start_net(handle, chnIndex, 3000)) {
                Log(isLog, "error", "fr_open_channel", "tsflexray_start_net", "channel index: ", chnIndex);
                return FLEXRAY_ERROR_CHANNEL_START_NET;
            }
            Log(isLog, "info", "fr_open_channel", "finished");
            return FLEXRAY_OK;
        }

        int32_t CloseChannel() {
            if (tsflexray_stop_net(handle, chnIndex, 3000)) {
                Log(isLog, "error", "fr_close_channel", "tsflexray_stop_net", "channel index: ", chnIndex);
                return FLEXRAY_ERROR_CHANNEL_STOP_NET;
            }
            Log(isLog, "info", "fr_close_channel", "finished");
            return FLEXRAY_OK;
        }

        int32_t SetMessage(uint16_t slot, uint8_t cycle, uint8_t dataLength, uint8_t *data, int32_t timeout) {
            FlexRayFrameConfig frFrame;
            frFrame.FIdxChn = chnIndex;
            frFrame.FSlotId = slot;
            frFrame.FCycleNumber = cycle;
            frFrame.FChannelMask = 1;
            frFrame.FDir = 0;
            frFrame.FFrameType = 0;
            frFrame.FActualPayloadLength = dataLength;
            memcpy(frFrame.FData, data, dataLength);
            if (tsflexray_transmit_sync(handle, &frFrame, timeout)) {
                Log(isLog, "error", "fr_set_message", "tsflexray_transmit_sync", "slot:", slot);
                return FLEXRAY_ERROR_SET_MESSAGE;
            }
//            Log(isLog, "info", "fr_set_message", "slot:", slot, "finished");
            return FLEXRAY_OK;
        }

        int32_t GetMessage_(uint16_t *slot, uint8_t *cycle, uint8_t *dataLength, uint8_t *data, int32_t timeout) {
            //lock
            std::lock_guard<std::mutex> lock(mtx);
            FlexRayFrameConfig frFrame[1];
            int32_t size = 1;

            if(tsfifo_receive_flexray_msgs(handle, frFrame, &size, chnIndex, 1))
                return FLEXRAY_OK;
            *slot = frFrame[0].FSlotId;
            *cycle = frFrame[0].FCycleNumber;
            *dataLength = frFrame[0].FActualPayloadLength;
            memcpy(data, frFrame[0].FData, *dataLength);

//            auto &queue = receiveMap[*slot];
//            if (queue.empty())
//                return FLEXRAY_ERROR_GET_MESSAGE;
//            auto frame = queue.front();
//            queue.pop();
//            *slot = frame.slot;
//            *cycle = frame.cycle;
//            *dataLength = frame.dataLength;
//            memcpy(data, frame.data, *dataLength);

//            Log(isLog, "info", "GetMessage", "slot:", *slot, "finished");
            return FLEXRAY_OK;
        }

        int32_t GetMessages(uint16_t *item, uint16_t *slot, uint8_t *cycle, uint8_t *dataLength, uint8_t *data, int32_t timeout) {
            //read 100 times
            //lock
            std::lock_guard<std::mutex> lock(mtx);
            FlexRayFrameConfig frFrames[100];
            int32_t size = 100;
            if (tsfifo_receive_flexray_msgs(handle, frFrames, &size, chnIndex, 1))
                return FLEXRAY_ERROR_GET_MESSAGE;
            uint32_t pos = 0;
            for (int i = 0; i < 100; i++) {
                *item += 1;
                cycle[i] = frFrames[i].FCycleNumber;
                dataLength[i] = frFrames[i].FActualPayloadLength;
                memcpy(data + pos, frFrames[i].FData, dataLength[i]);
                pos += dataLength[i];
            }
            Log(isLog, "info", "fr_get_messages", "item:", *item, "finished");
            return FLEXRAY_OK;
        }

        int32_t GetErrorCode() const { return errorCode; }
    };
}

#endif //TSFLEXRAY_TOSUNFLEXRAY_H
