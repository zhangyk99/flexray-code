//
// Created by 79933 on 2025/12/15.
//

#ifndef TSFLEXRAY_FLEXRAYWRAPPER_H
#define TSFLEXRAY_FLEXRAYWRAPPER_H

#ifndef FLEXRAYDLL
#ifdef __linux__
#define FLEXRAYDLL __attribute__((visibility("default")))
#else
#define FLEXRAYDLL __declspec(dllexport)
#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

struct ClusterConfig;

struct ECUConfig;

FLEXRAYDLL int32_t fr_find_all_channels(char *str, int32_t *chnCount);

FLEXRAYDLL int32_t fr_get_error_code(char *error, int32_t *eventNum);

FLEXRAYDLL int32_t fr_set_channel_config(void **channel, int32_t deviceIndex, int32_t chnIndex, uint16_t* chnConfig, ClusterConfig *clusterConfig, ECUConfig *ecuConfig);

FLEXRAYDLL int32_t fr_open_channel(void *channel);

FLEXRAYDLL int32_t fr_close_channel(void *channel);

FLEXRAYDLL int32_t fr_set_message(void *channel, uint16_t slot, uint8_t cycle, uint8_t dataLength, uint8_t *data, int32_t timeout);

FLEXRAYDLL int32_t fr_set_messages(void *channel, uint16_t item, uint16_t *slot, uint8_t *cycle, uint8_t *dataLength, uint8_t *data, int32_t timeout);

FLEXRAYDLL int32_t fr_get_message(void *channel, uint16_t *slot, uint8_t *cycle, uint8_t *dataLength, uint8_t *data, int32_t timeout);

FLEXRAYDLL int32_t fr_get_messages(void *channel, uint16_t *item, uint16_t *slot, uint8_t *cycle, uint8_t *dataLength, uint8_t *data, int32_t timeout);

}

#endif //TSFLEXRAY_FLEXRAYWRAPPER_H
