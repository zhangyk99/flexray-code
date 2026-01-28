//
// Created by 79933 on 2026/1/27.
//

#include "flexraywrapper.h"
#include "tosunflexray.h"

#define CHECK_HANDLE(x) if(x == nullptr) return FLEXRAY_ERROR_NULLPTR;
#define WRAPPER_CAST(x) static_cast<TosunFlexRay::FlexRayChannel*>(x)

bool init = false;

int32_t fr_find_all_channels(char *str, int32_t *chnCount){

}

int32_t fr_get_error_code(char *error, int32_t eventNum) {
    std::string errStr;
    switch(eventNum){
        case FLEXRAY_OK:
            errStr = "no error";
            break;
        case FLEXRAY_ERROR_NULLPTR:
            errStr = "point null";
            break;
        case FLEXRAY_ERROR_GET_DEVICE_COUNT:
            errStr = "get device count failed";
            break;
        case FLEXRAY_ERROR_NO_DEVICE:
            errStr = "there is no device";
            break;
        case FLEXRAY_ERROR_GET_DEVICE_INFO:
            errStr = "get device info failed";
            break;
        case FLEXRAY_ERROR_DEVICE_CONNECT:
            errStr = "device connect failed";
            break;
        case FLEXRAY_ERROR_GET_FLEXARY_CHANNEL:
            errStr = "get flexray channel channel failed";
            break;
        case FLEXRAY_ERROR_DEVICE_DISCONNECT:
            errStr = "device disconnect failed";
            break;
        case FLEXRAY_ERROR_CHANNEL_CONFIG:
            errStr = "set channel config failed";
            break;
        case FLEXRAY_ERROR_CHANNEL_RECEIVE_EVENT:
            errStr = "set channel receive event failed";
            break;
        case FLEXRAY_ERROR_CHANNEL_START_NET:
            errStr = "channel start net failed";
            break;
        case FLEXRAY_ERROR_CHANNEL_STOP_NET:
            errStr = "channel stop net failed";
            break;
        case FLEXRAY_ERROR_SET_MESSAGE:
            errStr = "set message failed";
            break;
        case FLEXRAY_ERROR_GET_MESSAGE:
            errStr = "get message failed";
            break;
        default:
            errStr = "other error";
    }
    errStr.copy(error, errStr.length());
    return FLEXRAY_OK;
}

int32_t fr_set_channel_config(void **channel, int32_t deviceIndex, int32_t chnIndex, uint16_t *chnConfig, ClusterConfig *clusterConfig, ECUConfig *ecuConfig){

}

int32_t fr_open_channel(void *channel){

}

int32_t fr_close_channel(void *channel){

}

int32_t fr_set_message(void *channel, uint16_t slot, uint8_t dataLength, uint8_t *data, int32_t timeout){

}

int32_t fr_set_messages(void *channel, uint16_t item, uint16_t *slot, uint8_t *dataLength, uint8_t *data, int32_t timeout){

}

int32_t fr_get_message(void *channel, uint16_t *slot, uint8_t *cycle, uint8_t *dataLength, uint8_t *data, int32_t timeout){

}

int32_t fr_get_messages(void *channel, uint16_t *item, uint16_t *slot, uint8_t *cycle, uint8_t *dataLength, uint8_t *data, int32_t timeout){

}