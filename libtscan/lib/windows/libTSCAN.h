#ifndef _LIBTSCAN_H
#define _LIBTSCAN_H

#include <math.h>
#include <stdio.h>
#ifndef __cplusplus
#include <stdbool.h>
#else
#include <cstring>
#endif

#define CH1 0
#define CH2 1
#define CH3 2
#define CH4 3
#define CH5 4
#define CH6 5
#define CH7 6
#define CH8 7
#define CH9 8
#define CH10 9
#define CH11 10
#define CH12 11
#define CH13 12
#define CH14 13
#define CH15 14
#define CH16 15
#define CH17 16
#define CH18 17
#define CH19 18
#define CH20 19
#define CH21 20
#define CH22 21
#define CH23 22
#define CH24 23
#define CH25 24
#define CH26 25
#define CH27 26
#define CH28 27
#define CH29 28
#define CH30 29
#define CH31 30
#define CH32 31

typedef enum { lvlError = 1, lvlWarning = 2, lvlOK = 3, lvlHint = 4, lvlInfo = 5, lvlVerbose = 6 } TLogLevel;

// basic var type definition
typedef unsigned __int8 u8;
typedef signed __int8 s8;
typedef unsigned __int16 u16;
typedef signed __int16 s16;
typedef unsigned __int32 u32;
typedef signed __int32 s32;
typedef unsigned __int64 u64;
typedef signed __int64 s64;
// pointer definition
typedef unsigned __int8* pu8;
typedef signed __int8* ps8;
typedef unsigned __int16* pu16;
typedef signed __int16* ps16;
typedef unsigned __int32* pu32;
typedef signed __int32* ps32;
typedef unsigned __int64* pu64;
typedef signed __int64* ps64;

typedef unsigned __int8** ppu8;
typedef signed __int8** pps8;
typedef unsigned __int16** ppu16;
typedef signed __int16** pps16;
typedef unsigned __int32** ppu32;
typedef signed __int32** pps32;
typedef unsigned __int64** ppu64;
typedef signed __int64** pps64;

typedef float  single;
typedef float* psingle;
typedef double* pdouble;
typedef char* pchar;
typedef char** ppchar;
typedef void* TObject;
typedef void* pvoid;
typedef bool* pbool;
typedef size_t* psize_t;

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define SWAP_BYTES(x) (((x) >> 8) | ((x & 0xFF) << 8))

#ifdef DLLTEST_EXPORT
#define TSAPI(ret) __declspec(dllexport) ret __stdcall
#else
#define TSAPI(ret) __declspec(dllimport) ret __stdcall
#endif

#pragma pack(push)
#pragma pack(1)


#define TS_AF_INET  2
#define TS_SOCK_STREAM      1
#define TS_SOCK_DGRAM       2
#define TS_SOCK_RAW         3
#define  TS_MSG_DONTWAIT    0x08 


// CAN definitions
#define MASK_CANProp_DIR_TX 0x01
#define MASK_CANProp_REMOTE 0x02
#define MASK_CANProp_EXTEND 0x04
#define MASK_CANProp_ERROR  0x80
#define MASK_CANProp_LOGGED 0x60

// CAN FD message properties
#define MASK_CANFDProp_IS_FD 0x01
#define MASK_CANFDProp_IS_EDL MASK_CANFDProp_IS_FD
#define MASK_CANFDProp_IS_BRS 0x02
#define MASK_CANFDProp_IS_ESI 0x04

// LIN message properties
#define MASK_LINProp_DIR_TX         0x01
#define MASK_LINProp_SEND_BREAK     0x02
#define MASK_LINProp_RECEIVED_BREAK 0x04
#define MASK_LINProp_SEND_SYNC      0x80
#define MASK_LINProp_RECEIVED_SYNC  0x10

// C++ property definition
#define PROPERTY(t,n)  __declspec( property ( put = property__set_##n, get = property__get_##n ) ) t n;\
    typedef t property__tmp_type_##n
#define READONLY_PROPERTY(t,n) __declspec( property (get = property__get_##n) ) t n;\
    typedef t property__tmp_type_##n
#define WRITEONLY_PROPERTY(t,n) __declspec( property (put = property__set_##n) ) t n;\
    typedef t property__tmp_type_##n
#define GET(n) property__tmp_type_##n property__get_##n() 
#define SET(n) void property__set_##n(const property__tmp_type_##n& value)   

// C++ array property definitino
#define ARRAY_PROPERTY(t,n,s) __declspec( property ( put = property__set_##n, get = property__get_##n ) ) t n[s];\
    typedef t property__tmp_type_##n
#define READONLY_ARRAY_PROPERTY(t,n,s) __declspec( property (get = property__get_##n) ) t n[s];\
    typedef t property__tmp_type_##n
#define WRITEONLY_ARRAY_PROPERTY(t,n,s) __declspec( property (put = property__set_##n) ) t n[s];\
    typedef t property__tmp_type_##n
#define ARRAY_GET(n) property__tmp_type_##n property__get_##n(int index)
#define ARRAY_SET(n) void property__set_##n(int index, const property__tmp_type_##n& value)

const u8 DLC_DATA_BYTE_CNT[16] = {
    0, 1, 2, 3, 4, 5, 6, 7,
    8, 12, 16, 20, 24, 32, 48, 64
};
typedef struct _TLIBCAN
{
    u8 FIdxChn;//channel index starting from 0
    u8 FProperties;//default 0, masked status:  = CAN [7] 0-normal frame, 1-error frame [6] 0-not logged, 1-already logged [5-3] tbd [2] 0-std frame, 1-extended frame [1] 0-data frame, 1-remote frame [0] dir: 0-RX, 1-TX
    u8 FDLC;//dlc from 0 to 15   = CAN
    u8 FReserved;
    s32 FIdentifier;//CAN identifier   = CAN
    s64 FTimeUs;//timestamp in us = CAN
    u8 FData[8];
#ifdef __cplusplus
    // is_tx -----------------------------------------------------
    PROPERTY(bool, is_tx);
    GET(is_tx)
    {
        return (FProperties & MASK_CANProp_DIR_TX) != 0;
    }
    SET(is_tx)
    {
        if (value) {
            FProperties = FProperties | MASK_CANProp_DIR_TX;
        }
        else {
            FProperties = FProperties & (~MASK_CANProp_DIR_TX);
        }
    }
    // is_data ----------------------------------------------------
    PROPERTY(bool, is_data);
    GET(is_data)
    {
        return (FProperties & MASK_CANProp_REMOTE) == 0;
    }
    SET(is_data)
    {
        if (value) {
            FProperties = FProperties & (~MASK_CANProp_REMOTE);
        }
        else {
            FProperties = FProperties | MASK_CANProp_REMOTE;
        }
    }
    // is_std -----------------------------------------------------
    PROPERTY(bool, is_std);
    GET(is_std)
    {
        return (FProperties & MASK_CANProp_EXTEND) == 0;
    }
    SET(is_std)
    {
        if (value) {
            FProperties = FProperties & (~MASK_CANProp_EXTEND);
        }
        else {
            FProperties = FProperties | MASK_CANProp_EXTEND;
        }
    }
    // is_err ----------------------------------------------------
    PROPERTY(bool, is_err);
    GET(is_err)
    {
        return (FProperties & MASK_CANProp_ERROR) != 0;
    }
    SET(is_err)
    {
        if (value) {
            FProperties = FProperties | MASK_CANProp_ERROR;

        }
        else {
            FProperties = FProperties & (~MASK_CANProp_ERROR);
        }
    }
    // load data bytes -------------------------------------------
    void load_data_array(u8* a) {
        for (u32 i = 0; i < 8; i++) {
            FData[i] = *a++;
        }
    }
    void set_data(const u8 d0, const u8 d1, const u8 d2, const u8 d3, const u8 d4, const u8 d5, const u8 d6, const u8 d7) {
        FData[0] = d0;
        FData[1] = d1;
        FData[2] = d2;
        FData[3] = d3;
        FData[4] = d4;
        FData[5] = d5;
        FData[6] = d6;
        FData[7] = d7;
    }
    // initialize with standard identifier -----------------------
    void init_w_std_id(s32 AId, s32 ADLC) {
        FIdxChn = 0;
        FIdentifier = AId;
        FDLC = ADLC;
        FReserved = 0;
        FProperties = 0;
        is_tx = false;
        is_std = true;
        is_data = true;
        *(u64*)(&FData[0]) = 0;
        FTimeUs = 0;
    }
    // initialize with extended identifier -----------------------
    void init_w_ext_id(s32 AId, s32 ADLC) {
        FIdxChn = 0;
        FIdentifier = AId;
        FDLC = ADLC;
        FReserved = 0;
        FProperties = 0;
        is_tx = false;
        is_std = false;
        is_data = true;
        *(u64*)(&FData[0]) = 0;
        FTimeUs = 0;
    }
#endif
}TLIBCAN, * PLIBCAN;

typedef struct _TLIBCANFD
{
    u8 FIdxChn;//channel index starting from 0
    u8 FProperties;//default 0, masked status:  = CAN [7] 0-normal frame, 1-error frame [6] 0-not logged, 1-already logged [5-3] tbd [2] 0-std frame, 1-extended frame [1] 0-data frame, 1-remote frame [0] dir: 0-RX, 1-TX
    u8 FDLC;//dlc from 0 to 15   = CAN
    u8 FFDProperties;//[7-3] tbd <> CAN  [2] ESI, The E RROR S TATE I NDICATOR (ESI) flag is transmitted dominant by error active nodes, recessive by error passive nodes. ESI does not exist in CAN format frames [1] BRS, If the bit is transmitted recessive, the bit rate is switched from the standard bit rate of the A RBITRATION P HASE to the preconfigured alternate bit rate of the D ATA P HASE . If it is transmitted dominant, the bit rate is not switched. BRS does not exist in CAN format frames. [0] EDL: 0-normal CAN frame, 1-FD frame, added 2020-02-12, The E XTENDED D ATA L ENGTH (EDL) bit is recessive. It only exists in CAN FD format frames
    s32 FIdentifier;//CAN identifier   = CAN
    s64 FTimeUs;//timestamp in us = CAN
    u8 FData[64];
#ifdef __cplusplus
    // is_tx -----------------------------------------------------
    PROPERTY(bool, is_tx);
    GET(is_tx)
    {
        return (FProperties & MASK_CANProp_DIR_TX) != 0;
    }
    SET(is_tx)
    {
        if (value) {
            FProperties = FProperties | MASK_CANProp_DIR_TX;
        }
        else {
            FProperties = FProperties & (~MASK_CANProp_DIR_TX);
        }
    }
    // is_data ---------------------------------------------------
    PROPERTY(bool, is_data);
    GET(is_data)
    {
        return (FProperties & MASK_CANProp_REMOTE) == 0;
    }
    SET(is_data)
    {
        if (value) {
            FProperties = FProperties & (~MASK_CANProp_REMOTE);
        }
        else {
            FProperties = FProperties | MASK_CANProp_REMOTE;
        }
    }
    // is_std ----------------------------------------------------
    PROPERTY(bool, is_std);
    GET(is_std)
    {
        return (FProperties & MASK_CANProp_EXTEND) == 0;
    }
    SET(is_std)
    {
        if (value) {
            FProperties = FProperties & (~MASK_CANProp_EXTEND);
        }
        else {
            FProperties = FProperties | MASK_CANProp_EXTEND;
        }
    }
    // is_err ----------------------------------------------------
    PROPERTY(bool, is_err);
    GET(is_err)
    {
        return (FProperties & MASK_CANProp_ERROR) != 0;
    }
    SET(is_err)
    {
        if (value) {
            FProperties = FProperties | MASK_CANProp_ERROR;

        }
        else {
            FProperties = FProperties & (~MASK_CANProp_ERROR);
        }
    }
    // is_edl ----------------------------------------------------
    PROPERTY(bool, is_edl);
    GET(is_edl)
    {
        return (FFDProperties & MASK_CANFDProp_IS_FD) != 0;
    }
    SET(is_edl)
    {
        if (value) {
            FFDProperties = FFDProperties | MASK_CANFDProp_IS_FD;
        }
        else {
            FFDProperties = FFDProperties & (~MASK_CANFDProp_IS_FD);
        }
    }
    // is_brs ----------------------------------------------------
    PROPERTY(bool, is_brs);
    GET(is_brs)
    {
        return (FFDProperties & MASK_CANFDProp_IS_BRS) != 0;
    }
    SET(is_brs)
    {
        if (value) {
            FFDProperties = FFDProperties | MASK_CANFDProp_IS_BRS;
        }
        else {
            FFDProperties = FFDProperties & (~MASK_CANFDProp_IS_BRS);
        }
    }
    // is_esi ----------------------------------------------------
    PROPERTY(bool, is_esi);
    GET(is_esi)
    {
        return (FFDProperties & MASK_CANFDProp_IS_ESI) != 0;
    }
    SET(is_esi)
    {
        if (value) {
            FFDProperties = FFDProperties | MASK_CANFDProp_IS_ESI;
        }
        else {
            FFDProperties = FFDProperties & (~MASK_CANFDProp_IS_ESI);
        }
    }
    // load data bytes -------------------------------------------
    void load_data(u8* a) {
        for (u32 i = 0; i < 64; i++) {
            FData[i] = *a++;
        }
    }
    // initialize with standard identifier -----------------------
    void init_w_std_id(s32 AId, s32 ADLC) {
        s32 i;
        FIdxChn = 0;
        FIdentifier = AId;
        FDLC = ADLC;
        FProperties = 0;
        FFDProperties = MASK_CANFDProp_IS_FD;
        is_tx = false;
        is_std = true;
        is_data = true;
        for (i = 0; i < 64; i++) FData[i] = 0;
        FTimeUs = 0;
    }
    // initialize with extended identifier -----------------------
    void init_w_ext_id(s32 AId, s32 ADLC) {
        s32 i;
        FIdxChn = 0;
        FIdentifier = AId;
        FDLC = ADLC;
        FFDProperties = MASK_CANFDProp_IS_FD;
        FProperties = 0;
        is_tx = false;
        is_std = false;
        is_data = true;
        for (i = 0; i < 64; i++) FData[i] = 0;
        FTimeUs = 0;
    }
    // get fd data length ----------------------------------------
    s32 get_data_length() {
        s32 l = MIN(FDLC, 15);
        l = MAX(l, 0);
        return DLC_DATA_BYTE_CNT[l];
    }
    // to CAN struct ---------------------------------------------
    TLIBCAN to_tcan(void) {
        return *(TLIBCAN*)(&FIdxChn);
    }
#endif
}TLIBCANFD, * PLIBCANFD;


