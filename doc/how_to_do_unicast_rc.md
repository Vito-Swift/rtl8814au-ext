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
  - 0-9: {CCK1M, CCK2M, CCK5.5M, CCK11M, 6M, 9M, 12M, 18M, 24M, 36M}
  - 10-19: MCS4-MCS13
  - 20-29: MCS20-MCS29
  - 30-35: VHTSS1MCS4-VHTSS1MCS9
  - 36-39: VHTSS2MCS0-VHTSS2MCS3
  - 40-49: VHTSS3MCS0-VHTSS3MCS9
  - 50-53: VHTSS4MCS6-VHTSS4MCS9

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
