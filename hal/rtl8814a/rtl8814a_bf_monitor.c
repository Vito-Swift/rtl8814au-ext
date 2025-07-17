/**
 * @ Author: Vito Wu
 * @ Create Time: 2025-07-11 12:40:59
 * @ Modified by: Vito Wu
 * @ Modified time: 2025-07-18 00:33:49
 * @ Description:
 */

#define _RTL8814A_BF_MONITOR_C_

#include <hal_data.h>
#include <drv_types.h>
#include <rtw_rf.h>
#include <rtw_recv.h>
#include <rtw_xmit.h>
#include <hal_intf.h>
#include "hal8814a_fw.h"

#ifdef CONFIG_BEAMFORMING_MONITOR

/* hal/phydm/txbf/haltxbfinterface.c: send_fw_ndpa_packet */
u8 bf_monitor_send_vht_ndpa(PADAPTER adapter,
                            u8 *ra,
                            u8 *ta,
                            u16 p_aid,
                            u8 g_id,
                            u8 seq,
                            enum channel_width bw)
{
    struct xmit_priv *pxmitpriv;
    struct mlme_ext_priv *pmlmeext;
    struct xmit_frame *pmgntframe;
    struct mlme_ext_info *pmlmeinfo;
    struct ndpa_sta_info_monitor sta_info;
    struct pkt_attrib *attrib;
    struct rtw_ieee80211_hdr *pwlanhdr;
    u8 *pframe;
    enum MGN_RATE txrate;
    u16 duration = 0;
    u8 aSifsTime = 0;
    u8 sequence = 0;

    pxmitpriv = &adapter->xmitpriv;
    pmlmeext = &adapter->mlmeextpriv;
    pmlmeinfo = &pmlmeext->mlmext_info;
    pmgntframe = alloc_mgtxmitframe(pxmitpriv);
    if (!pmgntframe)
    {
        RTW_ERR("%s: alloc mgnt frame fail!\n", __FUNCTION__);
        return _FALSE;
    }

    attrib = &pmgntframe->attrib;
    update_mgntframe_attrib(adapter, attrib);
    attrib->type = WIFI_MGT_TYPE;
    attrib->subtype = WIFI_NDPA;
    attrib->bwmode = bw;
    attrib->qsel = QSLT_MGNT;
    attrib->rate = (u8)ODM_MGN_VHT2SS_MCS0;
    attrib->bf_pkt_type = 0;
    attrib->txbf_g_id = g_id;
    attrib->txbf_p_aid = p_aid;
    attrib->retry_ctrl = _FALSE;

    _rtw_memset(pmgntframe->buf_addr, 0, TXDESC_OFFSET + WLANHDR_OFFSET);
    pframe = pmgntframe->buf_addr + TXDESC_OFFSET;
    pwlanhdr = (struct rtw_ieee80211_hdr *)pframe;
    pwlanhdr->frame_ctl = 0;
    set_frame_sub_type(pframe, attrib->subtype);

    aSifsTime = 16;
    duration = 2 * aSifsTime + 44;
    if (bw == CHANNEL_WIDTH_80)
        duration += 40;
    else if (bw == CHANNEL_WIDTH_40)
        duration += 87;
    else
        duration += 180;
    set_duration(pframe, duration);

    /* Receiver Address (bfee)*/
    _rtw_memcpy(pwlanhdr->addr1, ra, ETH_ALEN);
    /* Transmitter Address (bfer)*/
    _rtw_memcpy(pwlanhdr->addr2, ta, ETH_ALEN);

    /* Sounding Sequence, bit0~1 is reserved */
    sequence = (seq << 2) & 0x3f;
    _rtw_memcpy(pframe + 16, &sequence, 1);

    /* STA Info */
    sta_info.aid = p_aid;
    sta_info.feedback_type = 0; /* "Feedback Type" set to 0 for SU */
    sta_info.nc_index = 1;      /* "Nc Index" reserved if the Feedback Type field indicates SU */
    _rtw_memcpy(pframe + 17, (u8 *)&sta_info, 2);

    attrib->pktlen = 19;
    attrib->last_txcmdsz = attrib->pktlen;

    dump_mgntframe(adapter, pmgntframe);

    return _TRUE;
}

/* hal/phydm/txbf/haltxbf8814a.c */
void bf_monitor_phydm_txbf_rfmode_8814a(PADAPTER adapter, u8 su_bfee_cnt)
{
    struct dm_struct *dm = (struct dm_struct *)GET_PDM_ODM(adapter);
    u8 i;
    u8 tx_ss = 3;
    if (dm->rf_type == RF_1T1R)
        return;

#if DEV_BUS_TYPE == RT_USB_INTERFACE
    tx_ss = *dm->hub_usb_mode;
#endif
    if (tx_ss == 3 || tx_ss == 2)
    {
        if (dm->rf_type == RF_4T4R)
            tx_ss = 0xf;
        else if (dm->rf_type == RF_3T3R)
            tx_ss = 0xe;
        else
            tx_ss = 0x6;
    }
    else if (tx_ss == 1) /*USB 2.0 always 2Tx*/
        tx_ss = 0x6;
    else
        tx_ss = 0x6;

    if (tx_ss == 0xf)
    {
        odm_set_bb_reg(dm, REG_BB_TX_PATH_SEL_1_8814A, MASKBYTE3 | MASKBYTE2HIGHNIBBLE, 0x93f);
        odm_set_bb_reg(dm, REG_BB_TX_PATH_SEL_1_8814A, MASKDWORD, 0x93f93f0);
    }
    else if (tx_ss == 0xe)
    {
        odm_set_bb_reg(dm, REG_BB_TX_PATH_SEL_1_8814A, MASKBYTE3 | MASKBYTE2HIGHNIBBLE, 0x93e);
        odm_set_bb_reg(dm, REG_BB_TX_PATH_SEL_2_8814A, MASKDWORD, 0x93e93e0);
    }
    else if (tx_ss == 0x6)
    {
        odm_set_bb_reg(dm, REG_BB_TX_PATH_SEL_1_8814A, MASKBYTE3 | MASKBYTE2HIGHNIBBLE, 0x936);
        odm_set_bb_reg(dm, REG_BB_TX_PATH_SEL_2_8814A, MASKLWORD, 0x9360);
    }

    /*@for 8814 19ac(idx 1), 19b4(idx 0), different Tx ant setting*/
    odm_set_bb_reg(dm, REG_BB_TXBF_ANT_SET_BF1_8814A, BIT(28) | BIT29, 0x2); /*@enable BB TxBF ant mapping register*/
    odm_set_bb_reg(dm, REG_BB_TXBF_ANT_SET_BF1_8814A, BIT30, 0x1);           /*@if Nsts > Nc don't apply V matrix*/
}

void bf_monitor_sounding_config(PADAPTER adapter,
                                u8 bf_en,
                                enum channel_width bw)
{
}

void bf_monitor_init(PADAPTER adapter, u8 *bfer_macaddr, u8 bfer_g_id, u16 bfee_p_aid)
{
}

void bf_monitor_reset(PADAPTER adapter)
{
}

void bf_monitor_send(PADAPTER adapter, u8 *bfer_macaddr, u8 *bfee_macaddr, u16 p_aid, u8 g_id, u8 seq, enum channel_width bw)
{
}

void bf_monitor_print_cbr(PADAPTER adapter, struct seq_file *m)
{
}

void bf_monitor_print_conf(PADAPTER adapter, struct seq_file *m)
{
}

void bf_monitor_enable_txbf(PADAPTER adapter, bool en)
{
}

#endif /* CONFIG_BEAMFORMING_MONITOR */