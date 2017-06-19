/*******************************************************************************
 * (c) Copyright 2014-2016 Microsemi SoC Products Group.  All rights reserved.
 *
 * This file contains type definitions used throughout CoreTSE and PHY device
 * drivers. User need not include this file in application source code.
 * Inclusion of core_tse.h inherits these types.
 *
 * SVN $Revision: 8477 $
 * SVN $Date: 2016-07-12 10:30:54 +0530 (Tue, 12 Jul 2016) $
 */
#ifndef CORE_TSE_TYPES_H_
#define CORE_TSE_TYPES_H_

#include <cpu_types.h>
#include "coretse_user_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************/
/* Public type definitions                                                     */
/*******************************************************************************/

/*******************************************************************************
 * MAC interface speed
 * This type definition provides various interface speeds supported by
 * CoreTSE_AHB/CoreTSE IP.
 */
typedef enum
{
    TSE_MAC10MBPS     = 0x00,
    TSE_MAC100MBPS    = 0x01,
    TSE_MAC1000MBPS   = 0x02,
    TSE_INVALID_SPEED = 0x03
} tse_speed_t;

/*******************************************************************************
 * Transmit and Receive statistics.
 * This type definition provides various statistics supported by this driver.
 * The desired statistics value to be read is to be passed to the function
 * TSE_read_stat(). The width of returned statistic value is mentioned in the
 * comment against the statistic.
 */
typedef enum
{
    TSE_FRAME_CNT_64,                 /* 18-bit */
    TSE_FRAME_CNT_127,                /* 18-bit */
    TSE_FRAME_CNT_255,                /* 18-bit */
    TSE_FRAME_CNT_511,                /* 18-bit */
    TSE_FRAME_CNT_1K,                 /* 18-bit */
    TSE_FRAME_CNT_MAX,                /* 18-bit */
    TSE_FRAME_CNT_VLAN,               /* 18-bit */
    TSE_RX_BYTE_CNT,                  /* 24-bit */
    TSE_RX_PKT_CNT,                   /* 18-bit */
    TSE_RX_FCS_ERR_CNT,               /* 12-bit */
    TSE_RX_MULTICAST_PKT_CNT,         /* 18-bit */
    TSE_RX_BROADCAST_PKT_CNT,         /* 22-bit */
    TSE_RX_CTRL_PKT_CNT,              /* 18-bit */
    TSE_RX_PAUSE_PKT_CNT,             /* 12-bit */
    TSE_RX_UNKNOWN_OPCODE_CNT,        /* 12-bit */
    TSE_RX_ALIGN_ERR_CNT,             /* 12-bit */
    TSE_RX_FRAMELENGTH_ERR_CNT,       /* 16-bit */
    TSE_RX_CODE_ERR_CNT,              /* 12-bit */
    TSE_RX_CS_ERR_CNT,                /* 12-bit */
    TSE_RX_UNDERSIZE_PKT_CNT,         /* 12-bit */
    TSE_RX_OVERSIZE_PKT_CNT,          /* 12-bit */
    TSE_RX_FRAGMENT_CNT,              /* 12-bit */
    TSE_RX_JABBER_CNT,                /* 12-bit */
    TSE_RX_DROP_CNT,                  /* 12-bit */
    TSE_TX_BYTE_CNT,                  /* 24-bit */
    TSE_TX_PKT_CNT,                   /* 18-bit */
    TSE_TX_MULTICAST_PKT_CNT,         /* 18-bit */
    TSE_TX_BROADCAST_PKT_CNT,         /* 18-bit */
    TSE_TX_PAUSE_PKT_CNT,             /* 12-bit */
    TSE_TX_DEFFERAL_PKT_CNT,          /* 12-bit */
    TSE_TX_EXCS_DEFFERAL_PKT_CNT,     /* 12-bit */
    TSE_TX_SINGLE_COLL_PKT_CNT,       /* 12-bit */
    TSE_TX_MULTI_COLL_PKT_CNT,        /* 12-bit */
    TSE_TX_LATE_COLL_PKT_CNT,         /* 12-bit */
    TSE_TX_EXCSS_COLL_PKT_CNT,        /* 12-bit */
    TSE_TX_TOTAL_COLL_PKT_CNT,        /* 13-bit */
    TSE_TX_PAUSE_HONORED_CNT,         /* 12-bit */
    TSE_TX_DROP_CNT,                  /* 12-bit */
    TSE_TX_JABBER_CNT,                /* 12-bit */
    TSE_TX_FCS_ERR_CNT,               /* 12-bit */
    TSE_TX_CNTRL_PKT_CNT,             /* 12-bit */
    TSE_TX_OVERSIZE_PKT_CNT,          /* 12-bit */
    TSE_TX_UNDERSIZE_PKT_CNT,         /* 12-bit */
    TSE_TX_FRAGMENT_CNT,              /* 12-bit */
    TSE_MAC_LAST_STAT
} tse_stat_t;

