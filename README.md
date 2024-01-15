# RTL8814AU Driver

original repository: [morrownr/8814au](https://github.com/morrownr/8814au)

---

## Useful commands

1. Rebuild this driver
    ```shell
    $ sudo ./remove-driver.sh && sudo ./install-driver.sh  
    ```

2. Enable vht (80211ac) when launch kernel module (very important for use in AP mode!!!)
    ```shell
    $ sudo modprobe -r 8814au && sudo modprobe 8814au rtw_vht_enable=2
    ```

3. Read proc debug
    ```shell
    $ cat /proc/net/rtl8814au/${iface_name}/sta_tp_info
    ====== mac_id : 0 [STA MAC ADDR] ======
    tx_rate : CCK_1M(L)  rx_rate : OFDM_6M, rx_rate_bmc : CCK_1M, rssi : 53 %
    RTW: [TP] Tx : 0(Kbps) Rx : 0(Kbps) Total : 0(Kbps)
    RTW: [Smooth TP] Tx : 0(Kbps) Rx : 0(Kbps) Total : 0(Kbps)
    ```
## Reference points:

- Linux frame transmit entry point: [os_dep/linux/xmit_linux.c](https://github.com/Vito-Swift/rtl8814au-ext/blob/main/os_dep/linux/xmit_linux.c)
- Fill-in frame headers in monitor mode: [core/rtw_mlme_ext.c](https://github.com/Vito-Swift/rtl8814au-ext/blob/main/core/rtw_mlme_ext.c)
