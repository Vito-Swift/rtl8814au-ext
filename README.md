# RTL8814AU Driver

original repository: [morrownr/8814au](https://github.com/morrownr/8814au)

---

##

Enable vht when launch kernel module (very important for use in AP mode!!!)
```
$ sudo modprobe -r 8814au && sudo modprobe 8814au rtw_vht_enable=2
```

## Reference points:

- Linux frame transmit entry point: [os_dep/linux/xmit_linux.c](https://github.com/Vito-Swift/rtl8814au-ext/blob/main/os_dep/linux/xmit_linux.c)
- Fill-in frame headers in monitor mode: [core/rtw_mlme_ext.c](https://github.com/Vito-Swift/rtl8814au-ext/blob/main/core/rtw_mlme_ext.c)
