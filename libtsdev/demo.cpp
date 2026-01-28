//
// Created by 79933 on 2026/1/27.
//

#include "tsdev_host_lib_c.h"

#include <chrono>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <thread>
#include <iomanip>
#include <vector>
#include <queue>

#pragma warning(disable : 4996)

using namespace std;

// TSDev FlexRay 示例：演示如何通过 USB 配置 FlexRay 控制器并接收报文

// 设备句柄，由创建/打开设备时获得
static tsdev_handle_t handle_p;

// 简单日志回调：可以按需打开打印
static void _log_out_callback(tsdev_handle_t handle,
                              void* usr_arg,
                              tsdev_log_lvl_t lvl,
                              const char* msg) {
    (void)handle;
    (void)usr_arg;
    (void)lvl;
    // 如需查看 SDK 日志，可取消下一行注释
    // printf("%s", msg);
}

// FlexRay 命令应答回调
static tsdev_err_t tsdev_flexray_cmd_resp_event(
        tsdev_handle_t handle,
        void* usr_arg,
        tsdev_tsmsg_t msg,
        uint16_t rev,
        uint16_t fsize,
        const struct tsdev_THW_FlexrayResp* data) {
    (void)handle;
    (void)usr_arg;
    (void)msg;
    (void)rev;
    (void)fsize;

    printf("tsdev_THW_FlexrayResp action: %d\n", data->action);
    return tsdev_err_ok;
}

class ReceiveQueue{
public:
    std::vector<std::queue<tsdev_flexray_frame>> recQueue;
    ReceiveQueue(int chnSize){
        recQueue.resize(chnSize);
    }
};

// FlexRay 帧回调：打印部分关键信息
static tsdev_err_t tsdev_flexray_frame_event(tsdev_handle_t handle,
                                             void* usr_arg,
                                             tsdev_tsmsg_t msg,
                                             uint16_t rev,
                                             uint16_t fsize,
                                             const struct tsdev_flexray_frame* data) {
    auto rPtr = static_cast<ReceiveQueue*>(usr_arg);
    auto d = *data;
    rPtr->recQueue[data->FIdxChn].push(d);

//    (void)handle;
//    (void)usr_arg;
//    (void)msg;
//    (void)rev;
//    (void)fsize;
//
    printf("tsdev_flexray_frame FFrameType: %d\n", data->FFrameType);
    if (data->FFrameType == 0) {
        printf("  FTimeUs: %lld\n", static_cast<long long>(data->FTimeUs));
        printf("  FIdxChn: %d\n", data->FIdxChn);
        printf("  FDir: %d\n", data->FDir);
        printf("  FPayloadLength: %d\n", data->FPayloadLength);
        printf("  FActualPayloadLength: %d\n", data->FActualPayloadLength);
        printf("  FCycleNumber: %d\n", data->FCycleNumber);
        printf("  FSlotId: %d\n", data->FSlotId);
        printf("--------------------------------------------------------------\n");
    }
    return tsdev_err_ok;
}

// 全局命令结构，用于下发 FlexRay 配置命令
static struct tsdev_TPC_FlexrayCmdReq cmd = {0};

