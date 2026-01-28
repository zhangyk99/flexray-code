#include "tsdev_usr_module_private.h"

static tsdev_err_t tsdev_start(tsdev_handle_t handle);
static tsdev_err_t tsdev_cycle_entry(tsdev_handle_t handle, uint64_t time_us_now);
static tsdev_err_t tsdev_stop(tsdev_handle_t handle);

uint32_t test_var0;
double test_var1;
struct tsdev_cal_sig_def_t sig_def0[] = {
	{
		.name = "test_var0"
		,.sig_type = tsdev_sig_u32
		,.cov_type = tsdev_sig_cov_linear
		,.config = TSDEV_CAL_SIG_CFG_SIGNAL
		,.dim_cnt = 0
		,.default_val = {.u32_val = 0}
		,.invaild_val = {.u32_val = 0}
		,.max_val = {.u32_val = 20000}
		,.min_val = {.u32_val = 0}
		,.addr_u = {.ptr = &test_var0}
		,.addr_offset = 0  //set to any value
		,.cov = {.linear = {.a = 1.0, .b = 0}}
	}
	,{
		.name = "test_var1"
		,.sig_type = tsdev_sig_float64
		,.cov_type = tsdev_sig_cov_none
		,.config = TSDEV_CAL_SIG_CFG_SIGNAL
		,.dim_cnt = 0
		,.default_val = {.f64_val = 0}
		,.invaild_val = {.f64_val = -1}
		,.max_val = {.f64_val = 20000}
		,.min_val = {.f64_val = 0}
		,.addr_u = {.ptr = &test_var1}
		,.addr_offset = 0  //set to any value
	}
};
static const struct tsdev_usr_module_core_desc tsdev_usr_module_core_descs[] =
{
	{
		.name = "cycle 0"
		, .sch_prescaler = 1
		, .callbacks = {
			.tsdev_start = tsdev_start
			,.tsdev_cycle_entry = tsdev_cycle_entry
			,.tsdev_stop = tsdev_stop
			}
		, .sig_cnt = sizeof(sig_def0) / sizeof(sig_def0[0])
		, .sig_defs = sig_def0
	}
};

const struct tsdev_usr_module_config tsdev_usr_module_config =
{
	.core_cnt = sizeof(tsdev_usr_module_core_descs) / sizeof(tsdev_usr_module_core_descs[0])
	, .cores = tsdev_usr_module_core_descs
};
tsdev_handle_t dev_handle = NULL;
uint16_t dev_cnt = 0;
struct tsdev_info_t dev_info;
static tsdev_err_t tsdev_start(tsdev_handle_t handle)
{
	test_var0 = 0;
	test_var1 = 0.0;
	if (host_data_size > 0)
	{
		uint8_t* ptr = host_data_ptr;
		ptr[0] = 0;
		ptr[host_data_size - 1] = 0;
	}

	tsdev_api_log_out(handle, tsdev_log_trace, "tsdev on_start\r\n");
	test_var0 = 0;
	dev_cnt = 0;
	tsdev_api_get_device_cnt(handle, &dev_cnt);
	if (dev_cnt > 0)
	{
		tsdev_api_get_device_handle(handle, 0, &dev_handle, &dev_info);

		struct tsdev_TPC_FDHardwareConf cfg = { 0 };
		cfg.FAutoReportGrp = 0; // 0;// 43;         // 1:default = 0xFF, masked status see THW_GetHWConf
		cfg.FStoreInNVM = 0;    // STORE_IN_NVM, RESET_CHANNEL_MASK Etc
		// TCANFDCoreConf FCANFDConf; // 14:fd core configuration
		cfg.FIdx = 1;            //     can fd channel index, 0xFF = no set
		cfg.FChannelTypeIdx = 0; // 0;//3;   //     channel type index
		cfg.FPrescalerArb = 1;   // 1;//0;     //     can clock prescaler
		cfg.FBTR0Arb = 63;       //     can btr0 register
		cfg.FBTR1Arb = 16;       //     can btr1 register
		cfg.FSJWArb = 15;        // 16;//15;           //     can sync. jump width
		cfg.FPrescalerData = 1;  // 1;// 0;    //     can clock prescaler
		cfg.FBTR0Data = 15;      // 15;         //     can btr0 register
		cfg.FBTR1Data = 4;       //     can btr1 register
		cfg.FSJWData = 3;        // 4;//3;          //     can sync. jump width
		cfg.F120OhmRes = 1;      //     0: do not install; 1: install
		cfg.FControllerType = 1; //     0: Classic CAN; 1: ISO CAN; 2: Non-ISO CAN
		cfg.FControllerMode = 0; //     0: Normal; 1: No ACK; 2: restricted
		cfg.config = 1;          //    bit0: FAutoRecoverOnErr 0: do not recover; 1: auto recover
		for (int i = 0; i < 2; ++i)
		{
			cfg.FIdx = i;
			tsdev_api_can_configure_regs_async(dev_handle, &cfg);
		}
	}
	return tsdev_err_ok;
}
static tsdev_err_t tsdev_cycle_entry(tsdev_handle_t handle, uint64_t time_us_now)
{
	++test_var0;
	test_var1 += 15;
	if (host_data_size > 0)
	{
		uint8_t* ptr = host_data_ptr;
		++ptr[0];
		++ptr[host_data_size - 1];
	}

	if (dev_cnt > 0)
	{
		struct tsdev_canfd_tx_t frame;
		frame.FIdentifier = 0x100;
		frame.FIdxChn = 0;
		frame.FCtrlGrp = 1;
		frame.FFDProperties = 1;
		frame.FDLC = 8;
		for (int j = 0; j < 1; ++j)
		{
			frame.FData[j] = test_var0 + j;
		}
		if (host_data_size > 0)
		{
			uint8_t* ptr = host_data_ptr;
			frame.FData[3] = ptr[3];
		}
		tsdev_api_can_transmit_async(dev_handle, &frame);
	}
	return tsdev_err_ok;
}
static tsdev_err_t tsdev_stop(tsdev_handle_t handle)
{
	tsdev_api_log_out(handle, tsdev_log_trace, "tsdev on_stop\r\n");
	return tsdev_err_ok;
}




#define WIN32_LEAN_AND_MEAN            
#include <windows.h>
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