typedef struct _TLIBLIN
{
    u8 FIdxChn;//channel index starting from 0
    u8 FErrCode;//0: normal
    u8 FProperties;//default 0, masked status:  [7] tbd [6] 0-not logged, 1-already logged [5-4] FHWType [3] 0-not ReceivedSync, 1- ReceivedSync [2] 0-not received FReceiveBreak, 1-Received Break [1] 0-not send FReceiveBreak, 1-send Break [0] dir: 0-RX, 1-TX
    u8 FDLC;//dlc from 0 to 8
    u8 FIdentifier;//LIN identifier:0--64
    u8 FChecksum;//LIN checksum
    u8 FStatus;//place holder 1
    s64 FTimeUS;//timestamp in us
    u8 FData[8];
#ifdef __cplusplus
    // is_tx -----------------------------------------------------
    PROPERTY(bool, is_tx);
    GET(is_tx)
    {
        return (FProperties & MASK_LINProp_DIR_TX) != 0;
    }
    SET(is_tx)
    {
        if (value) {
            FProperties = FProperties | MASK_LINProp_DIR_TX;
        }
        else {
            FProperties = FProperties & (~MASK_LINProp_DIR_TX);
        }
    }
    // load data bytes -------------------------------------------
    void load_data(u8* a) {
        for (u32 i = 0; i < 8; i++) {
            FData[i] = *a++;
        }
    }
    // initialize with identifier --------------------------------
    void init_w_id(const s32 AId, const s32 ADLC) {
        FIdxChn = 0;
        FErrCode = 0;
        FProperties = 0;
        FDLC = ADLC;
        FIdentifier = AId;
        *(__int64*)(&FData[0]) = 0;
        FChecksum = 0;
        FStatus = 0;
        FTimeUS = 0;
    }
#endif
}TLIBLIN, * PLIBLIN;

typedef struct _TLIBFlexRay
{
    u8 FIdxChn;//channel index starting from 0
    u8 FChannelMask;//0: reserved, 1: A, 2: B, 3: AB
    u8 FDir;//0: Rx, 1: Tx, 2: Tx Request
    u8 FPayloadLength;//payload length in bytes
    u8 FActualPayloadLength;//actual data bytes
    u8 FCycleNumber;//cycle number: 0~63
    u8 FCCType;//0 = Architecture independent, 1 = Invalid CC type, 2 = Cyclone I, 3 = BUSDOCTOR, 4 = Cyclone II, 5 = Vector VN interface, 6 = VN - Sync - Pulse(only in Status Event, for debugging purposes only)
    u8 FFrameType;// 1 reserved byte
    u16 FHeaderCRCA;// header crc A
    u16 FHeaderCRCB;// header crc B
    u16 FFrameStateInfo;// bit 0~15, error flags
    u16 FSlotId;// static seg: 0~1023
    u32 FFrameFlags;// bit 0~22 //0 1 = Null frame //1 1 = Data segment contains valid data //2 1 = Sync bit //3 1 = Startup flag //4 1 = Payload preamble bit //5 1 = Reserved bit, //6 1 = Error flag(error frame or invalid frame) //7..14 Reserved //15 1 = Async.monitoring has generated this event //16 1 = Event is a PDU //17 Valid for PDUs only.The bit is set if the PDU is valid(either if the PDU has no  // update bit, or the update bit for the PDU was set in the received frame). //18 Reserved //19 1 = Raw frame(only valid if PDUs are used in the configuration).A raw frame may  // contain PDUs in its payload //20 1= Dynamic segment 0 = static segment ,21 This flag is only vaild for frames and not for PDUS. 1 = ThePDUs in the payload of // this frame are logged in separate logging entries. 0 = The PDUs in the payload of this  // frame must be extracted out of this frame.The logging file does not contain separate  // PDU - entries. //22 Valid for PDUs only.The bit is set if the PDU has an update bit
    u32 FFrameCRC;//frame crc
    u64 FReserved1;//8 reserved bytes
    u64 FReserved2;//8 reserved bytes
    u64 FTimeUs;// timestamp in us
    u8 FData[254];
#ifdef __cplusplus
    // is_tx -----------------------------------------------------
    PROPERTY(bool, is_tx);
    GET(is_tx) {
        return FDir != 0;
    }
    SET(is_tx) {
        if (value) {
            FDir = 1;
        }
        else {
            FDir = 0;
        }
    }
    // is_null ---------------------------------------------------
    PROPERTY(bool, is_null);
    GET(is_null) {
        return (FFrameFlags & ((u32)1 << 0)) != 0;
    }
    SET(is_null) {
        if (value) {
            FFrameFlags |= ((u32)1 << 0);
        }
        else {
            FFrameFlags &= ~((u32)1 << 0);
        }
    }
    // is_data ---------------------------------------------------
    PROPERTY(bool, is_data);
    GET(is_data) {
        return (FFrameFlags & ((u32)1 << 1)) != 0;
    }
    SET(is_data) {
        if (value) {
            FFrameFlags |= ((u32)1 << 1);
        }
        else {
            FFrameFlags &= ~((u32)1 << 1);
        }
    }
    // is_sync ---------------------------------------------------
    PROPERTY(bool, is_sync);
    GET(is_sync) {
        return (FFrameFlags & ((u32)1 << 2)) != 0;
    }
    SET(is_sync) {
        if (value) {
            FFrameFlags |= ((u32)1 << 2);
        }
        else {
            FFrameFlags &= ~((u32)1 << 2);
        }
    }
    // is_startup ------------------------------------------------
    PROPERTY(bool, is_startup);
    GET(is_startup) {
        return (FFrameFlags & ((u32)1 << 3)) != 0;
    }
    SET(is_startup) {
        if (value) {
            FFrameFlags |= ((u32)1 << 3);
        }
        else {
            FFrameFlags &= ~((u32)1 << 3);
        }
    }
    // is_pp -----------------------------------------------------
    PROPERTY(bool, is_pp);
    GET(is_pp) {
        return (FFrameFlags & ((u32)1 << 4)) != 0;
    }
    SET(is_pp) {
        if (value) {
            FFrameFlags |= ((u32)1 << 4);
        }
        else {
            FFrameFlags &= ~((u32)1 << 4);
        }
    }
    // is_err ----------------------------------------------------
    PROPERTY(bool, is_err);
    GET(is_err) {
        return (FFrameFlags & ((u32)1 << 6)) != 0;
    }
    SET(is_err) {
        if (value) {
            FFrameFlags |= ((u32)1 << 6);
        }
        else {
            FFrameFlags &= ~((u32)1 << 6);
        }
    }
    // is_static_segment -----------------------------------------
    PROPERTY(bool, is_static_segment);
    GET(is_static_segment) {
        return (FFrameFlags & ((u32)1 << 20)) != 0;
    }
    SET(is_static_segment) {
        if (value) {
            FFrameFlags |= ((u32)1 << 20);
        }
        else {
            FFrameFlags &= ~((u32)1 << 20);
        }
    }
    // initialize with slot id -----------------------------------
    void init_w_slot_id(const s32 ASlotId, const s32 ADLC) {
        FIdxChn = 0;
        FChannelMask = 1;
        FDir = 0;
        FPayloadLength = ADLC;
        FActualPayloadLength = ADLC;
        FCycleNumber = 0;
        FCCType = 5;
        FFrameType = 0;
        FHeaderCRCA = 0;
        FHeaderCRCB = 0;
        FFrameStateInfo = 0;
        FFrameFlags = 1 << 1; // data frame
        FSlotId = ASlotId;
        FFrameCRC = 0;
        FReserved1 = 0;
        FReserved2 = 0;
        FTimeUs = 0;
        for (u32 i = 0; i < 254; i++) {
            FData[i] = 0;
        }
    }
    // load data bytes -------------------------------------------
    void load_data(u8* a) {
        for (u32 i = 0; i < 254; i++) {
            FData[i] = *a++;
        }
    }
#endif
}TLIBFlexRay, * PLIBFlexRay;

