/**
 * @ Author: Vito Wu
 * @ Create Time: 2025-07-11 12:41:13
 * @ Modified by: Vito Wu
 * @ Modified time: 2025-07-17 10:33:11
 * @ Description:
 */

#ifdef CONFIG_BEAMFORMING_MONITOR

struct ndpa_sta_info_monitor
{
    u16 aid : 12;
    u16 feedback_type : 1;
    u16 nc_index : 3;
};

struct csi_rpt_monitor
{
    u8 nc;
    u8 nr;
    enum channel_width bw;
    u8 ng;
    u8 codebook;
    u8 token;
    s8 rx_pwr[4];       // per-path's pwdb
    s8 rx_snr[4];       // per-path's SNR
    u8 rx_evm[4];       // per-path's EVM
    s8 snr[4];          // [-128,127], map to [-10dB,+53.75dB]
    u16 csi_matrix_len; // 80m=293(292.5)/234sc, 40m=135/108sc, 20m=65/52sc, 6bit_phi+4bit_psi
                        // u8 csi_matrix[293];  // useless
};

// PROCFS hdl
void bf_monitor_init(PADAPTER adapter, u8 *bfer_macaddr, u8 bfer_g_id, u16 bfee_p_aid);
void bf_monitor_reset(PADAPTER adapter);
void bf_monitor_send(PADAPTER adapter, u8 *bfer_macaddr, u8 *bfee_macaddr, u16 p_aid, u8 g_id, u8 seq, enum channel_width bw);
void bf_monitor_print_cbr(PADAPTER adapter, struct seq_file *m);
void bf_monitor_print_conf(PADAPTER adapter, struct seq_file *m);
void bf_monitor_enable_txbf(PADAPTER adapter, bool en);

// Internal APIs
u8 bf_monitor_send_vht_ndpa(PADAPTER adapter, u8 *ra, u8 *ta, u16 p_aid, u8 g_id, u8 seq, enum channel_width bw);

#endif /* CONFIG_BEAMFORMING_MONITOR */