/*******************************************************************************
 * The application must use this function prototype to define the transmit
 * completion handler call-back function which can be provided as a parameter
 * to the TSE_set_tx_callback() function.
 */
typedef void (*tse_transmit_callback_t)(void * p_user_data);

/*******************************************************************************
 * The application must use this function prototype to define the receive
 * call-back listener function which can be provided as a parameter to the
 * TSE_set_rx_callback() function.
 */
typedef void (*tse_receive_callback_t)(uint8_t * p_rx_packet,
                                       uint32_t pckt_length,
                                       void * p_user_data);

/*******************************************************************************
 * The application must use this function prototype to define the WoL event
 * handler function which can be provided as a parameter to the
 * TSE_set_wol_callback() function.
 */
typedef void (*tse_wol_callback_t)(void);

/*******************************************************************************
 * DMA Descriptor
 * Driver creates and manages two descriptor rings for transmission and reception.
 */
typedef struct
{
    uint32_t pkt_start_addr;    /* Packet start address */
    volatile uint32_t pkt_size; /* Packet size & Per packet override flags */
    uint32_t next_desriptor;    /* Link to next descriptor */
    uint32_t index;             /* Index: helps in handling interrupts */
    void * caller_info;         /* Pointer to user specific data */
} tse_desc_t;