typedef struct _TLibEth_CMD_config {
    u8  eth_config0;
    //bit 0-1 phy_type:2; //0: 100base-Tx/1000Base-TX, 1: 100/1000Base-T1, 2,3: rev
    //bit2 auto_neg : 1;    
    //bit3-4: speed : 2; //0-10mbps, 1-100mbps, 2-1000mbps
    //bit5: is_master : 1;
    //bit6-7 loop : 2;//0: no loop, 1: mac_loop, 2: phy-loop, 3: phy_remote loop
    u8 eth_config1;
    //bit0 wakeup : 1;//0-disable, 1-enable
    //bit1-4 test_mode : 4;
    //  0x00 normal operation  other test mode
    //bit5-6 tx_mode : 2;
    //  0x00 enable 0x01 disable
    //bit7  enable : 1;
    u8 eth_config2;
    //bit0-4 phy_addr : 5;
    //bit5 accept wrong crc frame:1
    //bit6-7: not used now
    u8 eth_config3;
    //bit0: disable_promiscuous_mode
    //bit1: enable_recieve_all
    //bit2-3: enable_srouce_fileter: 0 disable 1: enable 2 inverse
    //bit4: inverse_dest_fileter
    //bit5-6: ControlFrames: 0: block all  1: forward all  2: forward by filter
    //bit7: enable rx broadcast frame
    u8 filter_config0;
    //bit0-1: multicast frame filter: 0: no filter  1: perfect 2: hash 3: hash and perfect
    //bit2-3: unicast frame filter: 0: perfect 1: hash 2: hash and perfect
    u8 filter_config1;
    u64 filter_hash_table;
    //bit0-47: mac addr For example, if 0x112233445566 is received
    //          (0x11 in lane 0 of the first column) on the MII as the destination address, then the
    //          MacAddress0 Register [47:0] is compared with 0x665544332211
    //          perfect0 is always enable
    u64 filter_perfect0;
    //bit63: AE: Address Enable, When this bit is set, the address filter module uses the second MAC address for perfect
    //          filtering. When this bit is reset, the address filter module ignores the address for filtering.
    //bit62: SA: Source Address:
    //          When this bit is set, the MAC Address1[47:0] is used to compare with the SA fields of the
    //          received packet. When this bit is reset, the MAC Address x[47:0] is used to compare with the
    //          DA fields of the received packet.
    //bit56-61: MBC[5:0]: Mask Byte Control
    //          These bits are mask control bits for comparing each of the MAC Address bytes. When set
    //          high, the MAC does not compare the corresponding byte of received DA or SA with the
    //          contents of MAC Address1 registers. Each bit controls the masking of the bytes as follows:
    //          Bit 29: Register 194[15:8]
    //          Bit 28: Register 194[7:0]
    //          Bit 27: Register 195[31:24]
    //          ..
    //          Bit 24: Register 195[7:0]
    //          You can filter a group of addresses (known as group address filtering) by masking one or
    //          more bytes of the address.
    //bit0-47:  same as filter_perfect0
    u64 filter_perfect1;
    u64 rev[6];
} TLibEth_CMD_config, * PLibEth_CMD_config;

#define SWAP_BYTES(x) (((x) >> 8) | ((x & 0xFF) << 8))

typedef struct _TLibEthernetHeader {
    u8   FIdxChn;
    u8 FIdxSwitch;
    u8 FIdxPort;
    u8 FConfig;
    u16 FEthernetPayloadLength;
    u16 FReserved;
    u64 FTimeUs;
    u8* FEthernetDataAddr;
#ifndef _WIN64
    u32 FPadding;                // to be compatible with x64
#endif
    pu8 actual_data_pointer() {
        return &FIdxChn + sizeof(_TLibEthernetHeader);
    }
    void reset_data_pointer() {
        FEthernetDataAddr = actual_data_pointer();
    }
    bool has_vlans(pu16 AOffsetBytes) {
        *AOffsetBytes = 0;
        while (*(pu16)(actual_data_pointer() + 6 + 6 + (*AOffsetBytes)) == 0x0081) {
            *AOffsetBytes = *AOffsetBytes + 4;
        }
        return *AOffsetBytes > 0;
    }
    pu16 ethernet_type_addr() {
        u16 o;
        has_vlans(&o);
        pu8 p = FEthernetDataAddr + 6 + 6 + o;
        return (pu16)(p);
    }
    pu8 destination_mac_addr() {
        return FEthernetDataAddr;
    }
    void init(const u16 APayloadLength) {
        FIdxChn = 0;
        FIdxSwitch = 0;
        FIdxPort = 0;
        FConfig = 0;
        FEthernetPayloadLength = APayloadLength;
        FReserved = 0;
        FTimeUs = 0;
        reset_data_pointer();
#ifndef _WIN64
        FPadding = 0;
#endif
        s32 i;
        pu8 p = FEthernetDataAddr;
        s32 n = MIN(1612 - 14, APayloadLength);
        n += 14;
        for (i = 0; i < n; i++) {
            *p++ = 0;
        }
        *(pu16)(ethernet_type_addr()) = 0x00; // IPV4 = SWAP_BYTES(0x0800)
        p = destination_mac_addr();
        for (i = 0; i < 6; i++) {
            *p++ = 0xFF;
        }
    }
    pu8 source_mac_addr() {
        return FEthernetDataAddr + 6;
    }
    u16 ethernet_type() {
        u16 t = *(pu16)(ethernet_type_addr());
        return SWAP_BYTES(t);
    }
    bool is_ip_frame() {
        return ethernet_type() == 0x0800;
    }
    pu8 destination_ip_addr() {
        if (!is_ip_frame()) return nullptr;
        u16 o;
        has_vlans(&o);
        return FEthernetDataAddr + 0x1E + o;
    }
    pu8 source_ip_addr() {
        if (!is_ip_frame()) return nullptr;
        u16 o;
        has_vlans(&o);
        return FEthernetDataAddr + 0x1A + o;
    }
    bool is_tcp_frame() {
        u16 o;
        has_vlans(&o);
        return (ethernet_type() == 0x0800) && (0x06 == *(FEthernetDataAddr + 0x17 + o));
    }
    bool is_udp_frame() {
        u16 o;
        has_vlans(&o);
        return (ethernet_type() == 0x0800) && (0x11 == *(FEthernetDataAddr + 0x17 + o));
    }
    pu16 first_vlan_addr() {
        return (pu16)(actual_data_pointer() + 6 + 6);
    }
    s32 total_ethernet_packet_length() {
        u16 o;
        has_vlans(&o);
        return sizeof(_TLibEthernetHeader) + 6 + 6 + 2 + o + FEthernetPayloadLength;
    }
    s32 ethernet_data_length() {
        u16 o;
        has_vlans(&o);
        return FEthernetPayloadLength + 6 + 6 + o + 2;
    }
    void set_ethernet_data_length(const u16 ALength) {
        u16 o;
        has_vlans(&o);
        o += 14;
        if (ALength > o) {
            FEthernetPayloadLength = ALength - o;
        }
    }
    pu8 ethernet_payload_addr() {
        u16 o;
        has_vlans(&o);
        return FEthernetDataAddr + 6 + 6 + 2 + o;
    }
    u16 destination_port_value() {
        if (!is_ip_frame()) return 0;
        u16 o;
        has_vlans(&o);
        o = *(u16*)(FEthernetDataAddr + 0x24 + o);
        return SWAP_BYTES(o);
    }
    u16 source_port_value() {
        if (!is_ip_frame()) return 0;
        u16 o;
        has_vlans(&o);
        o = *(u16*)(FEthernetDataAddr + 0x22 + o);
        return SWAP_BYTES(o);
    }
    u16 get_ip_header_checksum() {
        u16 o;
        if (!is_ip_frame()) return 0;
        has_vlans(&o);
        o = *(pu16)(FEthernetDataAddr + 0x18 + o);
        return SWAP_BYTES(o);
    }
    void set_ip_header_checksum(const u16 AValue) {
        u16 o;
        if (!is_ip_frame()) return;
        has_vlans(&o);
        *(pu16)(FEthernetDataAddr + 0x18 + o) = SWAP_BYTES(AValue);
    }
    void add_vlan(u16 AVlan)
    {
        pu16 pvlan = first_vlan_addr();
        *pvlan = 0x0081;
        *(pvlan + 1) = SWAP_BYTES(AVlan);
    }
    void set_destination_mac_addr(pu8 ADesMacaddress)
    {
        pu8 p = destination_mac_addr();
        memcpy(p, ADesMacaddress, 6);
    }
    void set_source_mac_address(pu8 ASouMacaddress)
    {
        pu8 p = source_mac_addr();
        memcpy(p, ASouMacaddress, 6);
    }
    void set_udp_destination_ip_addr(const char* DesIpAddr)
    {
        u16 o;
        has_vlans(&o);
        pu8 IsUDP = (FEthernetDataAddr + 0x17 + o);
        *IsUDP = 0X11;
        *(IsUDP - 1) = 0X1;
        pu16 type = ethernet_type_addr();
        *type = 0x0008;
        *(type + 1) = 0x0045;
        pu8 p = destination_ip_addr();
        ip_str_to_u8(DesIpAddr, p);
    }
    void set_udp_source_ip_addr(const char* SouIpAddr)
    {
        u16 o;
        has_vlans(&o);
        pu8 IsUDP = (FEthernetDataAddr + 0x17 + o);
        *IsUDP = 0X11;
        *(IsUDP - 1) = 0X1;
        pu16 type = ethernet_type_addr();
        *type = 0x0008;
        *(type + 1) = 0x0045;
        pu8 p = source_ip_addr();
        ip_str_to_u8(SouIpAddr, p);
    }
    void set_udp_port_destination(const u16 AValue) {
        u16 o;
        if (!is_udp_frame()) return;
        has_vlans(&o);
        *(pu16)(FEthernetDataAddr + 0x24 + o) = SWAP_BYTES(AValue);
    }
    void set_udp_port_source(const u16 AValue) {
        u16 o;
        if (!is_udp_frame()) return;
        has_vlans(&o);
        *(pu16)(FEthernetDataAddr + 0x22 + o) = SWAP_BYTES(AValue);
    }
    void set_udp_payload_length(const u16 ALength) {
        u16 o;
        if (!is_udp_frame()) return;
        has_vlans(&o);
        *(pu16)(FEthernetDataAddr + 0x26 + o) = SWAP_BYTES(ALength + 8/*header length*/);
    }
    void set_udp_payload(pu8 AData, s32 ALen)
    {
        u16 o;
        has_vlans(&o);
        FEthernetPayloadLength = ALen + 30 + o;
        pu16 d = (pu16)ethernet_payload_addr();
        *(d + 1) = (FEthernetPayloadLength << 8);
        set_udp_payload_length(ALen);
        pu8 p = get_udp_payload_addr();
        memcpy(p, AData, ALen);
    }
    void init_udp_packet(
        const pu8 src_mac, const pu8 dst_mac,
        const char* src_ip, const char* dst_ip,
        u16 src_port, u16 dst_port,
        const pu8 payload, u16 payload_len
    ) {
        // 1. 初始化以太网头
        set_destination_mac_addr(dst_mac);
        set_source_mac_address(src_mac);
        *(pu16)(ethernet_type_addr()) = SWAP_BYTES(0x0800); // IPv4

        // 2. 初始化 IP 头
        u16 ip_header_len = 20; // 无选项
        u16 total_ip_len = ip_header_len + 8 + payload_len; // IP头 + UDP头 + 数据
        pu8 ip_header = ethernet_payload_addr();

        // IP 版本 (4) + 头长度 (5 words)
        ip_header[0] = 0x45;
        ip_header[1] = 0; // DSCP/ECN
        *(pu16)(ip_header + 2) = SWAP_BYTES(total_ip_len); // 总长度
        *(pu16)(ip_header + 4) = 0; // ID (可随机化)
        *(pu16)(ip_header + 6) = SWAP_BYTES(0x4000); // Flags (DF) + Fragment Offset
        ip_header[8] = 64; // TTL
        ip_header[9] = 17; // 协议 (UDP=17)
        *(pu16)(ip_header + 10) = 0; // 校验和（先填0）
        ip_str_to_u8(src_ip, ip_header + 12); // 源IP
        ip_str_to_u8(dst_ip, ip_header + 16); // 目标IP

        // 计算 IP 校验和
        *(pu16)(ip_header + 10) = ip_checksum(ip_header, ip_header_len);

        // 3. 初始化 UDP 头
        pu8 udp_header = ip_header + ip_header_len;
        *(pu16)(udp_header + 0) = SWAP_BYTES(src_port); // 源端口
        *(pu16)(udp_header + 2) = SWAP_BYTES(dst_port); // 目标端口
        *(pu16)(udp_header + 4) = SWAP_BYTES(8 + payload_len); // UDP长度
        *(pu16)(udp_header + 6) = 0; // 校验和（可选）

        // 4. 填充数据
        if (payload && payload_len > 0) {
            memcpy(udp_header + 8, payload, payload_len);
        }
        FEthernetPayloadLength = ip_header_len + 8 + payload_len;
        *(pu16)(udp_header + 6) = 0;
    }
private:
    int ip_str_to_u8(const char* ip_str, pu8 ip_u8) {
        s32 parts[4];
        if (sscanf_s(ip_str, "%d.%d.%d.%d", &parts[0], &parts[1], &parts[2], &parts[3]) != 4) {
            return -1; // 格式错误
        }
        for (int i = 0; i < 4; i++) {
            if (parts[i] < 0 || parts[i] > 255) {
                return -1; // 数值越界
            }
            ip_u8[i] = (u8)parts[i];
        }
        return 0; // 成功

        return 0; // 成功
    }
    bool check_udp_fragment(pu16 AId, pu16 AOffset) {
        u16 o;
        bool r;
        has_vlans(&o);
        r = (0x40 & *(FEthernetDataAddr + 0x14 + o)) == 0;
        if (r) {
            *AOffset = *(pu16)(FEthernetDataAddr + 0x14 + o);
            *AOffset = (SWAP_BYTES(*AOffset) & 0x1FFF) << 3;
            *AId = *(pu16)(FEthernetDataAddr + 0x12 + o);
            *AId = SWAP_BYTES(*AId);
        }
        return r;
    }
    pu8 get_ip_packet_data_addr() {
        u16 o;
        if (!is_ip_frame()) return nullptr;
        has_vlans(&o);
        return FEthernetDataAddr + 0x0E + o;
    }
    pu8 get_udp_payload_addr() {
        u16 o, id, fo;
        if (!is_udp_frame()) return nullptr;
        if (check_udp_fragment(&id, &fo)) {
            if (fo > 0) {
                return get_ip_packet_data_addr();
            }
        }
        has_vlans(&o);
        return FEthernetDataAddr + 0x2A + o;
    }
    u16 ip_checksum(const u8* data, u16 len) {
        u32 sum = 0;
        for (u16 i = 0; i < len; i += 2) {
            sum += (data[i] << 8) | data[i + 1];
        }
        while (sum >> 16) {
            sum = (sum & 0xFFFF) + (sum >> 16);
        }
        return ~sum;
    }

} TLibEthernetHeader, * PLibEthernetHeader;