void tsdev_api_flexray_bridge_init(tsdev_handle_t handle) {
    tsdev_err_t err = tsdev_err_ok;

    struct tsdev_TPC_FlexrayCmdReq cmd_req = { 0 };
    struct tsdev_TPC_FlexrayConfig cfg_req = { 0 };

    memset(&cmd_req, 0, sizeof(cmd_req));
    memset(&cfg_req, 0, sizeof(cfg_req));

    /** stop flexrays */
    for (int i = 0; i < 2; ++i) {
        cmd_req.node_idx = i;
        cmd_req.action = 0;
        tsdev_api_flexray_cmdreq(handle, &cmd_req);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // VGM - CHN_0
    cfg_req.node_idx = 0;
    cfg_req.type = 0;
    cfg_req.controllerConfig.NETWORK_MANAGEMENT_VECTOR_LENGTH = 8;
    cfg_req.controllerConfig.PAYLOAD_LENGTH_STATIC = 16;
    cfg_req.controllerConfig.REVERS_16 = 0;
    cfg_req.controllerConfig.LATEST_TX = 124;
    // __ prtc1Control
    cfg_req.controllerConfig.T_S_S_TRANSMITTER = 9;
    cfg_req.controllerConfig.CAS_RX_LOW_MAX = 87;
    cfg_req.controllerConfig.SPEED = 0;
    cfg_req.controllerConfig.WAKE_UP_SYMBOL_RX_WINDOW = 301;
    cfg_req.controllerConfig.WAKE_UP_PATTERN = 43;
    // __ prtc2Control
    cfg_req.controllerConfig.WAKE_UP_SYMBOL_RX_IDLE = 59;
    cfg_req.controllerConfig.WAKE_UP_SYMBOL_RX_LOW = 55;
    cfg_req.controllerConfig.WAKE_UP_SYMBOL_TX_IDLE = 180;
    cfg_req.controllerConfig.WAKE_UP_SYMBOL_TX_LOW = 60;
    // __ succ1Config
    cfg_req.controllerConfig.channelAConnectedNode = 1;
    cfg_req.controllerConfig.channelBConnectedNode = 1;
    cfg_req.controllerConfig.channelASymbolTransmitted = 1;
    cfg_req.controllerConfig.channelBSymbolTransmitted = 1;
    cfg_req.controllerConfig.ALLOW_HALT_DUE_TO_CLOCK = 1;
    cfg_req.controllerConfig.SINGLE_SLOT_ENABLED = 0; // FALSE_0, TRUE_1
    cfg_req.controllerConfig.wake_up_idx = 0;
    cfg_req.controllerConfig.ALLOW_PASSIVE_TO_ACTIVE = 2;
    cfg_req.controllerConfig.COLD_START_ATTEMPTS = 10;
    cfg_req.controllerConfig.synchFrameTransmitted = 1;
    cfg_req.controllerConfig.startupFrameTransmitted = 1;
    // __ succ2Config
    cfg_req.controllerConfig.LISTEN_TIMEOUT = 401202;
    cfg_req.controllerConfig.LISTEN_NOISE = 2; // 2_16
    // __ succ3Config
    cfg_req.controllerConfig.MAX_WITHOUT_CLOCK_CORRECTION_PASSIVE = 10;
    cfg_req.controllerConfig.MAX_WITHOUT_CLOCK_CORRECTION_FATAL = 14;
    cfg_req.controllerConfig.REVERS0 = 0;
    // __ gtuConfig
    // __ gtu01Config
    cfg_req.controllerConfig.MICRO_PER_CYCLE = 200000;
    // __ gtu02Config
    cfg_req.controllerConfig.Macro_Per_Cycle = 5000;
    cfg_req.controllerConfig.SYNC_NODE_MAX = 8;
    cfg_req.controllerConfig.REVERS1 = 0;
    // __ gtu03Config
    cfg_req.controllerConfig.MICRO_INITIAL_OFFSET_A = 31;
    cfg_req.controllerConfig.MICRO_INITIAL_OFFSET_B = 31;
    cfg_req.controllerConfig.MACRO_INITIAL_OFFSET_A = 11;
    cfg_req.controllerConfig.MACRO_INITIAL_OFFSET_B = 11;
    // __ gtu04Config
    cfg_req.controllerConfig.N_I_T = 44;
    cfg_req.controllerConfig.OFFSET_CORRECTION_START = 4981;
    // __ gtu05Config
    cfg_req.controllerConfig.DELAY_COMPENSATION_A = 1;
    cfg_req.controllerConfig.DELAY_COMPENSATION_B = 1;
    cfg_req.controllerConfig.CLUSTER_DRIFT_DAMPING = 2;
    cfg_req.controllerConfig.DECODING_CORRECTION = 48;
    // __ gtu06Config
    cfg_req.controllerConfig.ACCEPTED_STARTUP_RANGE = 212;
    cfg_req.controllerConfig.MAX_DRIFT = 601;
    // __ gtu07Config
    cfg_req.controllerConfig.STATIC_SLOT = 61;
    cfg_req.controllerConfig.NUMBER_OF_STATIC_SLOTS = 60;
    // __ gtu08Config
    cfg_req.controllerConfig.MINISLOT = 10;
    cfg_req.controllerConfig.REVERS2 = 0;
    cfg_req.controllerConfig.NUMBER_OF_MINISLOTS = 129;
    // __ gtu09Config
    cfg_req.controllerConfig.DYNAMIC_SLOT_IDLE_PHASE = 0;
    cfg_req.controllerConfig.ACTION_POINT_OFFSET = 9;
    cfg_req.controllerConfig.MINISLOT_ACTION_POINT_OFFSET = 3;
    cfg_req.controllerConfig.REVERS3 = 0;
    // __ gtu10Config
    cfg_req.controllerConfig.OFFSET_CORRECTION_OUT = 378;
    cfg_req.controllerConfig.RATE_CORRECTION_OUT = 601;
    // __ gtu11Config
    cfg_req.controllerConfig.EXTERN_OFFSET_CORRECTION = 0;
    cfg_req.controllerConfig.EXTERN_RATE_CORRECTION = 0;

    cfg_req.controllerConfig.config_byte1 = 1;
    cfg_req.controllerConfig.config_byte = 191;

    // frame config
    cfg_req.frameConfig.frame_cnt = 1;
    cfg_req.frameConfig.trigger_cnt = 1;
    cfg_req.frameConfig.frame_length[0] = 32;
    cfg_req.frameConfig.slot_id[0] = 1;
    cfg_req.frameConfig.frame_length_idx[0] = 0;
    cfg_req.frameConfig.cycle_code[0] = 1;
    cfg_req.frameConfig.config_byte[0] = 49;

    tsdev_api_flexray_set_controller_frametrigger(handle, &cfg_req);

    // VDDM - CHN_1
    cfg_req.node_idx = 1;
    cfg_req.type = 0;
    cfg_req.controllerConfig.NETWORK_MANAGEMENT_VECTOR_LENGTH = 8;
    cfg_req.controllerConfig.PAYLOAD_LENGTH_STATIC = 16;
    cfg_req.controllerConfig.REVERS_16 = 0;
    cfg_req.controllerConfig.LATEST_TX = 124;
    // __ prtc1Control
    cfg_req.controllerConfig.T_S_S_TRANSMITTER = 9;
    cfg_req.controllerConfig.CAS_RX_LOW_MAX = 87;
    cfg_req.controllerConfig.SPEED = 0;
    cfg_req.controllerConfig.WAKE_UP_SYMBOL_RX_WINDOW = 301;
    cfg_req.controllerConfig.WAKE_UP_PATTERN = 42;
    // __ prtc2Control
    cfg_req.controllerConfig.WAKE_UP_SYMBOL_RX_IDLE = 59;
    cfg_req.controllerConfig.WAKE_UP_SYMBOL_RX_LOW = 55;
    cfg_req.controllerConfig.WAKE_UP_SYMBOL_TX_IDLE = 180;
    cfg_req.controllerConfig.WAKE_UP_SYMBOL_TX_LOW = 60;
    // __ succ1Config
    cfg_req.controllerConfig.channelAConnectedNode = 1;
    cfg_req.controllerConfig.channelBConnectedNode = 1;
    cfg_req.controllerConfig.channelASymbolTransmitted = 1;
    cfg_req.controllerConfig.channelBSymbolTransmitted = 1;
    cfg_req.controllerConfig.ALLOW_HALT_DUE_TO_CLOCK = 1;
    cfg_req.controllerConfig.SINGLE_SLOT_ENABLED = 0; // FALSE_0, TRUE_1
    cfg_req.controllerConfig.wake_up_idx = 0;
    cfg_req.controllerConfig.ALLOW_PASSIVE_TO_ACTIVE = 2;
    cfg_req.controllerConfig.COLD_START_ATTEMPTS = 10;
    cfg_req.controllerConfig.synchFrameTransmitted = 1;
    cfg_req.controllerConfig.startupFrameTransmitted = 1;
    // __ succ2Config
    cfg_req.controllerConfig.LISTEN_TIMEOUT = 401202;
    cfg_req.controllerConfig.LISTEN_NOISE = 2; // 2_16
    // __ succ3Config
    cfg_req.controllerConfig.MAX_WITHOUT_CLOCK_CORRECTION_PASSIVE = 10;
    cfg_req.controllerConfig.MAX_WITHOUT_CLOCK_CORRECTION_FATAL = 14;
    cfg_req.controllerConfig.REVERS0 = 0;
    // __ gtuConfig
    // __ gtu01Config
    cfg_req.controllerConfig.MICRO_PER_CYCLE = 200000;
    // __ gtu02Config
    cfg_req.controllerConfig.Macro_Per_Cycle = 5000;
    cfg_req.controllerConfig.SYNC_NODE_MAX = 8;
    cfg_req.controllerConfig.REVERS1 = 0;
    // __ gtu03Config
    cfg_req.controllerConfig.MICRO_INITIAL_OFFSET_A = 31;
    cfg_req.controllerConfig.MICRO_INITIAL_OFFSET_B = 31;
    cfg_req.controllerConfig.MACRO_INITIAL_OFFSET_A = 11;
    cfg_req.controllerConfig.MACRO_INITIAL_OFFSET_B = 11;
    // __ gtu04Config
    cfg_req.controllerConfig.N_I_T = 44;
    cfg_req.controllerConfig.OFFSET_CORRECTION_START = 4981;
    // __ gtu05Config
    cfg_req.controllerConfig.DELAY_COMPENSATION_A = 1;
    cfg_req.controllerConfig.DELAY_COMPENSATION_B = 1;
    cfg_req.controllerConfig.CLUSTER_DRIFT_DAMPING = 2;
    cfg_req.controllerConfig.DECODING_CORRECTION = 48;
    // __ gtu06Config
    cfg_req.controllerConfig.ACCEPTED_STARTUP_RANGE = 212;
    cfg_req.controllerConfig.MAX_DRIFT = 601;
    // __ gtu07Config
    cfg_req.controllerConfig.STATIC_SLOT = 61;
    cfg_req.controllerConfig.NUMBER_OF_STATIC_SLOTS = 60;
    // __ gtu08Config
    cfg_req.controllerConfig.MINISLOT = 10;
    cfg_req.controllerConfig.REVERS2 = 0;
    cfg_req.controllerConfig.NUMBER_OF_MINISLOTS = 129;
    // __ gtu09Config
    cfg_req.controllerConfig.DYNAMIC_SLOT_IDLE_PHASE = 0;
    cfg_req.controllerConfig.ACTION_POINT_OFFSET = 9;
    cfg_req.controllerConfig.MINISLOT_ACTION_POINT_OFFSET = 3;
    cfg_req.controllerConfig.REVERS3 = 0;
    // __ gtu10Config
    cfg_req.controllerConfig.OFFSET_CORRECTION_OUT = 378;
    cfg_req.controllerConfig.RATE_CORRECTION_OUT = 601;
    // __ gtu11Config
    cfg_req.controllerConfig.EXTERN_OFFSET_CORRECTION = 0;
    cfg_req.controllerConfig.EXTERN_RATE_CORRECTION = 0;

    cfg_req.controllerConfig.config_byte1 = 1;
    cfg_req.controllerConfig.config_byte = 191;

    // frame config
    cfg_req.frameConfig.frame_cnt = 1;
    cfg_req.frameConfig.trigger_cnt = 1;
    cfg_req.frameConfig.frame_length[0] = 32;
    cfg_req.frameConfig.slot_id[0] = 2;
    cfg_req.frameConfig.frame_length_idx[0] = 0;
    cfg_req.frameConfig.cycle_code[0] = 22;
    cfg_req.frameConfig.config_byte[0] = 49;

    tsdev_api_flexray_set_controller_frametrigger(handle, &cfg_req);

    // should start 2 nodes at same time
    for (int i = 0; i < 2; ++i) {
        tsdev_api_flexray_start_net(handle, i);
    }
}

void tsdev_api_flexray_demo(tsdev_handle_t handle) {
    auto rPtr = new ReceiveQueue(2);
    tsdev_api_flexray_cmd_register_event(handle, (void*)0x0123,
                                         tsdev_flexray_cmd_resp_event);
    tsdev_api_flexray_bridge_init(handle);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // flexray frame rx thread
    bool thread_exit = false;
    auto thread = std::thread([&]() {
        while (!thread_exit) {
            tsdev_api_flexray_receive(handle, static_cast<void*>(rPtr), 100000,
                                      tsdev_flexray_frame_event);
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    });
    // CLI
    do {
        printf("Input 'q' to exit:");
        char cmd;
        std::cin >> cmd;
        if (cmd == 'q') {
            thread_exit = true;
            if (thread.joinable()) {
                thread.join();
            }
            break;
        }
        else {
            printf("Error command.\n\r");
        }
    } while (true);
    // show result
    std::cout << "channel 0 receive frame num: " << rPtr->recQueue[0].size() << std::endl;
    std::cout << "channel 1 receive frame num: " << rPtr->recQueue[1].size() << std::endl;

    for(int i = 0; i < 2; i++){
        std::cout << "channel " << i << " data: " << std::endl;
        while(!rPtr->recQueue[i].empty()){
            auto it = rPtr->recQueue[i].front();
            rPtr->recQueue[i].pop();
            printf("tsdev_flexray_frame FFrameType: %d\n", it.FFrameType);
            if (it.FFrameType == 0) {
//                printf("  FTimeUs: %lld\n", static_cast<long long>(it.FTimeUs));
                printf("  FIdxChn: %d\n", it.FIdxChn);
//                printf("  FDir: %d\n", it.FDir);
//                printf("  FPayloadLength: %d\n", it.FPayloadLength);
//                printf("  FActualPayloadLength: %d\n", it.FActualPayloadLength);
                printf("  FCycleNumber: %d\n", it.FCycleNumber);
                printf("  FSlotId: %d\n", it.FSlotId);
                printf("--------------------------------------------------------------\n");
            }
        }
    }
}

static void flexray_usb_connect()
{
    struct tsdev_context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.dev_type = tsdev_devtype_all;
    ctx.com = tsdev_com_usb; // FlexRay 示例使用 USB 连接
    ctx.min_print_lvl = tsdev_log_trace;
    ctx.com_par.usb.config0 = 2;
    ctx.log_callback = [](tsdev_handle_t handle,
                          void* usr_arg,
                          tsdev_log_lvl_t lvl,
                          const char* msg) {
        (void)handle;
        (void)usr_arg;
        (void)lvl;
        printf("%s", msg);
    };
    tsdev_api_create(&ctx, &handle_p, NULL);
}

// FlexRay 示例入口：创建上下文 -> 扫描并打开设备 -> 配置并启动 FlexRay
void app_fr_test(void) {
    int connect_mode_index = 0;
    std::cout << "Please select connection mode index:" << endl;
    std::cout << "0)usb connect:" << endl;
    cin >> connect_mode_index;

    if (connect_mode_index == 0)
    {
        flexray_usb_connect();
    }
    else
    {
        return;
    }

    // 扫描设备
    uint16_t cnt = 0;
    tsdev_api_scan(handle_p, &cnt, 1000, 1);
    if (cnt == 0) {
        printf("no dev find, exit\r\n");
    }
    else {
        struct tsdev_info_t select_info;
        for (int i = 0; i < cnt; ++i)
        {
            struct tsdev_info_t info;
            tsdev_api_get_info(handle_p, i, &info);
            std::cout << "[#] device[" << int(i) << "] SN:";
            for (int i = 0; i < 8; ++i)
            {
                std::cout << std::hex << std::setw(2) << setfill('0') << static_cast<int>(info.uid[i]) << std::dec;
            }
            std::cout << endl;
        }
        std::cout << "select device by index:";
        int select_dev;
        std::cin >> select_dev;
        tsdev_api_get_info(handle_p, select_dev, &select_info);

        if (tsdev_err_ok != tsdev_api_open(handle_p, select_info.uid, NULL, 10000)) {
            printf("dev open fail, exit\r\n");
        } else {
            printf("dev open success\r\n");
            tsdev_api_flexray_demo(handle_p);
            tsdev_api_close(handle_p);
        }
    }

    tsdev_api_destory(handle_p);
}

int main(int argc, char** argv) {
    app_fr_test();
    return 0;
}