/*******************************************************************************
 * CoreTSE_AHB/CoreTSE Configuration Structure.
 You need to create a record of this type to hold the configuration of the
 CoreTSE_AHB/CoreTSE. The TSE_cfg_struct_def_init() function should be used to
 initialize the configuration record to default values. Later, the configuration
 elements in the record can be changed to desired values.

  framefilter
  The framefilter configuration parameter specifies the address based frame
  filtering choice. The framefilter configuration can be set to a bitmask of the
  following defines to specify the address based frame filtering mode:
        • TSE_FC_PASS_BROADCAST_MASK
        • TSE_FC_PASS_MULTICAST_MASK
        • TSE_FC_PASS_UNICAST_MASK
        • TSE_FC_PROMISCOUS_MODE_MASK
        • TSE_FC_PASS_UNICAST_HASHT_MASK
        • TSE_FC_PASS_MULTICAST_HASHT_MASK
        • TSE_FC_DEFAULT_MASK

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_FC_DEFAULT_MASK to indicating that frames with broadcast and unicast match
  should only be received. All other frames will be dropped.

  Note: The TSE_set_address_filter() must be used to set the list of
  destination mac addresses which will be passed when the constants
  TSE_FC_PASS_UNICAST_HASHT_MASK and/or TSE_FC_PASS_MULTICAST_HASHT_MASK is
  specified.

  speed_duplex_select
  The speed_duplex_select configuration parameter specifies the allowed link
  speeds. It is a bit-mask of the various link speed and duplex modes. The
  speed_duplex_select configuration can be set to a bitmask of the following
  defines to specify the allowed link speed and duplex mode:
          • TSE_ANEG_10M_FD
          • TSE_ANEG_10M_HD
          • TSE_ANEG_100M_FD
          • TSE_ANEG_100M_HD
          • TSE_ANEG_1000M_FD
          • TSE_ANEG_1000M_HD

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_ANEG_ALL_SPEEDS indicating that a link will be setup for best available
  speed and duplex combination.

  mac_addr
  The mac_addr configuration parameter is a 6-byte array containing the local
  MAC address of CoreTSE_AHB/CoreTSE.

  phy_address
  The phy_address parameter specifies the address of the PHY device, set in
  hardware by the address pins of the PHY device.

  tx_edc_enable
  The tx_edc_enable parameter specifies enable or disable error detection and
  correction for tx FIFOs. The allowed values for the tx_edc_enable configuration
  parameter are:
          • TSE_ERR_DET_CORR_ENABLE
          • TSE_ERR_DET_CORR_DISABLE
  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_ERR_DET_CORR_ENABLE.

  rx_edc_enable
  The rx_edc_enable parameter specifies enable or disable error detection and
  correction for rx FIFOs. The allowed values for the rx_edc_enable configuration
  parameter are:
          • TSE_ERR_DET_CORR_ENABLE
          • TSE_ERR_DET_CORR_DISABLE

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_ERR_DET_CORR_ENABLE.

  preamble_length
  The preamble_length parameter specifies the length of the preamble field of
  the packet in bytes. The allowed values for the preamble_length configuration
  parameter are:
          • TSE_PREAMLEN_DEFVAL
          • TSE_PREAMLEN_MAXVAL

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_PREAMLEN_DEFVAL.

  hugeframe_enable
  The hugeframe_enable parameter specifies enable or disable huge frame support.
  When enabled, it allows frames longer than the maximum frame length to be
  transmitted and received. When disabled, the hardware limits the length of
  frames at the maximum frame length. The allowed values for the hugeframe_enable
  configuration parameter are:
          • TSE_HUGE_FRAME_ENABLE
          • TSE_HUGE_FRAME_DISABLE

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_HUGE_FRAME_DISABLE.

  length_field_check
  The length_field_check parameter specifies enable or disable length filed check.
  When enabled, the CoreTSE_AHB/CoreTSE checks the frame’s length field to
  ensure it matches the actual data field length. The allowed values for the
  length_field_check configuration parameter are:
          • TSE_LENGTH_FILED_CHECK_ENABLE
          • TSE_LENGTH_FILED_CHECK_DISABLE

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_LENGTH_FILED_CHECK_ENABLE.

  pad_n_CRC
  The pad_n_CRC parameter specifies enable or disable padding and appending CRC.
  When enabled, the CoreTSE_AHB/CoreTSE pads all the short frames and appends a
  CRC to every frame whether or not padding is required. When disabled, frames
  presented to the CoreTSE_AHB/CoreTSE have a valid length and contain a CRC.
  The allowed values for the pad_n_CRC configuration parameter are:
          • TSE_PAD_N_CRC_ENABLE
          • TSE_PAD_N_CRC_DISABLE

  The TSE_cfg_struct_def_init() function sets this configuration parameter
  to TSE_PAD_N_CRC_ENABLE

  append_CRC
  The append_CRC parameter specifies enable or disable appending CRC. When
  enabled, the CoreTSE_AHB/CoreTSE appends a CRC to all the frames. When disabled,
  frames presented to the CoreTSE_AHB/CoreTSE, have a valid length and contain a
  valid CRC. The allowed values for the append_CRC configuration parameter are:
          • TSE_CRC_ENABLE
          • TSE_CRC_DISABLE

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_CRC_ENABLE.

  fullduplex
  The fullduplex parameter specifies enable or disable full duplex. When enabled,
  the CoreTSE_AHB/CoreTSE operates in full duplex mode. When disabled, the MAC
  operates in half duplex mode. The allowed values for the fullduplex
  configuration parameter are:
          • TSE_FULLDUPLEX_ENABLE
          • TSE_FULLDUPLEX_DISABLE
  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_FULLDUPLEX_ENABLE.

  loopback
  The loopback parameter specifies enable or disable loop back mode. When
  enabled, the CoreTSE_AHB/CoreTSE’s transmit outputs to be looped back to its
  receiving inputs. The allowed values for the loopback configuration parameter
  are:
          • TSE_LOOPBACK_ENABLE
          • TSE_LOOPBACK_DISABLE

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_LOOPBACK_DISABLE.

  rx_flow_ctrl
  The rx_flow_ctrl parameter specifies enable or disable receiver flow control.
  When enabled, the CoreTSE_AHB/CoreTSE detects and acts on PAUSE flow control
  frames. When disabled, it ignores PAUSE flow control frames. The allowed values
  for the rx_flow_ctrl configuration parameter are:
          • TSE_RX_FLOW_CTRL_ENABLE
          • TSE_RX_FLOW_CTRL_DISABLE

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_RX_FLOW_CTRL_ENABLE.

  tx_flow_ctrl
  The tx_flow_ctrl parameter specifies enable or disable transmitter flow control.
  When enabled, the transmitter sends PAUSE flow control frames when requested
  by the system. When disabled, prevents the transmitter from sending flow
  control frames. The allowed values for the tx_flow_ctrl configuration
  parameter are:
          • TSE_TX_FLOW_CTRL_ENABLE
          • TSE_TX_FLOW_CTRL_DISABLE

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_TX_FLOW_CTRL_ENABLE.

  min_IFG
  The min_IFG parameter specifies minimum size of management gap (IFG) to enforce
  between frames (expressed in bit times). The allowed values for the min_IFG
  configuration parameter are:
          • TSE_MINIFG_DEFVAL
          • TSE_MINIFG_MAXVAL

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_MINIFG_DEFVAL.

  btb_IFG
  The btb_IFG parameter specifies the Inter frame gap between back-to-back
  packets (expressed in bit times), used exclusively in full-duplex mode when
  two transmit packets are sent back-to-back. The allowed values for the btb_IFG
  configuration parameter are:
          • TSE_BTBIFG_DEFVAL
          • TSe_BTBIFG_MAXVAL

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_BTBIFG_DEFVAL.

  max_retx_tries
  The max_retx_tries parameter specifies the number of retransmission attempts
  following a collision before aborting the packet due to excessive collisions.
  The allowed values for the max_retx_tries configuration parameter are:
          • TSE_MAXRETX_DEFVAL
          • TSE_MAXRETX_MAXVAL

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_MAXRETX_DEFVAL.

  excessive_defer
  The excessive_defer parameter specifies enable or disable transmission of
  packet that exceeded max collisions. When enabled, the transmitter allows the
  transmission of a packet that has been excessively deferred. When disabled,
  the transmitter aborts the transmission of a packet that has been excessively
  deferred. The allowed values for the excessive_defer configuration parameter
  are:
          • TSE_EXSS_DEFER_ENABLE
          • TSE_EXSS_DEFER_DISABLE

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_EXSS_DEFER_DISABLE.

  nobackoff
  The nobackoff parameter specifies enable or disable back off. When enabled,
  the transmitter immediately re-transmits following a collision. When disabled,
  the transmitter follows the binary exponential backoff rule. The allowed
  values for the nobackoff configuration parameter are:
          • TSE_NO_BACKOFF_ENABLE
          • TSE_NO_BACKOFF_DISABLE

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_NO_BACKOFF_DISABLE.

  backpres_nobackoff
  The backpres_nobackoff parameter specifies enable or disable back off in back
  pressure mode. When enabled, the transmitter immediately re-transmits
  following a collision during back pressure operation. When disabled, the
  transmitter follows the binary exponential backoff rule. The allowed values
  for the backpres_nobackoff configuration parameter are:
      • TSE_BACKPRESS_NO_BACKOFF_ENABLE
      • TSE_BACKPRESS_NO_BACKOFF_DISABLE

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_BACKPRESS_NO_BACKOFF_DISABLE.

  ABEB_enable
  The ABEB_enable parameter specifies enable or disable arbitrary binary
  exponential back-off. When enabled, it configures the transmitter to use the
  ABEB_truncvalue value instead of the 802.3 standard tenth collisions. When
  disabled, it causes the transmitter to follow the 802.3 standard binary
  exponential backoff rule. The allowed values for the ABEB_enable configuration
  parameter are:
          • TSE_ABEB_ENABLE
          • TSE_ABEB_DISABLE

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_ABEB_DISABLE.

  ABEB_truncvalue
  The ABEB_truncvalue parameter specifies alternative binary exponential back-off
  value. This value is used when the ABEB_enable parameter is enabled. The allowed
  values for the ABEB_truncvalue configuration parameter are:
          • TSE_ABEBTRUNC_DEFVAL
          • TSE_ABEBTRUNC_MAXVAL

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_ABEBTRUNC_DEFVAL.

  phyclk
  The phyclk parameter specifies MII management clock divider value. PCLK is the
  source clock The allowed values for the phyclk configuration parameter are:
          • TSE_DEF_PHY_CLK
          • TSE_BY4_PHY_CLK
          • TSE_BY6_PHY_CLK
          • TSE_BY8_PHY_CLK
          • TSE_BY10_PHY_CLK
          • TSE_BY14_PHY_CLK
          • TSE_BY20_PHY_CLK
          • TSE_BY28_PHY_CLK

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_DEF_PHY_CLK.

  supress_preamble
  The supress_preamble parameter specifies enable or disable preamble suppression
  at PHY. When enabled, MII Management suppresses preamble generation and reduces
  the Management cycle from 64 clocks to 32clocks. When disabled, MII Management
  performs Management read/write cycles with the 64 clocks of preamble. The
  allowed values for the supress_preamble configuration parameter are:
          • TSE_SUPPRESS_PREAMBLE_ENABLE
          • TSE_SUPPRESS_PREAMBLE_DISABLE

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_SUPPRESS_PREAMBLE_DISABLE.

  autoscan_phys
  The autoscan_phys parameter specifies enable or disable of the continual read
  from a set of PHYs of contiguous address space. When enabled, it causes MDIO
  management to continually read from a set of PHYs of contiguous address space.
  The starting address of the PHY is specified by the content of the PHY address
  field . The next PHY to be read will be PHY address + 1. The last PHY to be
  queried in this read sequence will be the one residing at address 0x31, after
  which the read sequence returns to the PHY specified by the PHY address field.
  When disabled, MDIO management performs only one read operation at the specified
  PHY address. The allowed values for the max_frame_length configuration parameter
  are:
          • TSE_PHY_AUTOSCAN_ENABLE
          • TSE_PHY_AUTOSCAN_DISABLE

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_PHY_AUTOSCAN_DISABLE.

  max_frame_length
  The max_frame_length parameter specifies the maximum frame size in both the
  transmit and receive directions. The allowed values for the max_frame_length
  configuration parameter are:
          • TSE_MAXFRAMELEN_DEFVAL
          • TSE_MAXFRAMELEN_MAXVAL

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_MAXFRAMELEN_DEFVAL.

  non_btb_IFG
  The non_btb_IFG parameter specifies non back to back inter-frame gap value.
  The allowed values for the non_btb_IFG configuration parameter are:
          • TSE_NONBTBIFG_DEFVAL
          • TSE_NONBTBIFG_MAXVAL

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_NONBTBIFG_DEFVAL.

  slottime
  The slottime parameter specifies the slot time or collision window during
  which collisions might occur in a properly configured network. The allowed
  values for the slottime configuration parameter are:
          • TSE_SLOTTIME_DEFVAL
          • TSE_SLOTTIME_MAXVAL

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_SLOTTIME_DEFVAL.

  framedrop_mask
  The framedrop_mask parameter specifies to drop frames based on receive
  statistics. The allowed values for the framedrop_mask configuration parameter
  are:
        •   TSE_DEFVAL_FRAMEDROP_MASK
        •   TSE_SMALL_AFG_FRAMEDROP_MASK
        •   TSE_EXDV_EVENT_FRAMEDROP_MASK
        •   TSE_FALSE_CARRIER_FRAMEDROP_MASK
        •   TSE_CODE_ERR_FRAMEDROP_MASK
        •   TSE_CRC_ERR_FRAMEDROP_MASK
        •   TSE_LEN_CHKERR_FRAMEDROP_MASK
        •   TSE_OUTOFRANGE_LEN_FRAMEDROP_MASK
        •   TSE_OK_FRAME_FRAMEDROP_MASK
        •   TSE_MULTICAST_FRAMEDROP_MASK
        •   TSE_BROADCAST_FRAMEDROP_MASK
        •   TSE_DRIBBLE_NIBBLE_FRAMEDROP_MASK
        •   TSE_CONTROL_FRAME_FRAMEDROP_MASK
        •   TSE_PAUSE_FRAME_FRAMEDROP_MASK
        •   TSE_UNSUPPORTED_OPCODE_FRAMEDROP_MASK
        •   TSE_VLAN_TAG_FRAMEDROP_MASK
        •   TSE_LONG_EVENT_FRAMEDROP_MASK
        •   TSE_TRUNCKATED_FRAME_FRAMEDROP_MASK
        •   TSE_UNICAST_NO_SAMATCH_FRAMEDROP_MASK
        •   TSE_SHORT_FRAME_FRAMEDROP_MASK

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_DEFVAL_FRAMEDROP_MASK.

  wol_enable
  The wol_enable parameter specifies enable or disable of the Wake-On-LAN (WoL)
  feature. When enabled, CoreTSE_AHB/CoreTSE can detect either the Unicast match
  frame or AMD magic packet frame and assert the WoL interrupt. When disabled,
  the WoL interrupt is not asserted even though Unicast match frame or AMD
  magic packet frame is detected. The allowed values for the supress_preamble
  configuration parameter are:
          • TSE_WOL_UNICAST_FRAME_DETECT_EN
          • TSE_WOL_MAGIC_FRAME_DETECT_EN
          • TSE_WOL_DETECT_DISABLE

  aneg_enable
  The aneg_enable parameter specifies enable or disable of the Auto-Negotiation
  feature in the PHY and “TBI/1000BaseX” module. When enabled, CoreTSE_AHB/CoreTSE
  the Auto-negotiation in the PHY and “TBI/1000BaseX” module is enabled. When
  disabled, CoreTSE_AHB/CoreTSE does not enable auto-negotiation in PHY and
  “TBI/1000BaseX” module. Disabling auto-negotiation allows user to use the
  CoreTSE_AHB/CoreTSE in point to point topology where speed/duplex configuration
  is known and auto-negotiation is not required.

  The TSE_cfg_struct_def_init() function sets this configuration parameter to
  TSE_ENABLE.

 */
