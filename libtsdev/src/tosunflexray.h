//
// Created by 79933 on 2026/1/27.
//

#ifndef TOSUNFLEXRAY_H
#define TOSUNFLEXRAY_H

#include <vector>

#include "tsdev_host_lib_c.h"
#include "flexrayconfig.h"
#include "flexraylog.h"
#include "flexrayerror.h"

namespace TosunFlexRay{
    bool isLog = true;

    int32_t Init(tsdev_handle_t &handle){
        tsdev_context_t ctx{};
        memset(&ctx, 0, sizeof(ctx));
        ctx.dev_type = tsdev_devtype_all;
        ctx.com = tsdev_com_usb;
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

        auto res = tsdev_api_create(&ctx, &handle, NULL);
        if(res != tsdev_err_ok) {
            Log(isLog, "info", "Init", "finish");
            return FLEXRAY_ERROR_INIT;
        }else
            Log(isLog, "error", "Init", "tsdev_api_create");
        return res;
    }

    int32_t Finalize(tsdev_handle_t &handle){
        auto res = tsdev_api_close(handle);
        if(res != tsdev_err_ok) {
            Log(isLog, "info", "Finalize", "finish");
            return FLEXRAY_ERROR_FINALIZE;
        } else
            Log(isLog, "error", "Finalize", "tsdev_api_create");
        return FLEXRAY_OK;
    }

    int32_t FindAllChannel(){
        return 0;
    }


    class FlexRayChannel{
        size_t devHandle;
        int32_t chnIndex;
        int32_t errorCode = 0;

    public:
        FlexRayChannel(size_t devHandle, int32_t chnIndex, uint16_t *config, ClusterConfig *clusterConfig, ECUConfig *ecuConfig):
        devHandle(devHandle), chnIndex(chnIndex){

        }

        int32_t GetErrorCode(){

        }
    };

}

#endif //TOSUNFLEXRAY_H