typedef struct _TLibEthernetMax {
    TLibEthernetHeader FHeader;
    u8 FBytes[1612];
    void reset_data_pointer() {
        FHeader.FEthernetDataAddr = FHeader.actual_data_pointer();
    }
} TLibEthernetMax, * PLibEthernetMax;

typedef struct _TLIBFlexray_controller_config
{
    u8 NETWORK_MANAGEMENT_VECTOR_LENGTH;
    u8 PAYLOAD_LENGTH_STATIC;
    u16 FReserved;
    u16 LATEST_TX;//__ prtc1Control
    u16 T_S_S_TRANSMITTER;
    u8 CAS_RX_LOW_MAX;
    u8 SPEED;//0 for 10m, 1 for 5m, 2 for 2.5m, convert from Database
    u16 WAKE_UP_SYMBOL_RX_WINDOW;
    u8 WAKE_UP_PATTERN;//__ prtc2Control
    u8 WAKE_UP_SYMBOL_RX_IDLE;
    u8 WAKE_UP_SYMBOL_RX_LOW;
    u8 WAKE_UP_SYMBOL_TX_IDLE;
    u8 WAKE_UP_SYMBOL_TX_LOW;//__ succ1Config
    u8 channelAConnectedNode;//Enable ChannelA: 0: Disable 1: Enable
    u8 channelBConnectedNode;//Enable ChannelB: 0: Disable 1: Enable
    u8 channelASymbolTransmitted;//Enable Symble Transmit function of Channel A: 0: Disable 1: Enable
    u8 channelBSymbolTransmitted;//Enable Symble Transmit function of Channel B: 0: Disable 1: Enable
    u8 ALLOW_HALT_DUE_TO_CLOCK;
    u8 single_SLOT_ENABLED;//FALSE_0, TRUE_1
    u8 wake_up_idx;//Wake up channe: 0:ChannelA�� 1:ChannelB
    u8 ALLOW_PASSIVE_TO_ACTIVE;
    u8 COLD_START_ATTEMPTS;
    u8 synchFrameTransmitted;//Need to transmit sync frame
    u8 startupFrameTransmitted;//Need to transmit startup frame // __ succ2Config
    u32 LISTEN_TIMEOUT;
    u8 LISTEN_NOISE;//2_16 __ succ3Config
    u8 MAX_WITHOUT_CLOCK_CORRECTION_PASSIVE;
    u8 MAX_WITHOUT_CLOCK_CORRECTION_FATAL;
    u8 REVERS0;//Memory Align // __ gtuConfig //__ gtu01Config
    u32 MICRO_PER_CYCLE;//__ gtu02Config
    u16 Macro_Per_Cycle;
    u8 SYNC_NODE_MAX;
    u8 REVERS1;//Memory Align //__ gtu03Config
    u8 MICRO_INITIAL_OFFSET_A;
    u8 MICRO_INITIAL_OFFSET_B;
    u8 MACRO_INITIAL_OFFSET_A;
    u8 MACRO_INITIAL_OFFSET_B;//__ gtu04Config
    u16 N_I_T;
    u16 OFFSET_CORRECTION_START;//__ gtu05Config
    u8 DELAY_COMPENSATION_A;
    u8 DELAY_COMPENSATION_B;
    u8 CLUSTER_DRIFT_DAMPING;
    u8 DECODING_CORRECTION;//__ gtu06Config
    u16 ACCEPTED_STARTUP_RANGE;
    u16 MAX_DRIFT;//__ gtu07Config
    u16 STATIC_SLOT;
    u16 NUMBER_OF_STATIC_SLOTS;//__ gtu08Config
    u8 MINISLOT;
    u8 REVERS2;//Memory Align
    u16 NUMBER_OF_MINISLOTS;//__ gtu09Config
    u8 DYNAMIC_SLOT_IDLE_PHASE;
    u8 ACTION_POINT_OFFSET;
    u8 MINISLOT_ACTION_POINT_OFFSET;
    u8 REVERS3;//Memory Align __ gtu10Config
    u16 OFFSET_CORRECTION_OUT;
    u16 RATE_CORRECTION_OUT;//__ gtu11Config
    u8 EXTERN_OFFSET_CORRECTION;
    u8 EXTERN_RATE_CORRECTION;
    u8 REVERS4;//Memory Align
    u8 config_byte;//Memory Align //bit0: 1������cha���ն˵���  0�������� //bit1: 1������chb���ն˵���  0�������� //bit2: 1�����ý���FIFO    0�������� //bit4: 1��cha�Ž�ʹ��             0����ʹ�� //bit5: 1��chb�Ž�ʹ��             0����ʹ��
}TLIBFlexray_controller_config, * PLIBFlexray_controller_config;

typedef enum {
    N_OK = 0,
    N_TP_TIMEOUT_AS = 139,
    N_TP_TIMEOUT_AR = 140,
    N_TP_TIMEOUT_BS = 141,
    N_TP_TIMEOUT_CR = 142,
    N_TP_WRONG_SN = 143,
    N_TP_INVALID_FS = 144,
    N_TP_UNEXP_PDU = 145,
    N_TP_WFT_OVRN = 146,
    N_TP_BUFFER_OVFLW = 147,
    N_TP_NOT_IDLE = 148,
    N_TP_ERROR_FROM_CAN_DRIVER = 149,
    IDX_ERR_LIN_MASTER_TRANSMIT_N_AS_TIMEOUT = 202,
    IDX_ERR_LIN_MASTER_TRANSMIT_TRANSMIT_ERROR = 203,
    IDX_ERR_LIN_MASTER_REV_N_CR_TIMEOUT = 204,
    IDX_ERR_LIN_MASTER_REV_ERROR = 205,
    IDX_ERR_LIN_MASTER_REV_INTERLLEAVE_TIMEOUT = 206,
    IDX_ERR_LIN_MASTER_REV_NO_RESPONSE = 207,
    IDX_ERR_LIN_MASTER_REV_SN_ERROR = 208,
    IDX_ERR_LIN_SLAVE_TRANSMIT_N_CR_TIMEOUT = 209,
    IDX_ERR_LIN_SLAVE_REV_N_CR_TIMEOUT = 210,
    IDX_ERR_LIN_SLAVE_TRANSMIT_ERROR = 211,
    IDX_ERR_LIN_SLAVE_REV_ERROR = 212,
}ISO_TP_RESAULT, * PSO_TP_RESAULT;
typedef enum {
    TS_USB = 0,
    TS_WIFI = 1,
    TS_USB_EX = 2,
    TS_TSDev_EthIF = 3,
    TS_TSDev_USBIF = 4,
}TTSHWType, * PTSHWType;
typedef enum {
    tldt_CAN = 0,
    tldt_LIN = 1,
    tldt_FR = 2,
    tldt_Eth = 3,
    tldt_AI = 4,
    tldt_AO = 5,
    tldt_DI = 6,
    tldt_DO = 7,
    tldt_GPS = 8,
    tldt_Undef = 9,
}TLinkedDataChnType, * PLinkedDataChnType;
typedef enum {
    CHN1 = 0,
    CHN2 = 1,
    CHN3 = 2,
    CHN4 = 3,
    CHN5 = 4,
    CHN6 = 5,
    CHN7 = 6,
    CHN8 = 7,
}TAPP_CHANNEL, * PAPP_CHANNEL;
typedef enum {
    lfdtCAN = 0,
    lfdtISOCAN = 1,
    lfdtNonISOCAN = 2,
}TLIBCANFDControllerType, * PLIBCANFDControllerType;
typedef enum {
    lfdmNormal = 0,
    lfdmACKOff = 1,
    lfdmRestricted = 2,
}TLIBCANFDControllerMode, * PLIBCANFDControllerMode;
typedef enum {
    REPLAY_IDLE_STATE = 0,
    REPLAY_RUNNING = 1,
    REPLAY_PAUSING = 2,
}TReply_State, * PReply_State;
typedef struct _TLIBGPSData {
    u64 FTimeUS;
    u32 UTCTime;
    u32 UTCDate;
    float Latitude;
    float Longitude;
    float Speed;
    float Direct;
    float Altitude;
    u8 N_S;
    u8 E_W;
    u8 Satellite;
    u8 FIdxChn;
}TLIBGPSData, * PLIBGPSData;

typedef struct _TLIBCompressedData {
    s32 FRealDataLength;
    u8 FDataBuffer[24080];
}TLIBCompressedData, * PLIBCompressedData;

typedef struct _TLIBEth_CMD_config {
    u8 eth_config0;
    u8 eth_config1;
    u8 eth_config2;
    u8 eth_config3;
    u8 filter_config0;
    u8 filter_config1;
    u64 filter_hash_table;
    u64 filter_perfect0;
    u64 filter_perfect1;
    u64 rev[6];
}TLIBEth_CMD_config, * PLIBEth_CMD_config;

typedef struct _TLIBTrigger_def {
    u16 slot_id;
    u8 frame_idx;
    u8 cycle_code;
    u8 config_byte;
    u8 rev;
}TLIBTrigger_def, * PLIBTrigger_def;

typedef struct _TEMMC_RECORD_DATA {
    u32 FUTCDate;
    u32 FUTCTime;
    u32 FStartSector;
    u32 FSectorSize;
    u32 FOffSetMiniSecond;
}TEMMC_RECORD_DATA, * PEMMC_RECORD_DATA;

typedef struct _TDiagConfigParameter {
    u32 FReqID;
    u32 FResID;
    u32 FFunctionalID;
    u8 FIsReqIDStandard;
    u8 FIsResIDStandard;
    u8 FIsFunctionalIDStandard;
    u8 FIdxChn;
    u8 FFilledByte;
    u8 FAtLeast8Bytes;
    u8 FIsFD;
    u8 FIsFDBRS;
    u8 FMaxDLCofFDFrame;
    u8 N_WFTmax;
    u16 FReserved01;
    u32 FSTMin;
    u16 FReserved02;
    u8 FUserDefinedTxSTMin;
    u8 FUserDefinedFCDelay;
    u32 FTxSTMin;
    u32 FFCDelayMs;
    u32 FBlockSize;
    u32 FMaxLength;
    u16 N_As;
    u16 N_Ar;
    u16 N_Bs;
    u16 N_Br;
    u16 N_Cs;
    u16 N_Cr;
}TDiagConfigParameter, * PDiagConfigParameter;