typedef struct
{
    uint32_t framefilter;               /*Address based frame filter configuration*/
    uint32_t speed_duplex_select;       /* Link speed and duplex mode allowed to setup a link. */
    uint8_t mac_addr[6];                /* Station's MAC address */
    uint8_t phy_addr;                   /* Address of Ethernet PHY on MII management interface. */
    uint8_t tx_edc_enable;              /* Enable / disable error detection and correction for tx FIFOs */
    uint8_t rx_edc_enable;              /* Enable / disable error detection and correction for rx FIFOs */
    uint8_t preamble_length;            /* 4-bit Length of preamble field: default value is 0x7 */
    uint8_t hugeframe_enable;           /* Enable / disable huge frame support: default is disable 0 */
    uint8_t length_field_check;         /* Enable / disable length field checking */
    uint8_t pad_n_CRC;                  /* Enable / disable padding and appending CRC */
    uint8_t append_CRC;                 /* Enable / disable appending CRC */
    uint8_t fullduplex;                 /* Enable / disable full duplex: default is disable 0 */
    uint8_t loopback;                   /* Enable / disable loopback mode: default is disable 0 */
    uint8_t rx_flow_ctrl;               /* Enable / disable receiver flow control: default is disable 0 */
    uint8_t tx_flow_ctrl;               /* Enable / disable transmitter flow control: default is disable 0 */
    uint8_t min_IFG;                    /* 8-bit minimum inter-frame gap value */
    uint8_t btb_IFG;                    /* 7-bit back to back inter-frame gap value */
    uint8_t max_retx_tries;             /* 5-bit maximum retransmission tries value: default is 0xF */
    uint8_t excessive_defer;            /* Enable / disable transmission of packet that exceeded max collisions: default is disable 0 */
    uint8_t nobackoff;                  /* Enable / disable back off. default is disable 0 */
    uint8_t backpres_nobackoff;         /* Enable / disable back off in back pressure mode: default is disable 0 */
    uint8_t ABEB_enable;                /* Enable / disable arbitrary binary exponential back-off: default is disable 0 */
    uint8_t ABEB_truncvalue;            /* 4-bit alternative binary exponential back-off value: default is 0xA */
    uint8_t phyclk;                     /* 3-bit MGMT clock divider value */
    uint8_t supress_preamble;           /* Enable / disable preamble suppression at PHY: default is disable 0 */
    uint8_t autoscan_phys;              /* Enable / disable auto scanning of PHYs with programmed addresses: default is disable 0 */
    uint16_t max_frame_length;          /* Maximum frame length: default value is 0x0600(1536d) */
    uint16_t non_btb_IFG;               /* 14-bit non back to back inter-frame gap value */
    uint16_t slottime;                  /* 10-bit collision window value : default is 0x37 */
    uint32_t framedrop_mask;            /* 18-bit mask to drop frames based on receive statistics */
    uint8_t  wol_enable;                /* Enable / disable WoL event detect feature */
    uint8_t  aneg_enable;               /* Enable / disable Auto-negotiation in MSGMII module as well as phy (if PHY is in the system) */
} tse_cfg_t;

