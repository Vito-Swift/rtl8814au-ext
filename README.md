# RTL8814AU Driver

original repository: [morrownr/8814au](https://github.com/morrownr/8814au)

---

## Features

1. Assign and control Layer-II broadcast MCS rate in driver initfs 
2. Assign and control Layer-I injection MCS rate in Radiotap header field
3. Read raw compressed beamforming reports (NDPA response)
4. Proc Debug mode

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

4. Initfs TXBF parameters (os_dep/linux/os_initfs.c):
    ```c
    /*
     * BIT0: Enable VHT SU Beamformer
     * BIT1: Enable VHT SU Beamformee
     * BIT2: Enable VHT MU Beamformer, depend on VHT SU Beamformer
     * BIT3: Enable VHT MU Beamformee, depend on VHT SU Beamformee
     * BIT4: Enable HT Beamformer
     * BIT5: Enable HT Beamformee
    */
    int rtw_beamform_cap = BIT(1) | BIT(3);
    int rtw_bfer_rf_number = 0; /*BeamformerCapRfNum Rf path number, 0 for auto, others for manual*/
    int rtw_bfee_rf_number = 0; /*BeamformeeCapRfNum  Rf path number, 0 for auto, others for manual*/
    ```

5. Fix unicast rate: [how_to_do_unicast_rc.md](https://github.com/Vito-Swift/rtl8814au-ext/blob/main/doc/how_to_do_unicast_rc.md)

6. Control TX power: `iw dev $iface_name set txpower fixed $tx_power`, where `$tx_power` is mW/1000.

## Reference points:

- Linux frame transmit entry point: [os_dep/linux/xmit_linux.c](https://github.com/Vito-Swift/rtl8814au-ext/blob/main/os_dep/linux/xmit_linux.c)
- Fill-in frame headers in monitor mode: [core/rtw_mlme_ext.c](https://github.com/Vito-Swift/rtl8814au-ext/blob/main/core/rtw_mlme_ext.c)
