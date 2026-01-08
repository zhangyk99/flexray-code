//
// Created by 79933 on 2025/12/15.
//

#include <iostream>

#ifdef WIN32
#include "libTSCAN.h"
#include <thread>
#define FlexRayConfig _TLIBFlexray_controller_config
#define FlexRayFrameConfig _TLIBFlexRay
#define FlexRayTrigger _TLIBTrigger_def
#else
#include <unistd.h>
#include "TSCANDef.hpp"
#define FlexRayConfig TLibFlexray_controller_config
#define FlexRayFrameConfig TLibFlexRay
#define FlexRayTrigger TLibTrigger_def
#endif

//void
//#ifdef _WIN32
//__stdcall
//#endif
//flexray_rx_event(size_t *obj, const PLibFlexRay AData) {
//    if (AData->FDir != 0)return;
//    if (AData->FSlotId > 60 && AData->FFrameType == 0) {
//        printf("Received dynamic frames: chn:%d slotID:%d cycle:%d dataLen:%d\n", AData->FIdxChn, AData->FSlotId, AData->FCycleNumber, AData->FActualPayloadLength);
//        printf("data:");
//        for (int i = 0; i < AData->FActualPayloadLength; i++)
//            printf("%X ", AData->FData[i]);
//        printf("\n");
//    }
//}