// Arg[0] AData
typedef void(__stdcall* TCANQueueEvent)(const PLIBCAN AData);
// Arg[0] AData
typedef void(__stdcall* TCANFDQueueEvent)(const PLIBCANFD AData);
// Arg[0] AData
typedef void(__stdcall* TFlexrayQueueEvent)(const PLIBFlexRay AData);
// Arg[0] ADeviceHandle
typedef void(__stdcall* THighResTimerCallback)(const size_t ADeviceHandle);
// Arg[0] AData
typedef void(__stdcall* TLINQueueEvent)(const PLIBLIN AData);
// Arg[0] AData
typedef void(__stdcall* TCANQueueEvent_Win32)(const PLIBCAN AData);
// Arg[0] AData
typedef void(__stdcall* TCANFDQueueEvent_Win32)(const PLIBCANFD AData);
// Arg[0] AData
typedef void(__stdcall* TLINQueueEvent_Win32)(const PLIBLIN AData);
// Arg[0] AData
typedef void(__stdcall* TFlexRayQueueEvent_Win32)(const PLIBFlexRay AData);
// Arg[0] ADevicehandle
typedef void(__stdcall* TTSCANConnectedCallback)(const size_t ADevicehandle);
// Arg[0] AData
typedef void(__stdcall* TCompressedDataProcessEvent)(const PLIBCompressedData AData);
// Arg[0] ADevicehandle
typedef void(__stdcall* TTSCANConnectedCallback_Win32)(const size_t ADevicehandle);
// Arg[0] ADeviceHandle
typedef void(__stdcall* THighResTimerCallback_Win32)(const size_t ADeviceHandle);
// Arg[0] ADeviceHandle
// Arg[1] AData
typedef void(__stdcall* TCANQueueEvent_WHandle)(const size_t ADeviceHandle, const PLIBCAN AData);
// Arg[0] ADeviceHandle
// Arg[1] AData
typedef void(__stdcall* TCANFDQueueEvent_WHandle)(const size_t ADeviceHandle, const PLIBCANFD AData);
// Arg[0] ADeviceHandle
// Arg[1] AData
typedef void(__stdcall* TLINQueueEvent_WHandle)(const size_t ADeviceHandle, const PLIBLIN AData);
// Arg[0] ADeviceHandle
// Arg[1] AData
typedef void(__stdcall* TFlexRayQueueEvent_WHandle)(const size_t ADeviceHandle, const PLIBFlexRay AData);
// Arg[0] AIdxChn
// Arg[1] ATimestamp
// Arg[2] APackCmd
// Arg[3] AParameter
// Arg[4] AParameterLength
// Arg[5] AData
// Arg[6] ADataLength
typedef void(__stdcall* TDatapackageProcessEvent)(const u8 AIdxChn, const u64 ATimestamp, const u16 APackCmd, const pu8 AParameter, const u16 AParameterLength, const pu8 AData, const s32 ADataLength);
// Arg[0] AIdxChn
// Arg[1] ATimestamp
// Arg[2] APackCmd
// Arg[3] AParameter
// Arg[4] AParameterLength
// Arg[5] AData
// Arg[6] ADataLength
typedef void(__stdcall* TDatapackageProcessEvent_Win32)(const u8 AIdxChn, const u64 ATimestamp, const u16 APackCmd, const pu8 AParameter, const u16 AParameterLength, const pu8 AData, const s32 ADataLength);
// Arg[0] ATpModuleIndex
// Arg[1] AChn
// Arg[2] ATimeStamp
// Arg[3] APayLoad
// Arg[4] ASize
// Arg[5] AError
typedef void(__stdcall* N_USData_TranslateCompleted_Recall)(const s32 ATpModuleIndex, const s32 AChn, const u64 ATimeStamp, const pu8 APayLoad, const u32 ASize, const ISO_TP_RESAULT AError);
#if defined ( __cplusplus )
extern  "C"
{
#endif
    TSAPI(void)initialize_lib_tscan(const bool AEnableFIFO, const bool AEnableErrorFrame, const bool AUseHWTime);

    TSAPI(void)initialize_lib_tscan_verbose(const bool AEnableFIFO, const bool AEnableErrorFrame, const bool AUseHWTime, const TTSHWType ATSType);

    TSAPI(void)finalize_lib_tscan();

    TSAPI(void)tscan_unregister_event_all_connected_pascal();

    TSAPI(void)tscan_unregister_event_all_disconnected_pascal();

    TSAPI(void)UnregisterHighresTimers();

    TSAPI(void)SetQueueEnabled(const size_t ADeviceHandle, const u32 AEnable);

    TSAPI(void)SetConnectionDiagnosticsEnabled(const size_t ADeviceHandle, const u32 AEnable);

    TSAPI(void)SetAutoConnect(const u32 AIsAutoConnect);

    TSAPI(void)tscan_clear_can_bus_statistic();

    TSAPI(void)tscan_set_auto_calc_bus_statistics(const bool Value);

    TSAPI(void)tsreplay_clear_channel_map(const size_t AHandle);

    TSAPI(void)tsdiag_can_delete_all();

    TSAPI(u32)tscan_scan_devices(const pu32 ADeviceCount);

    TSAPI(u32)tscan_scan_devices_ex(const pu32 ADeviceCount);

    TSAPI(u32)tscan_scan_devices_usbif(const pu32 ADeviceCount);

    TSAPI(u32)tscan_scan_devices_ethif(const pu32 ADeviceCount);

    TSAPI(u32)tscan_get_device_info(const u32 ADeviceIndex, const ppchar AFManufacturer, const ppchar AFProduct, const ppchar AFSerial);

    TSAPI(u32)tscan_get_device_info_ex(const u32 ADeviceIndex, const ppchar AFManufacturer, const ppchar AFProduct, const ppchar AFSerial);

    TSAPI(u32)tscan_get_device_info_detail(const u32 ADeviceIndex, const ppchar AManufacturer, const ppchar AProduct, const ppchar ASerial, const ps32 ADeviceType, const ppchar ADeviceName, const ps32 ACANChannelCount, const pbool AIsCANFD, const ps32 ALINChannelCount, const ps32 AFRChannelCount, const ps32 AEthernetChannelCount);

    TSAPI(u32)tscan_connect(const char* ADeviceSerial, const psize_t AHandle);

    TSAPI(u32)tscan_connect_v2(const char* ADeviceSerial, const psize_t AHandle, const u8 AEnableTSync);

    TSAPI(u32)tscan_connect_interface(const char* ADeviceSerial, const psize_t AHandle, const s32 AChannel);

    TSAPI(u32)tscan_disconnect_all_devices();

    TSAPI(u32)tscan_disconnect_by_serial(const char* ADeviceSerial);

    TSAPI(u32)tscan_disconnect_by_handle(const size_t ADeviceHandle);

    TSAPI(u32)tscan_disconnect_by_handle_interface(const size_t ADeviceHandle, const s32 AChannel);

    TSAPI(u32)tscan_is_connected(const size_t ADeviceHandle, const pbool AIsConnected);

    TSAPI(u32)tscan_get_can_channel_count(const size_t ADeviceHandle, const ps32 AChnCount, const pbool AIsFDCAN);

    TSAPI(u32)tscan_get_lin_channel_count(const size_t ADeviceHandle, const ps32 AChnCount);

    TSAPI(u32)tscan_get_flexray_channel_count(const size_t ADeviceHandle, const ps32 AChnCount);

    TSAPI(u32)tscan_get_device_type(const size_t ADeviceHandle, const ps32 ADeviceType, const ppchar ADeviceName);

    TSAPI(u32)tscan_get_error_description(const u32 ACode, const ppchar ADesc);

    TSAPI(u32)tscan_register_event_connected_pascal(const TTSCANConnectedCallback ACallback);

    TSAPI(u32)tscan_register_event_disconnected_pascal(const TTSCANConnectedCallback ACallback);

    TSAPI(u32)tscan_unregister_event_connected_pascal(const TTSCANConnectedCallback ACallback);

    TSAPI(u32)tscan_unregister_event_disconnected_pascal(const TTSCANConnectedCallback ACallback);

    TSAPI(u32)tscan_register_event_connected(const TTSCANConnectedCallback_Win32 ACallback);

    TSAPI(u32)tscan_register_event_disconnected(const TTSCANConnectedCallback_Win32 ACallback);

    TSAPI(u32)tscan_unregister_event_connected(const TTSCANConnectedCallback_Win32 ACallback);

    TSAPI(u32)tscan_unregister_event_disconnected(const TTSCANConnectedCallback_Win32 ACallback);

    TSAPI(u32)tscan_register_event_can(const size_t ADeviceHandle, const TCANQueueEvent_Win32 ACallback);

    TSAPI(u32)tscan_unregister_event_can(const size_t ADeviceHandle, const TCANQueueEvent_Win32 ACallback);

    TSAPI(u32)tscan_register_event_canfd(const size_t ADeviceHandle, const TCANFDQueueEvent_Win32 ACallback);

    TSAPI(u32)tscan_unregister_event_canfd(const size_t ADeviceHandle, const TCANFDQueueEvent_Win32 ACallback);

    TSAPI(u32)RegisterHighTimerRecvCallback_Win32(const THighResTimerCallback_Win32 ACallback);

    TSAPI(u32)UnregisterHighTimerRecvCallback_Win32(const THighResTimerCallback_Win32 ACallback);

    TSAPI(u32)tslin_register_event_lin(const size_t ADeviceHandle, const TLINQueueEvent_Win32 ACallback);

    TSAPI(u32)tslin_unregister_event_lin(const size_t ADeviceHandle, const TLINQueueEvent_Win32 ACallback);

    TSAPI(u32)tsflexray_register_event_flexray(const size_t ADeviceHandle, const TFlexRayQueueEvent_Win32 ACallback);

    TSAPI(u32)tsflexray_unregister_event_flexray(const size_t ADeviceHandle, const TFlexRayQueueEvent_Win32 ACallback);

    TSAPI(s32)tscan_register_pretx_event_can(const size_t ADeviceHandle, const TCANQueueEvent_Win32 AEvent);

    TSAPI(s32)tscan_unregister_pretx_event_can(const size_t ADeviceHandle, const TCANQueueEvent_Win32 AEvent);

    TSAPI(s32)tscan_register_pretx_event_canfd(const size_t ADeviceHandle, const TCANFDQueueEvent_Win32 AEvent);

    TSAPI(s32)tscan_unregister_pretx_event_canfd(const size_t ADeviceHandle, const TCANFDQueueEvent_Win32 AEvent);

    TSAPI(s32)tslin_register_pretx_event_lin(const size_t ADeviceHandle, const TLINQueueEvent_Win32 AEvent);

    TSAPI(s32)tslin_unregister_pretx_event_lin(const size_t ADeviceHandle, const TLINQueueEvent_Win32 AEvent);

    TSAPI(s32)tsflexray_register_pretx_event_flexray(const size_t ADeviceHandle, const TFlexRayQueueEvent_Win32 AEvent);

    TSAPI(s32)tsflexray_unregister_pretx_event_flexray(const size_t ADeviceHandle, const TFlexRayQueueEvent_Win32 AEvent);

    TSAPI(s32)tsflexray_unregister_pretx_events_flexray(const size_t ADeviceHandle);

    TSAPI(s32)tscan_unregister_pretx_events_can(const size_t ADeviceHandle);

    TSAPI(s32)tslin_unregister_pretx_events_lin(const size_t ADeviceHandle);

    TSAPI(s32)tscan_unregister_pretx_events_canfd(const size_t ADeviceHandle);

    TSAPI(s32)tscan_unregister_pretx_events_all(const size_t ADeviceHandle);

    TSAPI(u32)tscan_register_event_can_whandle(const size_t ADeviceHandle, const TCANQueueEvent_WHandle ACallback);

    TSAPI(u32)tscan_unregister_event_can_whandle(const size_t ADeviceHandle, const TCANQueueEvent_WHandle ACallback);

    TSAPI(u32)tscan_register_event_canfd_whandle(const size_t ADeviceHandle, const TCANFDQueueEvent_WHandle ACallback);

    TSAPI(u32)tscan_unregister_event_canfd_whandle(const size_t ADeviceHandle, const TCANFDQueueEvent_WHandle ACallback);

    TSAPI(u32)tslin_register_event_lin_whandle(const size_t ADeviceHandle, const TLINQueueEvent_WHandle ACallback);

    TSAPI(u32)tslin_unregister_event_lin_whandle(const size_t ADeviceHandle, const TLINQueueEvent_WHandle ACallback);

    TSAPI(u32)tsflexray_register_event_flexray_whandle(const size_t ADeviceHandle, const TFlexRayQueueEvent_WHandle ACallback);

    TSAPI(u32)tsflexray_unregister_event_flexray_whandle(const size_t ADeviceHandle, const TFlexRayQueueEvent_WHandle ACallback);

    TSAPI(s32)tscan_register_pretx_event_can_whandle(const size_t ADeviceHandle, const TCANQueueEvent_WHandle AEvent);

    TSAPI(s32)tscan_unregister_pretx_event_can_whandle(const size_t ADeviceHandle, const TCANQueueEvent_WHandle AEvent);

    TSAPI(s32)tscan_register_pretx_event_canfd_whandle(const size_t ADeviceHandle, const TCANFDQueueEvent_WHandle AEvent);

    TSAPI(s32)tscan_unregister_pretx_event_canfd_whandle(const size_t ADeviceHandle, const TCANFDQueueEvent_WHandle AEvent);

    TSAPI(s32)tslin_register_pretx_event_lin_whandle(const size_t ADeviceHandle, const TLINQueueEvent_WHandle AEvent);

    TSAPI(s32)tslin_unregister_pretx_event_lin_whandle(const size_t ADeviceHandle, const TLINQueueEvent_WHandle AEvent);

    TSAPI(s32)tsflexray_register_pretx_event_flexray_whandle(const size_t ADeviceHandle, const TFlexRayQueueEvent_WHandle AEvent);

    TSAPI(s32)tsflexray_unregister_pretx_event_flexray_whandle(const size_t ADeviceHandle, const TFlexRayQueueEvent_WHandle AEvent);

    TSAPI(s32)tsflexray_unregister_pretx_events_flexray_whandle(const size_t ADeviceHandle);

    TSAPI(s32)tscan_unregister_pretx_events_can_whandle(const size_t ADeviceHandle);

    TSAPI(s32)tslin_unregister_pretx_events_lin_whandle(const size_t ADeviceHandle);

    TSAPI(s32)tscan_unregister_pretx_events_canfd_whandle(const size_t ADeviceHandle);

    TSAPI(s32)tscan_unregister_pretx_events_all_whandle(const size_t ADeviceHandle);

    TSAPI(u32)tscan_register_event_canfd_pascal(const size_t ADeviceHandle, const TCANFDQueueEvent ACallback);

    TSAPI(u32)tscan_unregister_event_canfd_pascal(const size_t ADeviceHandle, const TCANFDQueueEvent ACallback);

    TSAPI(u32)tscan_unregister_event_all_canfd_pascal(const size_t ADeviceHandle);

    TSAPI(u32)tsflexray_register_event_flexray_pascal(const size_t ADeviceHandle, const TFlexrayQueueEvent ACallback);

    TSAPI(u32)tsflexray_unregister_event_flexray_pascal(const size_t ADeviceHandle, const TFlexrayQueueEvent ACallback);

    TSAPI(u32)tsflexray_unregister_event_all_flexray_pascal(const size_t ADeviceHandle);

    TSAPI(u32)tscan_register_event_can_pascal(const size_t ADeviceHandle, const TCANQueueEvent ACallback);

    TSAPI(u32)tscan_unregister_event_can_pascal(const size_t ADeviceHandle, const TCANQueueEvent ACallback);

    TSAPI(u32)tscan_unregister_event_all_can_pascal(const size_t ADeviceHandle);

    TSAPI(u32)tslin_register_event_lin_pascal(const size_t ADeviceHandle, const TLINQueueEvent ACallback);

    TSAPI(u32)tslin_unregister_event_lin_pascal(const size_t ADeviceHandle, const TLINQueueEvent ACallback);

    TSAPI(u32)tslin_unregister_event_all_lin_pascal(const size_t ADeviceHandle);

    TSAPI(u32)UnregisterAllRecvCallbacks();

    TSAPI(u32)tscan_transmit_can_sync(const size_t ADeviceHandle, const PLIBCAN ACAN, const u32 ATimeoutMS);

    TSAPI(u32)tscan_transmit_can_async(const size_t ADeviceHandle, const PLIBCAN ACAN);

    TSAPI(u32)tscan_transmit_can_sequence(const size_t ADeviceHandle, const PLIBCAN ACANSeq, const s32 ASize);

    TSAPI(u32)tscan_transmit_canfd_sync(const size_t ADeviceHandle, const PLIBCANFD ACANFD, const u32 ATimeoutMS);

    TSAPI(u32)tscan_transmit_canfd_async(const size_t ADeviceHandle, const PLIBCANFD ACANFD);

    TSAPI(u32)tscan_transmit_canfd_sequence(const size_t ADeviceHandle, const PLIBCANFD ACANFDSeq, const s32 ASize);

    TSAPI(u32)tscan_transmit_canfd_async_compress(const size_t ADeviceHandle, const PLIBCANFD ACANFD);

    TSAPI(u32)tslin_transmit_lin_sync(const size_t ADeviceHandle, const PLIBLIN ALIN, const u32 ATimeoutMS);

    TSAPI(u32)tslin_transmit_lin_async(const size_t ADeviceHandle, const PLIBLIN ALIN);

    TSAPI(u32)tslin_transmit_fastlin_async(const size_t ADeviceHandle, const PLIBLIN ALIN);

    TSAPI(u32)StopPendingOperations();

    TSAPI(u32)HighresTimerTrigger(const size_t ADeviceHandle);

    TSAPI(u32)RegisterHighresTimer(const THighResTimerCallback ACallback);

    TSAPI(u32)UnregisterHighresTimer(const THighResTimerCallback ACallback);

    TSAPI(u32)tscan_config_can_by_baudrate(const size_t ADeviceHandle, const u32 AChnIdx, const double ARateKbps, const u32 A120OhmConnected);

    TSAPI(u32)tscan_config_canfd_by_baudrate(const size_t ADeviceHandle, const s32 AChnIdx, const double AArbRateKbps, const double  ADataRateKbps, const TLIBCANFDControllerType AControllerType, const TLIBCANFDControllerMode AControllerMode, const s32 A120OhmConnected);

    TSAPI(u32)tscan_config_can_by_raw_parameters(const size_t ADeviceHandle, const u32 AChnIdx, const u32  ASEG1, const u32  ASEG2, const u32  APrescaler, const u32  ASJW, const u32  AListen, const u32  ASampTimes, const u32  A120OhmConnected, const double ARateKbps);

    TSAPI(u32)tscan_configure_can_regs(const size_t ADeviceHandle, const s32 AChnIdx, const float ABaudrateKbps, const u32 ASEG1, const u32  ASEG2, const u32  APrescaler, const u32  ASJW, const u32 AOnlyListen, const u32 A120OhmConnected);

    TSAPI(u32)tscan_configure_canfd_regs(const size_t ADeviceHandle, const s32 AChnIdx, const float AArbBaudrate, const u32 AArbSEG1, const u32  AArbSEG2, const u32  AArbPrescaler, const u32  AArbSJW, const float ADataBaudrate, const u32 ADataSEG1, const u32  ADataSEG2, const u32  ADataPrescaler, const u32  ADataSJW, const TLIBCANFDControllerType AControllerType, const TLIBCANFDControllerMode AControllerMode, const u32 A120OhmConnected);

    TSAPI(u32)tslin_config_baudrate(const size_t ADeviceHandle, const u32 AChnIdx, const double ARateKBps, const s32 AProtocol);

    TSAPI(u32)tslin_config_baudrate_verbose(const size_t ADeviceHandle, const u32 AChnIdx, const double ARateKBps, const s32 AProtocol, const bool AKeepLowLevelIDLEMode);

    TSAPI(u32)tslin_clear_schedule_tables(const size_t ADeviceHandle, const u32 AChnIdx);

    TSAPI(u32)tslin_switch_normal_schedule_table(const size_t ADeviceHandle, const u32 AChnIdx, const u8 ASchIndex);

    TSAPI(u32)tslin_stop_lin_channel(const size_t ADeviceHandle, const u32 AChnIdx);

    TSAPI(u32)tslin_start_lin_channel(const size_t ADeviceHandle, const u32 AChnIdx);

    TSAPI(u32)tslin_set_node_functiontype(const size_t ADeviceHandle, const u32 AChnIdx, const u8 AFunctionType);

    TSAPI(u32)tslin_switch_runtime_schedule_table(const size_t ADeviceHandle, const u32 AChnIdx);

    TSAPI(s32)tslin_switch_idle_schedule_table(const size_t ADeviceHandle, const s32 AChnIdx);

    TSAPI(s32)tslin_batch_set_schedule_start(const size_t ADeviceHandle, const s32 AChnIdx);

    TSAPI(s32)tslin_batch_add_schedule_frame(const size_t ADeviceHandle, const s32 AChnIdx, const PLIBLIN ALINData, const s32 ADelayMs);

    TSAPI(s32)tslin_batch_set_schedule_end(const size_t ADeviceHandle, const s32 AChnIdx);

    TSAPI(s32)tslin_active_frame_in_schedule_table(const size_t ADeviceHandle, const u32 AChnIdx, const u8 AID, const s32 AIndex);

    TSAPI(s32)tslin_deactive_frame_in_schedule_table(const size_t ADeviceHandle, const u32 AChnIdx, const u8 AID, const s32 AIndex);

    TSAPI(u32)tstp_lin_master_request(const size_t ADeviceHandle, const u32 AChnIdx, const u8 ANAD, const pu8 AData, const u16 ADataNum, const u32 ATimeoutMs);

    TSAPI(u32)tstp_lin_master_tranceive_sync(const u32 AChnIdx, const u8 AReqNAD, const pu8 AReqData, const s32 AReqDataNum, const pu8 AResNAD, const pu8 AResData, const ps32 AResDataNum, const u32 ATimeoutMS);

    TSAPI(u32)tstp_lin_master_request_intervalms(const size_t ADeviceHandle, const u32 AChnIdx, const u16 AData);

    TSAPI(u32)tstp_lin_reset(const size_t ADeviceHandle, const u32 AChnIdx);

    TSAPI(u32)tstp_lin_slave_response_intervalms(const size_t ADeviceHandle, const u32 AChnIdx, const u16 AData);

    TSAPI(s32)tstp_lin_tp_para_default(const size_t ADeviceHandle, const u32 AChnIdx, const u16 AReqIntervalMs, const u16 AResIntervalMs, const u16 AResRetryTime);

    TSAPI(s32)tstp_lin_tp_para_special(const size_t ADeviceHandle, const u32 AChnIdx, const u16 AReqIntervalMs, const u16 AResIntervalMs, const u16 AResRetryTime);

    TSAPI(s32)tsdiag_lin_read_data_by_identifier(const size_t ADeviceHandle, const u32 AChnIdx, const u8 ANAD, const u16 AId, const pu8 AResNAD, const pu8 AResData, const ps32 AResDataNum, const u32 ATimeoutMS);

    TSAPI(s32)tsdiag_lin_write_data_by_identifier(const size_t ADeviceHandle, const s32 AChnIdx, const u8 AReqNAD, const u16 AID, const pu8 AReqData, const s32 AReqDataNum, const pu8 AResNAD, const pu8 AResData, const ps32 AResDataNum, const u32 ATimeoutMS);

    TSAPI(s32)tsdiag_lin_session_control(const size_t ADeviceHandle, const u32 AChnIdx, const u8 ANAD, const u8 ASession, const u32 ATimeoutMS);

    TSAPI(s32)tsdiag_lin_fault_memory_read(const size_t ADeviceHandle, const u32 AChnIdx, const u8 ANAD, const u16 data_length, const pu8 AData, const u32 ATimeoutMS);

    TSAPI(s32)tsdiag_lin_fault_memory_clear(const size_t ADeviceHandle, const u32 AChnIdx, const u8 ANAD, const u16 data_length, const pu8 AData, const u32 ATimeoutMS);

    TSAPI(u32)SecurityWriteStringSync(const size_t ADeviceHandle, const u8 AIndex, const char* AString, const u8 AStringLength, const s32 ATimeoutMS);

    TSAPI(u32)SecurityWriteStringASync(const size_t ADeviceHandle, const u8 AIndex, const char* AString, const u8 AStringLength);

    TSAPI(u32)SecurityReadStringSync(const size_t ADeviceHandle, const u8 AIndex, const char* AString, const pu8 AStringLength, const s32 ATimeoutMS);

    TSAPI(u32)SecurityUpdateKeySync(const size_t ADeviceHandle, const char* AOldKey, const u8 AOldKeyLength, const char* ANewKey, const u8 ANewKeyLength, const s32 ATimeoutMS);

    TSAPI(u32)SecurityUnlockWriteAuthoritySync(const size_t ADeviceHandle, const char* AKey, const u8 AKeyLength, const s32 ATimeoutMS);

    TSAPI(u32)SecurityGetKeyStateSync(const size_t ADeviceHandle, const s32 ATimeoutMS);

    TSAPI(u32)SecurityEncryptStringSync(const size_t ADeviceHandle, const u8 AIndex, const char* AString, const pu8 AStringLength, const s32 ATimeoutMS);

    TSAPI(u32)SecurityDecryptStringSync(const size_t ADeviceHandle, const u8 AIndex, const char* AString, const pu8 AStringLength, const s32 ATimeoutMS);

    TSAPI(u32)tscan_start_wait_can_message(const size_t ADeviceHandle, const PLIBCAN ACAN);

    TSAPI(u32)tscan_start_wait_canfd_message(const size_t ADeviceHandle, const PLIBCANFD ACANFD);

    TSAPI(u32)tscan_start_wait_lin_message(const size_t ADeviceHandle, const PLIBLIN ALIN);

    TSAPI(u32)tscan_wait_can_message_sync(const PLIBCAN ACAN, const u32 ATimeoutMS);

    TSAPI(u32)tscan_wait_canfd_message_sync(const PLIBCANFD ACAN, const u32 ATimeoutMS);

    TSAPI(u32)tscan_wait_lin_message_sync(const PLIBLIN ALIN, const u32 ATimeoutMS);

    TSAPI(u32)tsio_get_levels(const size_t ADeviceHandle, const pu64 ADigiLevels, const pu16 AAnalogLevels8, const pu64 ATimestampUs);

    TSAPI(u32)tsio_get_level(const size_t ADeviceHandle, const s32 AChnIdx, const pu32 ADigiLevel, const pu64 ATimestampUs);

    TSAPI(u32)tsio_set_levels(const size_t ADeviceHandle, const u64 ADigiLevels, const u64 ADigiMasks_1_valid);

    TSAPI(u32)tsio_set_level(const size_t ADeviceHandle, const s32 AChnIdx, const u32 ADigiLevel);

    TSAPI(u32)ReplayOnlineStartBLF(const size_t ADeviceHandle, const char* ABLFFileName, const u32 APlayTX, const u32 APlayRX);

    TSAPI(u32)ReplayOnlinePause();

    TSAPI(u32)ReplayOnlineResume();

    TSAPI(u32)ReplayOnlineStop();

    TSAPI(double)tscan_get_bus_status(const size_t ADeviceHandle, const s32 AChnBase0, const s32 AIndex);

    TSAPI(s32)tscan_get_fps(const size_t ADeviceHandle, const s32 AChnBase0, const s32 AIdentifier);

    TSAPI(bool)tscan_get_auto_calc_bus_statistics();

    TSAPI(u32)tscan_add_cyclic_msg_can(const size_t ADeviceHandle, const PLIBCAN ACAN, const float APeriodMS);

    TSAPI(u32)tscan_delete_cyclic_msg_can(const size_t ADeviceHandle, const PLIBCAN ACAN);

    TSAPI(u32)tscan_add_cyclic_msg_canfd(const size_t ADeviceHandle, const PLIBCANFD ACANFD, const float APeriodMS);

    TSAPI(u32)tscan_add_cyclic_msg_canfd_wo_compensation(const size_t ADeviceHandle, const PLIBCANFD ACANFD, const float APeriodMS);

    TSAPI(u32)tscan_add_cyclic_msg_can_wo_compensation(const size_t ADeviceHandle, const PLIBCAN ACAN, const float APeriodMS);

    TSAPI(u32)tscan_delete_cyclic_msg_canfd(const size_t ADeviceHandle, const PLIBCANFD ACANFD);

    TSAPI(s32)tsfifo_add_can_canfd_pass_filter(const size_t ADeviceHandle, const s32 AIdxChn, const s32 AIdentifier, const bool AIsStd);

    TSAPI(s32)tsfifo_add_lin_pass_filter(const size_t ADeviceHandle, const s32 AIdxChn, const s32 AIdentifier);

    TSAPI(s32)tsfifo_delete_can_canfd_pass_filter(const size_t ADeviceHandle, const s32 AIdxChn, const s32 AIdentifier);

    TSAPI(s32)tsfifo_delete_lin_pass_filter(const size_t ADeviceHandle, const s32 AIdxChn, const s32 AIdentifier);

    TSAPI(s32)tsfifo_add_flexray_pass_filter(const size_t ADeviceHandle, const s32 AIdxChn, const u16 ASlotID, const u8 ACycleBase, const u8 ARep);

    TSAPI(s32)tsfifo_delete_flexray_pass_filter(const size_t ADeviceHandle, const s32 AIdxChn, const u16 ASlotID, const u8 ACycleBase, const u8 ARep);

    TSAPI(u32)tsfifo_receive_can_msgs(const size_t ADeviceHandle, const PLIBCAN ACANBuffers, const ps32 ACANBufferSize, const u8 AChn, const u8 ARxTx);

    TSAPI(u32)tsfifo_clear_can_receive_buffers(const size_t ADeviceHandle, const s32 AIdxChn);

    TSAPI(u32)tsfifo_receive_canfd_msgs(const size_t ADeviceHandle, const PLIBCANFD ACANBuffers, const ps32 ACANBufferSize, const u8 AChn, const u8 ARxTx);

    TSAPI(u32)tsfifo_clear_canfd_receive_buffers(const size_t ADeviceHandle, const s32 AIdxChn);

    TSAPI(u32)tsfifo_receive_flexray_msgs(const size_t ADeviceHandle, const PLIBFlexRay ADataBuffers, const ps32 ADataBufferSize, const u8 AChn, const u8 ARxTx);

    TSAPI(u32)tsfifo_clear_flexray_receive_buffers(const size_t ADeviceHandle, const s32 AIdxChn);

    TSAPI(u32)tsfifo_receive_lin_msgs(const size_t ADeviceHandle, const PLIBLIN ALINBuffers, const ps32 ALINBufferSize, const u8 AChn, const u8 ARxTx);

    TSAPI(u32)tsfifo_clear_lin_receive_buffers(const size_t ADeviceHandle, const s32 AIdxChn);

    TSAPI(u32)tsfifo_receive_fastlin_msgs(const size_t ADeviceHandle, const PLIBLIN ALINBuffers, const ps32 ALINBufferSize, const u8 AChn, const u8 ARxTx);

    TSAPI(u32)tsfifo_clear_fastlin_receive_buffers(const size_t ADeviceHandle, const s32 AIdxChn);

    TSAPI(s32)tsfifo_read_can_buffer_frame_count(const size_t ADeviceHandle, const s32 AIdxChn, s32* ACount);

    TSAPI(s32)tsfifo_read_can_tx_buffer_frame_count(const size_t ADeviceHandle, const s32 AIdxChn, s32* ACount);

    TSAPI(s32)tsfifo_read_can_rx_buffer_frame_count(const size_t ADeviceHandle, const s32 AIdxChn, s32* ACount);

    TSAPI(s32)tsfifo_read_canfd_buffer_frame_count(const size_t ADeviceHandle, const s32 AIdxChn, s32* ACount);

    TSAPI(s32)tsfifo_read_canfd_tx_buffer_frame_count(const size_t ADeviceHandle, const s32 AIdxChn, s32* ACount);

    TSAPI(s32)tsfifo_read_canfd_rx_buffer_frame_count(const size_t ADeviceHandle, const s32 AIdxChn, s32* ACount);

    TSAPI(s32)tsfifo_read_lin_buffer_frame_count(const size_t ADeviceHandle, const s32 AIdxChn, s32* ACount);

    TSAPI(s32)tsfifo_read_lin_tx_buffer_frame_count(const size_t ADeviceHandle, const s32 AIdxChn, s32* ACount);

    TSAPI(s32)tsfifo_read_lin_rx_buffer_frame_count(const size_t ADeviceHandle, const s32 AIdxChn, s32* ACount);

    TSAPI(s32)tsfifo_read_flexray_buffer_frame_count(const size_t ADeviceHandle, const s32 AIdxChn, s32* ACount);

    TSAPI(s32)tsfifo_read_flexray_tx_buffer_frame_count(const size_t ADeviceHandle, const s32 AIdxChn, s32* ACount);

    TSAPI(s32)tsfifo_read_flexray_rx_buffer_frame_count(const size_t ADeviceHandle, const s32 AIdxChn, s32* ACount);

    TSAPI(s32)tsfifo_read_fastlin_buffer_frame_count(const size_t ADeviceHandle, const s32 AIdxChn, s32* ACount);

    TSAPI(s32)tsfifo_read_fastlin_tx_buffer_frame_count(const size_t ADeviceHandle, const s32 AIdxChn, s32* ACount);

    TSAPI(s32)tsfifo_read_fastlin_rx_buffer_frame_count(const size_t ADeviceHandle, const s32 AIdxChn, s32* ACount);

    TSAPI(u32)tscan_register_event_fastlin(const size_t ADeviceHandle, const TLINQueueEvent_Win32 ACallback);

    TSAPI(u32)tscan_unregister_event_fastlin(const size_t ADeviceHandle, const TLINQueueEvent_Win32 ACallback);

    TSAPI(u32)tsflexray_set_controller_frametrigger(const size_t ADeviceHandle, const s32 ANodeIndex, const PLIBFlexray_controller_config AControllerConfig, const ps32 AFrameLengthArray, const s32 AFrameNum, const PLIBTrigger_def AFrameTrigger, const s32 AFrameTriggerNum, const s32 ATimeoutMs);

    TSAPI(u32)tsflexray_set_controller(const size_t ADeviceHandle, const s32 ANodeIndex, const PLIBFlexray_controller_config AControllerConfig, const s32 ATimeoutMs);

    TSAPI(u32)tsflexray_set_frametrigger(const size_t ADeviceHandle, const s32 ANodeIndex, const ps32 AFrameLengthArray, const s32 AFrameNum, const PLIBTrigger_def AFrameTrigger, const s32 AFrameTriggerNum, const s32 ATimeoutMs);

    TSAPI(u32)tsflexray_cmdreq(const size_t ADeviceHandle, const s32 AChnIdx, const s32 Action, const pu8 AWriteBuffer, const s32 AWriteBufferSize, const pu8 AReadBuffer, const ps32 AReadBufferSize, const s32 ATimeoutMs);

    TSAPI(u32)tsflexray_start_net(const size_t ADeviceHandle, const s32 ANodeIndex, const s32 ATimeoutMs);

    TSAPI(u32)tsflexray_stop_net(const size_t ADeviceHandle, const s32 ANodeIndex, const s32 ATimeoutMs);

    TSAPI(u32)tsflexray_transmit_sync(const size_t ADeviceHandle, const PLIBFlexRay AData, const s32 ATimeoutMs);

    TSAPI(u32)tsflexray_transmit_async(const size_t ADeviceHandle, const PLIBFlexRay AData);

    TSAPI(u32)tsethernet_config_sync(const size_t ADeviceHandle, const s32 AIdxChn, const PLIBEth_CMD_config AConfig, const s32 ATimeoutMs);

    TSAPI(u32)tsethernet_transmit_async(const size_t ADeviceHandle, const PLibEthernetHeader AEthernetHeader);

    TSAPI(u32)tsccp_apply_write_data_package(const size_t ADeviceHandle, const u32 AMasterCRO, const u32 ASlaveDTO, const u8 AChn, const s32 ATimeoutMS);

    TSAPI(u32)tsccp_write_data_package(const size_t ADeviceHandle, const u8 AChn, const u8 AIsStd, const u8 isFirstPackage, const u8 ACTRNum, const u32 AMasterCRO, const u32 ASlaveDTO, const pu8 pData, const u16 dataCnt, const s32 ATimeoutMS);

    TSAPI(u32)tsccp_check_program_status(const size_t ADeviceHandle, const u32 AMasterCRO, const u32 ASlaveDTO, const pu8 AProgramState, const ps32 AProgramCnt, const s32 ATimeoutMS);

    TSAPI(u32)tsccp_apply_read_data_package(const size_t ADeviceHandle, const u32 AMasterCRO, const u32 ASlaveDTO, const u8 AChn, const s32 ATimeoutMS);

    TSAPI(u32)tsccp_read_data_package(const size_t ADeviceHandle, const u8 AChn, const u8 AIsStd, const u8 isFirstPackage, const u8 ACTRNum, const u32 AMasterCRO, const u32 ASlaveDTO, const pu8 pData, const u16 dataCnt, const s32 ATimeoutMS);

    TSAPI(u32)tsreplay_start_blf(const size_t ADeviceHandle, const char* ABlfFilePath, const s32 ATriggerByHardware, const u64 AStartUs, const u64 AEndUs);

    TSAPI(u32)tsreplay_stop(const size_t ADeviceHandle);

    TSAPI(u32)tsreplay_add_channel_map(const size_t AHandle, const s32 ALogicChannel, const s32 AHardwareChannel);

    TSAPI(s32)tscan_write_customer_Info(const size_t ADeviceHandle, const char* ACustomerInfo, const s32 AInfoSize, const s32 ATimeoutMS);

    TSAPI(s32)tscan_read_customer_Info(const size_t ADeviceHandle, const ppchar ACustomerInfo, const s32 ATimeoutMS);

    TSAPI(s32)tscan_add_precise_cyclic_message(const size_t ADeviceHandle, const s32 AIdentifier, const u8 AChn, const u8 AIsExt, const float APeriodMS, const s32 ATimeoutMS);

    TSAPI(s32)tscan_delete_precise_cyclic_message(const size_t ADeviceHandle, const s32 AIdentifier, const u8 AChn, const u8 AIsExt, const s32 ATimeoutMS);

    TSAPI(s32)tslog_logger_delete_file(const size_t ADeviceHandle, const s32 AFileIndex, const s32 ATimeoutMS);

    TSAPI(s32)tslog_logger_start_export_blf_file(const size_t ADeviceHandle, const s32 AFileIndex, const char* ABlfFileName, const u64 AStartTimeUs, const s32 AMaxSize, const pdouble AProgress, const u16 AYear, const u16  AMonth, const u16  ADay, const u16  AHour, const u16  AMinute, const u16  ASecond, const u16  AMinisecond, const s32 AFileSize, const s32 AHighPressed, const s32 ASubFileWithTimeStamp, const s32 ATimeoutMS);

    TSAPI(s32)tslog_logger_abort_export_blf_file(const size_t ADeviceHandle, const s32 ATimeoutMS);

    TSAPI(s32)tslog_logger_start_online_replay(const size_t ADeviceHandle, const s32 AFileIndex, const u64 AStartTimeUs, const s32 AMaxSize, const s32 ATimeoutMS);

    TSAPI(s32)tslog_logger_start_offline_replay(const size_t ADeviceHandle, const s32 AFileIndex, const u64 AStartTimeUs, const s32 AMaxSize, const s32 ATimeoutMS);

    TSAPI(s32)tslog_logger_stop_replay(const size_t ADeviceHandle, const s32 ATimeoutMS);

    TSAPI(s32)tslog_logger_set_logger_mode(const size_t ADeviceHandle, const u8 AMode, const s32 ATimeoutMS);

    TSAPI(s32)tslog_logger_register_export_data_event(const size_t ADeviceHandle, const TCompressedDataProcessEvent ACallback);

    TSAPI(s32)tslog_logger_unregister_export_data_event(const size_t ADeviceHandle, const TCompressedDataProcessEvent ACallback);

    TSAPI(s32)tslog_logger_clear_export_data_event(const size_t ADeviceHandle);

    TSAPI(s32)tslog_logger_enable_gps_module_verbose(const size_t ADeviceHandle, const s32 AEnabled, const s32 AOnlySyncSystemTime, const s32 ATimeoutMS);

    TSAPI(s32)tslog_logger_reset_gps_module(const size_t ADeviceHandle, const s32 AInitBaudrate, const s32 ATargetBaudrate, const s32 ASampleHz, const s32 ATimeoutMS);

    TSAPI(s32)tsapp_get_gps_data_async(const size_t ADeviceHandle, const PLIBGPSData AGPSData);

    TSAPI(s32)tsapp_update_rtc_sync(const size_t ADeviceHandle, const u8 AYear, const u8  AMonth, const u8  ADay, const u8  AHour, const u8  AMinute, const u8  ASecond, const u8 AWeek, const u8 ASyncTimeBaseFromGPS, const s32 ATimeoutMS);

    TSAPI(s32)tsapp_get_rtc_sync(const size_t ADeviceHandle, const pu8 AYear, const pu8  AMonth, const pu8  ADay, const pu8  AHour, const pu8  AMinute, const pu8  ASecond, const pu32 AOffSetMiniSeconds, const pu8 AWeek, const pu8 ASyncTimeBaseFromGPS, const s32 ATimeoutMS);

    TSAPI(s32)tsdiag_can_create(const ps32 pDiagModuleIndex, const u32 AChnIndex, const u8 ASupportFDCAN, const u8 AMaxDLC, const u32 ARequestID, const bool ARequestIDIsStd, const u32 AResponseID, const bool AResponseIDIsStd, const u32 AFunctionID, const bool AFunctionIDIsStd);

    TSAPI(s32)tsdiag_can_delete(const s32 ADiagModuleIndex);

    TSAPI(s32)tsdiag_can_attach_to_tscan_tool(const s32 ADiagModuleIndex, const size_t ACANToolHandle);

    TSAPI(u32)tsdiag_can_enable_hw_diagnostic(const s32 ADiagModuleIndex, const s32 AEnabled, const s32 ATimeoutMs);

    TSAPI(s32)tsdiag_can_config_hw_parameter(const s32 ADiagModuleIndex, const PDiagConfigParameter AParameter, const s32 ASize, const s32 ATimeoutMs);

    TSAPI(s32)tstp_can_send_functional(const s32 ADiagModuleIndex, const pu8 AReqArray, const s32 AReqArraySize);

    TSAPI(s32)tstp_can_send_request(const s32 ADiagModuleIndex, const pu8 AReqArray, const s32 AReqArraySize);

    TSAPI(s32)tstp_can_request_and_get_response(const s32 ADiagModuleIndex, const pu8 AReqArray, const s32 AReqArraySize, const pu8 AReturnArray, const ps32 AReturnArraySize);

    TSAPI(s32)tstp_can_register_tx_completed_recall(const s32 ADiagModuleIndex, const N_USData_TranslateCompleted_Recall ATxcompleted);

    TSAPI(s32)tstp_can_register_rx_completed_recall(const s32 ADiagModuleIndex, const N_USData_TranslateCompleted_Recall ARxcompleted);

    TSAPI(s32)tstp_can_unregister_tx_completed_recall(const s32 ADiagModuleIndex);

    TSAPI(s32)tstp_can_unregister_rx_completed_recall(const s32 ADiagModuleIndex);

    TSAPI(s32)tsdiag_can_session_control(const s32 ADiagModuleIndex, const u8 ASubSession);

    TSAPI(s32)tsdiag_can_routine_control(const s32 ADiagModuleIndex, const u8 AARoutineControlType, const u16 ARoutintID);

    TSAPI(s32)tsdiag_can_communication_control(const s32 ADiagModuleIndex, const u8 AControlType);

    TSAPI(s32)tsdiag_can_security_access_request_seed(const s32 ADiagModuleIndex, const s32 ALevel, const pu8 ARecSeed, const ps32 ARecSeedSize);

    TSAPI(s32)tsdiag_can_security_access_send_key(const s32 ADiagModuleIndex, const s32 ALevel, const pu8 ASeed, const s32 ASeedSize);

    TSAPI(s32)tsdiag_can_request_download(const s32 ADiagModuleIndex, const u32 AMemAddr, const u32 AMemSize);

    TSAPI(s32)tsdiag_can_request_upload(const s32 ADiagModuleIndex, const u32 AMemAddr, const u32 AMemSize);

    TSAPI(s32)tsdiag_can_transfer_data(const s32 ADiagModuleIndex, const pu8 ASourceDatas, const s32 ASize, const s32 AReqCase);

    TSAPI(s32)tsdiag_can_request_transfer_exit(const s32 ADiagModuleIndex);

    TSAPI(s32)tsdiag_can_write_data_by_identifier(const s32 ADiagModuleIndex, const u16 ADataIdentifier, const pu8 AWriteData, const s32 AWriteDataSize);

    TSAPI(s32)tsdiag_can_read_data_by_identifier(const s32 ADiagModuleIndex, const u16 ADataIdentifier, const pu8 AReturnArray, const ps32 AReturnArraySize);

    TSAPI(s32)tsapp_transmit_datapackage_async_candev(const size_t ADeviceHandle, const s32 AChnIdx, const u16 APackCmd, const pu8 AParameter, const u16 AParameterLength, const pu8 AData, const s32 ALength);

    TSAPI(s32)tsapp_transmit_datapackage_async_lindev(const size_t ADeviceHandle, const s32 AChnIdx, const u16 APackCmd, const pu8 AParameter, const u16 AParameterLength, const pu8 AData, const s32 ALength);

    TSAPI(s32)tsapp_transmit_datapackage_async_frdev(const size_t ADeviceHandle, const s32 AChnIdx, const u16 APackCmd, const pu8 AParameter, const u16 AParameterLength, const pu8 AData, const s32 ALength);

    TSAPI(s32)tsapp_transmit_datapackage_async_ethdev(const size_t ADeviceHandle, const s32 AChnIdx, const u16 APackCmd, const pu8 AParameter, const u16 AParameterLength, const pu8 AData, const s32 ALength);

    TSAPI(s32)tsapp_register_data_package_event(const size_t ADeviceHandle, const TLinkedDataChnType ALinkedDataChnType, const u16 APackCMD, const TDatapackageProcessEvent ACallback);

    TSAPI(s32)tsapp_unregister_data_package_event(const size_t ADeviceHandle, const TLinkedDataChnType ALinkedDataChnType, const u16 APackCMD, const TDatapackageProcessEvent ACallback);

    TSAPI(s32)tsapp_unregister_data_package_events(const size_t ADeviceHandle, const TLinkedDataChnType ALinkedDataChnType, const u16 APackCMD);


#if defined ( __cplusplus )
}
#endif
#pragma pack(pop)
#endif