/*******************************************************************************
 * CoreTSE_AHB/CoreTSE hardware instance
 * This structure is used to identify the various CoreTSE_AHB/CoreTSE hardware
 * instances in your system. Your application software must declare one instance
 * of this structure for each instance of CoreTSE_AHB/CoreTSE in your system. The
 * function TSE_init() initializes this structure. A pointer to an initialized
 * instance of the structure must be passed as the first parameter to the CoreTSE
 * driver functions, to identify which CoreTSE_AHB/CoreTSE hardware instance s
 * should perform the requested operation.
 */
typedef struct
{
    uint32_t                    base_addr;
    tse_desc_t                  tx_desc_tab[TSE_TX_RING_SIZE]; /* Transmit descriptor table */
    tse_desc_t                  rx_desc_tab[TSE_RX_RING_SIZE]; /* Receive descriptor table */
    tse_transmit_callback_t     tx_complete_handler;
    tse_receive_callback_t      pckt_rx_callback;
    int16_t                     nb_available_tx_desc;
    int16_t                     first_tx_index;
    int16_t                     last_tx_index;
    int16_t                     next_tx_index;
    int16_t                     nb_available_rx_desc;
    int16_t                     next_free_rx_desc_index;
    int16_t                     first_rx_desc_index;
    uint8_t                     phy_addr; /* PHY address for this instance of CoreTSE*/
    tse_wol_callback_t          wol_callback;
} tse_instance_t;

#ifdef __cplusplus
}
#endif

#endif /* CORE_TSE_TYPES_H_ */
