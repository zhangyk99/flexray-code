//
// Created by 79933 on 2026/1/27.
//

#ifndef LIBTSDEV_FLEXRAYERROR_H
#define LIBTSDEV_FLEXRAYERROR_H

enum ErrCode{
    FLEXRAY_OK                                 = 0,
    FLEXRAY_ERROR_NULLPTR                      = -5001, //空指针
    FLEXRAY_ERROR_INIT                         = -5002,
    FLEXRAY_ERROR_FINALIZE                     = -5003,
    FLEXRAY_ERROR_GET_DEVICE_COUNT             = -5004,
    FLEXRAY_ERROR_NO_DEVICE                    = -5005,
    FLEXRAY_ERROR_GET_DEVICE_INFO              = -5006,
    FLEXRAY_ERROR_DEVICE_CONNECT               = -5007,
    FLEXRAY_ERROR_GET_FLEXARY_CHANNEL          = -5008,
    FLEXRAY_ERROR_DEVICE_DISCONNECT            = -5009,
    FLEXRAY_ERROR_CHANNEL_CONFIG               = -5010,
    FLEXRAY_ERROR_CHANNEL_RECEIVE_EVENT        = -5011,
    FLEXRAY_ERROR_CHANNEL_START_NET            = -5012,
    FLEXRAY_ERROR_CHANNEL_STOP_NET             = -5013,
    FLEXRAY_ERROR_SET_MESSAGE                  = -5014,
    FLEXRAY_ERROR_GET_MESSAGE                  = -5015,

//    FLEXRAY_ERROR_SET_CHANNEL_CONFIG           = -2, //配置通道失败
//    FLEXRAY_ERROR_OPEN_CHANNEL                 = -3, //打开通道失败
//    FLEXRAY_ERROR_CLOSE_CHANNEL                = -4, //关闭通道失败
//    FLEXRAY_ERROR_SEND_MESSAGE                 = -5, //发送消息失败
//    FLEXRAY_ERROR_CLEAR_MESSAGE                = -6, //清除消息失败
//    FLEXRAY_ERROR_GET_MESSAGE                  = -7, //获取消息失败
//    FLEXRAY_ERROR_GET_MESSAGE_COUNT            = -8, //获取消息数量失败
};

#endif //LIBTSDEV_FLEXRAYERROR_H
