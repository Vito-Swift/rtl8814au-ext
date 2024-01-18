## How to do unicast rate control?

Realtek driver provides per-sta rate control via *proc* debug interface. To enable proc debug mode, set `CONFIG_PROC_DEBUG` to y in the Makefile
```Makefile
# enable /proc/net/rtlxxxx/ debug interfaces
CONFIG_PROC_DEBUG = y
```

Then, uncomment `CONFIG_PHYDM_FW_FIXRATE` in include/drv_conf.h
```c
#define CONFIG_PHYDM_FW_FIXRATE	
```

Rebuild the driver and bring up the module. The driver will create a `/proc/net/rtl8814au/<wiface_name>/` directory for controlling firmware and reading debug information. 

FD `/proc/net/rtl8814au/<wiface_name>/rate_ctl` is used to interface with the `proc_set_rate_ctl(struct file *file, const char __user *buffer, size_t count, loff_t *pos, void *data)` function. Command format:
```shell
$ echo <rate_idx> <bw_idx> <macid> > /proc/.../rate_ctl
```

- rate idx: 
  ```c
  static const enum MGN_RATE bcast_rate_map[] = 
	{MGN_1M,MGN_2M,MGN_5_5M,MGN_6M,MGN_9M,MGN_11M,MGN_12M,MGN_18M,MGN_24M,MGN_36M,
    MGN_48M,MGN_54M,MGN_MCS32,MGN_MCS0,MGN_MCS1,MGN_MCS2,MGN_MCS3,MGN_MCS4,MGN_MCS5,MGN_MCS6,
    MGN_MCS7,MGN_MCS8,MGN_MCS9,MGN_MCS10,MGN_MCS11,MGN_MCS12,MGN_MCS13,MGN_MCS14,MGN_MCS15,MGN_MCS16,
    MGN_MCS17,MGN_MCS18,MGN_MCS19,MGN_MCS20,MGN_MCS21,MGN_MCS22,MGN_MCS23,MGN_MCS24,MGN_MCS25,MGN_MCS26,
    MGN_MCS27,MGN_MCS28,MGN_MCS29,MGN_MCS30,MGN_MCS31,MGN_VHT1SS_MCS0,MGN_VHT1SS_MCS1,MGN_VHT1SS_MCS2,MGN_VHT1SS_MCS3,MGN_VHT1SS_MCS4,
	MGN_VHT1SS_MCS5,MGN_VHT1SS_MCS6,MGN_VHT1SS_MCS7,MGN_VHT1SS_MCS8,MGN_VHT1SS_MCS9,MGN_VHT2SS_MCS0,MGN_VHT2SS_MCS1,MGN_VHT2SS_MCS2,MGN_VHT2SS_MCS3,MGN_VHT2SS_MCS4,
    MGN_VHT2SS_MCS5,MGN_VHT2SS_MCS6,MGN_VHT2SS_MCS7,MGN_VHT2SS_MCS8,MGN_VHT2SS_MCS9,MGN_VHT3SS_MCS0,MGN_VHT3SS_MCS1,MGN_VHT3SS_MCS2,MGN_VHT3SS_MCS3,MGN_VHT3SS_MCS4,
    MGN_VHT3SS_MCS5,MGN_VHT3SS_MCS6,MGN_VHT3SS_MCS7,MGN_VHT3SS_MCS8,MGN_VHT3SS_MCS9,MGN_VHT4SS_MCS0,MGN_VHT4SS_MCS1,MGN_VHT4SS_MCS2,MGN_VHT4SS_MCS3,MGN_VHT4SS_MCS4,
    MGN_VHT4SS_MCS5,MGN_VHT4SS_MCS6,MGN_VHT4SS_MCS7,MGN_VHT4SS_MCS8,MGN_VHT4SS_MCS9,};
  ```
- bw (bandwidth) idx:
  ```c
  const char *const _ch_width_str[CHANNEL_WIDTH_MAX] = {
	"20MHz",
	"40MHz",
	"80MHz",
	"160MHz",
	"80_80MHz",
	"5MHz",
	"10MHz",
  };
  ```
- macid: `cat /proc/net/rtl8814au/<wiface_name>/sta_tp_info`
  ```shell
  $ cat /proc/net/rtl8814au/wlxe84e068fdc33/sta_tp_info

  ====== mac_id : 0 [e8:4e:06:9c:bd:94] ======
  tx_rate : MCS12(L)  rx_rate : OFDM_6M, rx_rate_bmc : CCK_1M, rssi : 49 %
  RTW: [TP] Tx : 0(Kbps) Rx : 0(Kbps) Total : 0(Kbps)
  RTW: [Smooth TP] Tx : 0(Kbps) Rx : 0(Kbps) Total : 0(Kbps)
  ```