int main(){
    size_t handle;
    size_t handle1;
    u32 ret;
    initialize_lib_tscan(true, true, true);

    u32 count = 0;
    char manufacturer[100];
    char product[100];
    char serial[100];
    char *pManufacturer = manufacturer;
    char *pProduct = product;
    char *pSerial = serial;

    char manufacturer1[100];
    char product1[100];
    char serial1[100];
    char *pManufacturer1 = manufacturer1;
    char *pProduct1 = product1;
    char *pSerial1 = serial1;

    tscan_scan_devices(&count);
    std::cout << "Number of hardware devices: " << count << std::endl;

    tscan_get_device_info(0, &pManufacturer, &pProduct, &pSerial);
    std::cout << pManufacturer << ", " << pProduct << ", " << pSerial << std::endl;
    std::cout << tscan_connect(pSerial, &handle) << std::endl;
    std::cout << "handle: " << handle << std::endl;
    tscan_get_device_info(1, &pManufacturer1, &pProduct1, &pSerial1);
    std::cout << pManufacturer1 << ", " << pProduct1 << ", " << pSerial1 << std::endl;
    std::cout << tscan_connect(pSerial1, &handle1) << std::endl;
    std::cout << "handle1: " << handle1 << std::endl;

    FlexRayConfig fr_config;
    fr_config.NETWORK_MANAGEMENT_VECTOR_LENGTH = 8;
    fr_config.PAYLOAD_LENGTH_STATIC = 16;
    fr_config.LATEST_TX = 124;

    fr_config.T_S_S_TRANSMITTER = 9;
    fr_config.CAS_RX_LOW_MAX = 87;
    fr_config.SPEED = 0;                                 //param speed
    fr_config.WAKE_UP_SYMBOL_RX_WINDOW = 301;
    fr_config.WAKE_UP_PATTERN = 50;                      //param channelIndex set 43/0 42/1
    fr_config.WAKE_UP_SYMBOL_RX_IDLE = 59;
    fr_config.WAKE_UP_SYMBOL_RX_LOW = 55;
    fr_config.WAKE_UP_SYMBOL_TX_IDLE = 180;
    fr_config.WAKE_UP_SYMBOL_TX_LOW = 60;

    fr_config.channelAConnectedNode = 1;
    fr_config.channelBConnectedNode = 1;
    fr_config.channelASymbolTransmitted = 1;
    fr_config.channelBSymbolTransmitted = 1;
    fr_config.ALLOW_HALT_DUE_TO_CLOCK = 1;
    fr_config.SINGLE_SLOT_ENABLED = 0;
    fr_config.wake_up_idx = 0;
    fr_config.ALLOW_PASSIVE_TO_ACTIVE = 2;
    fr_config.COLD_START_ATTEMPTS = 10;
    fr_config.synchFrameTransmitted = 1;
    fr_config.startupFrameTransmitted = 1;

    fr_config.LISTEN_TIMEOUT = 401202;
    fr_config.LISTEN_NOISE = 2;

    fr_config.MAX_WITHOUT_CLOCK_CORRECTION_PASSIVE = 10;
    fr_config.MAX_WITHOUT_CLOCK_CORRECTION_FATAL = 14;

    fr_config.MICRO_PER_CYCLE = 200000;
    fr_config.Macro_Per_Cycle = 5000;
    fr_config.SYNC_NODE_MAX = 8;

    fr_config.MICRO_INITIAL_OFFSET_A = 31;
    fr_config.MICRO_INITIAL_OFFSET_B = 31;
    fr_config.MACRO_INITIAL_OFFSET_A = 11;
    fr_config.MACRO_INITIAL_OFFSET_B = 11;
    fr_config.N_I_T = 44;
    fr_config.OFFSET_CORRECTION_START = 4981;
    fr_config.DELAY_COMPENSATION_A = 1;
    fr_config.DELAY_COMPENSATION_B = 1;
    fr_config.CLUSTER_DRIFT_DAMPING = 2;
    fr_config.DECODING_CORRECTION = 48;
    fr_config.ACCEPTED_STARTUP_RANGE = 212;

    fr_config.MAX_DRIFT = 601;
    fr_config.STATIC_SLOT = 61;
    fr_config.NUMBER_OF_STATIC_SLOTS = 60;
    fr_config.MINISLOT = 10;
    fr_config.NUMBER_OF_MINISLOTS = 129;
    fr_config.DYNAMIC_SLOT_IDLE_PHASE = 0;
    fr_config.ACTION_POINT_OFFSET = 9;
    fr_config.MINISLOT_ACTION_POINT_OFFSET = 3;
    fr_config.OFFSET_CORRECTION_OUT = 378;
    fr_config.RATE_CORRECTION_OUT = 601;
    fr_config.EXTERN_OFFSET_CORRECTION = 0;
    fr_config.EXTERN_RATE_CORRECTION = 0;
    fr_config.config_byte = 0x3c;
    fr_config.config_byte = fr_config.config_byte | 0x1 | 0x2; // lose param: config byte

    FlexRayTrigger fr_trigger[3];
    FlexRayTrigger fr_trigger1[3];

    //Cold start frame
    fr_trigger[0].frame_idx = 0;
    fr_trigger[0].slot_id = 2;
    fr_trigger[0].cycle_code = 18;
    fr_trigger[0].config_byte = 0X31;
    fr_trigger[0].rev = 0;
    //Static frames
    fr_trigger[1].frame_idx = 1;
    fr_trigger[1].slot_id = 6;
    fr_trigger[1].cycle_code = 18;
    fr_trigger[1].config_byte = 0X1;
    fr_trigger[1].rev = 0;
    //Dynamic frames
    fr_trigger[2].frame_idx = 2;
    fr_trigger[2].slot_id = 70;
    fr_trigger[2].cycle_code = 1;
    fr_trigger[2].config_byte = 0X89;
    fr_trigger[2].rev = 0;
    //Cold start frame
    fr_trigger1[0].frame_idx = 0;
    fr_trigger1[0].slot_id = 1;
    fr_trigger1[0].cycle_code = 1;
    fr_trigger1[0].config_byte = 0X31;
    fr_trigger1[0].rev = 0;
    //Static frames
    fr_trigger1[1].frame_idx = 1;
    fr_trigger1[1].slot_id = 3;
    fr_trigger1[1].cycle_code = 4;
    fr_trigger1[1].config_byte = 0X1;
    fr_trigger1[1].rev = 0;
    //Dynamic frames
    fr_trigger1[2].frame_idx = 2;
    fr_trigger1[2].slot_id = 87;
    fr_trigger1[2].cycle_code = 1;
    fr_trigger1[2].config_byte = 0X89;
    fr_trigger1[2].rev = 0;

    int FrameLengthArray[] = {32, 32, 32};
    int FrameLengthArray1[] = {32, 32, 32};

    ret = tsflexray_set_controller_frametrigger(handle, 0, &fr_config, FrameLengthArray, 3, fr_trigger, 3, 3000);
    std::cout << "set controller result: " << ret << std::endl;
    ret = tsflexray_set_controller_frametrigger(handle1, 1, &fr_config, FrameLengthArray1, 3, fr_trigger1, 3, 3000);
    std::cout << "set controller result: " << ret << std::endl;

//    ret = tsflexray_register_event_flexray_whandle(handle, &flexray_rx_event);
//    ret = tsflexray_register_event_flexray_whandle(handle1, &flexray_rx_event);

    ret = tsflexray_start_net(handle, 0, 3000);
    ret = tsflexray_start_net(handle1, 1, 3000);
    std::cout << ret << std::endl;

    FlexRayFrameConfig frFrame0;
    frFrame0.FIdxChn = 0;
    frFrame0.FSlotId = 2;
    frFrame0.FCycleNumber = 18;
    frFrame0.FChannelMask = 1;
    frFrame0.FDir = 0;
    frFrame0.FFrameType = 0;
    frFrame0.FActualPayloadLength = 32;
    for (int i = 0; i < frFrame0.FActualPayloadLength; i++)
        frFrame0.FData[i] = i;

    FlexRayFrameConfig frFrame1;
    frFrame1.FIdxChn = 1;
    frFrame1.FSlotId = 1;
    frFrame1.FCycleNumber = 1;
    frFrame1.FChannelMask = 1;
    frFrame1.FDir = 0;
    frFrame1.FFrameType = 0;
    frFrame1.FActualPayloadLength = 32;
    for (int i = 0; i < frFrame1.FActualPayloadLength; i++)
        frFrame1.FData[i] = i*2;

    FlexRayFrameConfig frFrame2;
    frFrame2.FIdxChn = 0;
    frFrame2.FSlotId = 6;
    frFrame2.FCycleNumber = 18;
    frFrame2.FChannelMask = 1;
    frFrame2.FDir = 0;
    frFrame2.FFrameType = 0;
    frFrame2.FActualPayloadLength = 32;
    for (int i = 0; i < frFrame2.FActualPayloadLength; i++)
        frFrame2.FData[i] = i;

    FlexRayFrameConfig frFrame3;
    frFrame3.FIdxChn = 1;
    frFrame3.FSlotId = 3;
    frFrame3.FCycleNumber = 4;
    frFrame3.FChannelMask = 1;
    frFrame3.FDir = 0;
    frFrame3.FFrameType = 0;
    frFrame3.FActualPayloadLength = 32;
    for (int i = 0; i < frFrame3.FActualPayloadLength; i++)
        frFrame3.FData[i] = i*2;

    FlexRayFrameConfig frFrame4;
    frFrame4.FIdxChn = 0;
    frFrame4.FSlotId = 70;
    frFrame4.FCycleNumber = 1;
    frFrame4.FChannelMask = 1;
    frFrame4.FDir = 0;
    frFrame4.FFrameType = 0;
    frFrame4.FActualPayloadLength = 32;
    for (int i = 0; i < frFrame4.FActualPayloadLength; i++)
        frFrame4.FData[i] = i;

    FlexRayFrameConfig frFrame5;
    frFrame5.FIdxChn = 1;
    frFrame5.FSlotId = 87;
    frFrame5.FCycleNumber = 1;
    frFrame5.FChannelMask = 1;
    frFrame5.FDir = 0;
    frFrame5.FFrameType = 0;
    frFrame5.FActualPayloadLength = 32;
    for (int i = 0; i < frFrame5.FActualPayloadLength; i++)
        frFrame5.FData[i] = i*2;

//    for(int i = 0; i < 10; i++) {
//        std::cout << "send round: " << i << std::endl;
//        tsflexray_transmit_sync(handle, &frFrame0, 100);
//        tsflexray_transmit_sync(handle, &frFrame2, 100);
//        tsflexray_transmit_sync(handle, &frFrame4, 100);
//        tsflexray_transmit_sync(handle1, &frFrame1, 100);
//        tsflexray_transmit_sync(handle1, &frFrame3, 100);
//        tsflexray_transmit_sync(handle1, &frFrame5, 100);
//
//        FlexRayFrameConfig frames[10];
//        int32_t size = 10;
//        tsfifo_receive_flexray_msgs(handle, frames, &size, 0, 1);
//        for(int j = 0; j < 10; j++){
//            printf("Received dynamic frames: chn:%d slotID:%d cycle:%d dataLen:%d\n", frames[j].FIdxChn, frames[j].FSlotId, frames[j].FCycleNumber, frames[j].FActualPayloadLength);
//            printf("data:");
//            for (int k = 0; k < frames[j].FActualPayloadLength; k++)
//                printf("%X ", frames[j].FData[k]);
//            printf("\n");
//        }
//#ifdef _WIN32
//        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//#else
//        sleep(1);
//#endif
//        std::cout << std::endl;
//    }

    for (int i = 0; i < 10; i++) {
        std::cout << "send round: " << i << "\n" << std::endl;
        std::cout << "send result: " << tsflexray_transmit_sync(handle, &frFrame0, 100) << std::endl;
        std::cout << "send result: " << tsflexray_transmit_sync(handle, &frFrame2, 100) << std::endl;
        std::cout << "send result: " << tsflexray_transmit_sync(handle, &frFrame4, 100) << std::endl;
        std::cout << "send result: " << tsflexray_transmit_sync(handle, &frFrame1, 100) << std::endl;
        std::cout << "send result: " << tsflexray_transmit_sync(handle, &frFrame3, 100) << std::endl;
        std::cout << "send result: " << tsflexray_transmit_sync(handle, &frFrame5, 100) << std::endl;

#ifdef _WIN32
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
#else
        usleep(100000);
#endif
    }

    FlexRayFrameConfig frFrame[100];
    int32_t size = 100;
    int32_t count_size = 0;

    int fifo_ret = tsfifo_receive_flexray_msgs(handle, frFrame, &size, 0, 1);
    std::cout << "FIFO ret is: " << fifo_ret << std::endl;

    for (int j = 0; j < size; j++) {
        if (frFrame[j].FSlotId > 60 && frFrame[j].FSlotId < 129 && frFrame[j].FFrameType == 0) {
            printf("FIFO Received dynamic frames: chn:%d slotID:%d cycle:%d dataLen:%d\n",
                   frFrame[j].FIdxChn, frFrame[j].FSlotId, frFrame[j].FCycleNumber, frFrame[j].FActualPayloadLength);
            printf("data:");
            for (int k = 0; k < frFrame[j].FActualPayloadLength; k++) {
                printf("%X ", frFrame[j].FData[k]);
            }
            printf("\n");
        }
    }

    tsflexray_stop_net(handle, 0, 1000);
    tsflexray_stop_net(handle1, 1, 1000);
#ifdef _WIN32
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
#else
    sleep(1);
#endif
    tscan_disconnect_by_handle(handle);
    tscan_disconnect_by_handle(handle1);
    finalize_lib_tscan();
}