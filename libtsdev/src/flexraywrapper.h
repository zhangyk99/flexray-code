//
// Created by 79933 on 2026/1/27.
//

#ifndef FLEXRAYWRAPPER_H
#define FLEXRAYWRAPPER_H


#ifndef FLEXRAYDLL
#ifdef __linux__
#define FLEXRAYDLL __attribute__((visibility("default"))) int32_t
#else
#define FLEXRAYDLL __declspec(dllexport) int32_t
#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

struct ClusterConfig;

struct ECUConfig;

FLEXRAYDLL fr_find_all_channels(char *str, int32_t *chnCount);

FLEXRAYDLL fr_get_error_code(char *error, int32_t eventNum);

FLEXRAYDLL fr_set_channel_config(void **channel, int32_t deviceIndex, int32_t chnIndex, uint16_t* chnConfig, ClusterConfig *clusterConfig, ECUConfig *ecuConfig);

FLEXRAYDLL fr_open_channel(void *channel);

FLEXRAYDLL fr_close_channel(void *channel);

FLEXRAYDLL fr_set_message(void *channel, uint16_t slot, uint8_t dataLength, uint8_t *data, int32_t timeout);

FLEXRAYDLL fr_set_messages(void *channel, uint16_t item, uint16_t *slot, uint8_t *dataLength, uint8_t *data, int32_t timeout);

FLEXRAYDLL fr_get_message(void *channel, uint16_t *slot, uint8_t *cycle, uint8_t *dataLength, uint8_t *data, int32_t timeout);

FLEXRAYDLL fr_get_messages(void *channel, uint16_t *item, uint16_t *slot, uint8_t *cycle, uint8_t *dataLength, uint8_t *data, int32_t timeout);

}

#endif //FLEXRAYWRAPPER_H
