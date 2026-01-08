//
// Created by 79933 on 2025/12/15.
//

#include "flexraywrapper.h"
#include "tosunflexray.h"


#define CHECK_HANDLE(x) if(x == nullptr) return FLEXRAY_ERROR_NULLPTR;
#define WRAPPER_CAST(x) static_cast<TosunFlexRay::FlexRayChannel *>(x)

bool isInit = false;
std::mutex mtx;
std::vector<size_t> handleList;
std::vector<int32_t> channelCount;
uint32_t channelOpenCount = 0;


int32_t fr_find_all_channels(char *str, int32_t *chnCount) {
    CHECK_HANDLE(str);
    CHECK_HANDLE(chnCount);

    std::lock_guard<std::mutex> lock(mtx);

    if (!isInit) {
        TosunFlexRay::Init(handleList, channelCount);
        isInit = true;
    }
    *chnCount = 0;
    std::string chnStr;
    for (uint32_t i = 0; i < channelCount.size(); i++) {
        for (uint32_t j = 0; j < channelCount[i]; j++) {
            std::string split = i == 0 && j == 0 ? "" : "\t";
            *chnCount += 1;
            chnStr += split + "Tosun" + std::to_string(i) + " channel" + std::to_string(j);
        }
    }
    auto len = chnStr.length() > 1999 ? 1999 : chnStr.length();
    chnStr.copy(str, chnStr.size());
    str[len] = '\0';
    return 0;
}

int32_t fr_get_error_code(char *error, int32_t *eventNum) {
    std::string errStr;
    switch(*eventNum){
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

int32_t fr_set_channel_config(void **channel, int32_t deviceIndex, int32_t chnIndex, uint16_t *chnConfig, ClusterConfig *clusterConfig, ECUConfig *ecuConfig) {
    if (!isInit) {
        TosunFlexRay::Init(handleList, channelCount);
        isInit = true;
    }
    *channel = new TosunFlexRay::FlexRayChannel(handleList[deviceIndex], chnIndex, chnConfig, clusterConfig, ecuConfig);
    auto res = WRAPPER_CAST(*channel)->GetErrorCode();
    if (!res)
        channelOpenCount++;
    return res;
}

int32_t fr_open_channel(void *channel) {
    std::lock_guard<std::mutex> lock(mtx);
    return WRAPPER_CAST(channel)->OpenChannel();
}

int32_t fr_close_channel(void *channel) {
    std::lock_guard<std::mutex> lock(mtx);
    auto res = WRAPPER_CAST(channel)->CloseChannel();
    delete WRAPPER_CAST(channel);
    if (!res)
        channelOpenCount--;
    else
        return res;
    if (!channelOpenCount)
        res = TosunFlexRay::Finalize();
    return res;
}

int32_t fr_set_message(void *channel, uint16_t slot, uint8_t cycle, uint8_t dataLength, uint8_t *data, int32_t timeout) {
    std::lock_guard<std::mutex> lock(mtx);
    return WRAPPER_CAST(channel)->SetMessage(slot, cycle, dataLength, data, timeout);
}

int32_t fr_set_messages(void *channel, uint16_t item, uint16_t *slot, uint8_t *cycle, uint8_t *dataLength, uint8_t *data, int32_t timeout) {
    std::lock_guard<std::mutex> lock(mtx);
    uint64_t dataPos = 0;
    uint8_t data_[256]{};
    for (uint16_t i = 0; i < item; i++) {
        memcpy(data, data_, dataLength[i]);
//        std::copy(data + dataPos, data +dataPos + dataLength[i], data_);
        dataPos += dataLength[i];
        auto res = WRAPPER_CAST(channel)->SetMessage(slot[i], cycle[i], dataLength[i], data, timeout);
        if (res)
            return res;
    }
    return 0;
}

int32_t fr_get_message(void *channel, uint16_t *slot, uint8_t *cycle, uint8_t *dataLength, uint8_t *data, int32_t timeout) {
    std::lock_guard<std::mutex> lock(mtx);
    return WRAPPER_CAST(channel)->GetMessage_(slot, cycle, dataLength, data, timeout);
}

int32_t fr_get_messages(void *channel, uint16_t *item, uint16_t *slot, uint8_t *cycle, uint8_t *dataLength, uint8_t *data, int32_t timeout) {
    return 0;
}

