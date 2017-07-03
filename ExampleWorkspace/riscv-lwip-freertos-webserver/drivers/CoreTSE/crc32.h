/***************************************************************************//**
 * (c) Copyright 2014-2016 Microsemi SoC Products Group.  All rights reserved.
 * crc32 header file.
 *
 * SVN $Revision: 8432 $
 * SVN $Date: 2016-06-20 15:56:50 +0530 (Mon, 20 Jun 2016) $
 *
 ******************************************************************************/

#ifndef __CRC32_H
#define __CRC32_H    1

#include <cpu_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Calculates 32 bits CRC value of given data */
uint32_t
TSE_crc32
(
    uint32_t value,
    const uint8_t *data,
    uint32_t data_length
);

/*Calculates 32 bit CRC value of given data,using standard Ethernet CRC function*/
uint32_t
TSE_ethernet_crc
(
    const uint8_t *data,
    uint32_t data_length
);

#ifdef __cplusplus
}
#endif

#endif    /* __CRC32_H */
