/*******************************************************************************
 * (c) Copyright 2013-2015 Microsemi SoC Products Group.  All rights reserved.
 *
 *
 *
 * SVN $Revision: 8673 $
 * SVN $Date: 2016-11-21 11:20:37 +0530 (Mon, 21 Nov 2016) $
 */
#include "drivers/CoreTSE/core_tse.h"

/*==============================================================================
 *
 */
const uint8_t * sys_cfg_get_mac_address(void)
{
    static uint8_t mac_address[6];

    mac_address[0] = 0xC0u;
    mac_address[1] = 0xB1u;
    mac_address[2] = 0x3Cu;
    mac_address[3] = 0x61u;
    mac_address[4] = 0x60u;
    mac_address[5] = 0x60u;

    return (const uint8_t *)mac_address;
}


