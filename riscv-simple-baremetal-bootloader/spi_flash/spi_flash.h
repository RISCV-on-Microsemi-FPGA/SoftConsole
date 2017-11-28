 /******************************************************************************
 *
 * (c) Copyright 2013 Microsemi SoC Products Group.  All rights reserved.
 *
 * Company: Microsemi Corporation
 *
 * File: spi_flash.h
 *
 * Note:
 *
 * This is a non DMA version of the flash driver for the CoreSPI 4.2.xx
 * based on the driver for MSS SPI based driver from MPM 6.
 *
 * The SPI_FLASH_GET_STATUS command now returns the 2 bytes of the status
 * via the pointer parameter and the PI_FLASH_GET_PROTECT command has been
 * added.
 *
 * SVN $Revision: 8241 $
 * SVN $Date: 2016-02-15 11:21:27 +0000 (Mon, 15 Feb 2016) $
 *
 *******************************************************************************/

#ifndef __AT25DF641_SPI_FLASH_H_
#define __AT25DF641_SPI_FLASH_H_

#include <stdint.h>
#include <stdlib.h>

/*******************************************************************************
 * Possible return values from functions on SPI FLASH.
 ******************************************************************************/
typedef enum {
    SPI_FLASH_SUCCESS = 0,
    SPI_FLASH_PROTECTION_ERROR,
    SPI_FLASH_WRITE_ERROR,
    SPI_FLASH_INVALID_ARGUMENTS,
    SPI_FLASH_INVALID_ADDRESS,
    SPI_FLASH_UNSUCCESS
} spi_flash_status_t;

/*******************************************************************************
 * Possible HW Control commands on SPI FLASH.
 ******************************************************************************/
typedef enum {
    SPI_FLASH_SECTOR_UNPROTECT = 0,
    SPI_FLASH_SECTOR_PROTECT,
    SPI_FLASH_GLOBAL_UNPROTECT,
    SPI_FLASH_GLOBAL_PROTECT,
    SPI_FLASH_GET_STATUS,
    SPI_FLASH_4KBLOCK_ERASE,
    SPI_FLASH_32KBLOCK_ERASE,
    SPI_FLASH_64KBLOCK_ERASE,
    SPI_FLASH_CHIP_ERASE,
    SPI_FLASH_READ_DEVICE_ID,
    SPI_FLASH_RESET,
    SPI_FLASH_GET_PROTECT
/*
    SPI_FLASH_SECTOR_LOCKDOWN,
    SPI_FLASH_FREEZE_SECTOR_LOCKDOWN
*/
} spi_flash_control_hw_t;

struct device_Info{
    uint8_t manufacturer_id;
    uint8_t device_id;
    uint8_t mem_cap;
};

/*******************************************************************************
 * This function initialzes the SPI peripheral for data transfer
 ******************************************************************************/
spi_flash_status_t
spi_flash_init
(
    void
);

/******************************************************************************
 * This function performs the various operations on the serial Flash
 * based on the command passed as first parameter.
 * The operation of the each command is explained below.
 *
 * @param operation
 *        The operations supported are as per the enum spi_flash_control_hw_t
 *        defined above. The functionality is as follows:
 *
 *        1. SPI_FLASH_SECTOR_UNPROTECT: Every 64KBytes are represented
 *           in sectors. There is a corresponding bits set for protection
 *           of that sector. To do modify operations like write and erase
 *           we need to call this operation to unprotect the block.
 *           The second parameter 'param1' for this function is the block
 *           address to unprotect.
 *
 *        2. SPI_FLASH_SECTOR_PROTECT :  Every 64KBytes are represented
 *           in sectors. There is a corresponding bits set for protection
 *           of that sector. To protect from the modify operations like
 *           write and erase we need to call this operation
 *           to protect the block. The second parameter 'param1' for this
 *           function is the block address to protect.
 *
 *        3. SPI_FLASH_GLOBAL_UNPROTECT: This command is used to unprotect
 *           the entire flash for modify operations.
 *
 *        4. SPI_FLASH_GLOBAL_PROTECT: This command is used to protect/lock
 *           the entire flash from modify operations.
 *
 *        5. SPI_FLASH_GET_STATUS: This function used to get the SPI Flash
 *           status register content for more details of the status bits
 *           refer to the data sheet for the AT25DF641. The second parameter
 *           is ignored and the third parameter points to an unsigned 16 bit
 *           value to store the status in. The first status byte is in b0-7
 *           and the second status byte in b8-15.
 *
 *        6. SPI_FLASH_4KBLOCK_ERASE: This command is used to erase the block
 *           starting at 4KB boundary. The starting address of the 4K Block is
 *           passed in the second parameter param1 of this API.
 *
 *        7. SPI_FLASH_32KBLOCK_ERASE: This command is used to erase the block
 *           starting at 32KB boundary. The starting address of the 32K Block
 *           is passed in the second parameter param1 of this API.
 *
 *        8. SPI_FLASH_64KBLOCK_ERASE: This command is used to erase the block
 *           starting at 64KB boundary. The starting address of the 64K Block
 *           is passed in the second parameter peram1 of this API.
 *
 *        9. SPI_FLASH_CHIP_ERASE This command is used to erase the entire flash chip.
 *
 *        10. SPI_FLASH_READ_DEVICE_ID: This command is used to read the
 *            device properties. The values are filled in the third parameter
 *            'ptrParam' of this API,
 *
 *        11. SPI_FLASH_RESET: In some cases it may be necessary to prematurely terminate
 *            a program or erase cycle early rather than wait the hundreds of microseconds or
 *            milliseconds necessary for the program or erase operation to complete normally.
 *            The Reset command allows a program or erase operation in progress to be ended
 *            abruptly and returns the device to an idle state.
 *
 *        12. SPI_FLASH_GET_PROTECT: Read the sector protection register for the sector in.
 *            question. Should be 0x00 if unprotected and 0xFF if protected. The second
 *            parameter is an address within the 64K sector we are examining. The third
 *            parameter is a pointer to an unsigned 8 bit location to store the protection
 *            register value in.
 *
 * @param param1        The param1 usage is explained in the above description according
 *                      to command in use.
 * @param ptrParam      The ptrParam usage is explained in the above description according
 *                      to command in use.
 * @return              The return value indicates if the write was successful.
 *                      Possible values are:
 *                      SPI_FLASH_SUCCESS,
 *                      SPI_FLASH_PROTECTION_ERROR,
 *                      SPI_FLASH_INVALID_ARGUMENTS,
 *                      SPI_FLASH_INVALID_ADDRESS,
 *                      SPI_FLASH_UNSUCCESS
 *
 *                      SPI_FLASH_SUCCESS: describes the SPI Flash operation is
 *                      correct and complete
 *
 *                      SPI_FLASH_PROTECTION_ERROR: The sector is under protected and
 *                      not allowing the operation.
 *                      We need to do the unprotect and do the operation
 *
 *                      SPI_FLASH_INVALID_ARGUMENTS: describes that function has received
 *                      Invalid arguments
 *
 *                      SPI_FLASH_INVALID_ADDRESS: describes that function has received
 *                      Invalid address
 *
 *                      SPI_FLASH_UNSUCCESS: describes the SPI Flash operation is
 *                      incomplete
 */

spi_flash_status_t
spi_flash_control_hw
(
    spi_flash_control_hw_t operation,
    uint32_t peram1,
    void *   ptrPeram
);

/*******************************************************************************
 * This function reads the content from the serial Flash.
 * The data is read from the memory location specified by the first parameter.
 * This address is ranges from 0 to SPI Flash Size. This address range is not
 * the processors absolute range.
 *
 * @param start_addr    This is the address at which data willbe read.
 *                      This address is ranges from 0 to SPI Flash Size.
 *                      This address range is not the processors absolute range.
 * @param p_data        This is a pointer to the buffer for holding the read data.
 * @param nb_bytes      This is the number of bytes to be read from SPI Flash.
 * @return              The return value indicates if the write was successful.
 *                      Possible values are:
 *                      SPI_FLASH_SUCCESS,
 *                      SPI_FLASH_PROTECTION_ERROR,
 *                      SPI_FLASH_INVALID_ARGUMENTS,
 *                      SPI_FLASH_INVALID_ADDRESS,
 *                      SPI_FLASH_UNSUCCESS
 *
 *                      SPI_FLASH_SUCCESS: describes the SPI Flash operation is
 *                      correct and complete
 *
 *                      SPI_FLASH_INVALID_ARGUMENTS: describes that function has received
 *                      Invalid arguments
 *
 *                      SPI_FLASH_INVALID_ADDRESS: describes that function has received
 *                      Invalid address
 *
 *                      SPI_FLASH_UNSUCCESS: describes the SPI Flash operation is
 *                      incomplete
 */
spi_flash_status_t
spi_flash_read
(
    uint32_t address,
    uint8_t * rx_buffer,
    size_t size_in_bytes
);

/*******************************************************************************
 * This function writes the content of the buffer passed as parameter to
 * Serial Flash through SPI. The data is written from the memory location specified
 * by the first parameter.
 * This address is ranges from 0 to SPI Flash Size. This address range is not
 * the processors absolute range
 *
 * @param start_addr    This is the address at which data will be written.
 *                      This address is ranges from 0 to SPI Flash Size.
 *                      This address range is not the processors absolute range
 * @param p_data        This is a pointer to the buffer holding the data to be
 *                      written into Serial Flash.
 * @param nb_bytes      This is the number of bytes to be written into Serial Flash.
 * @return              The return value indicates if the write was successful.
 *                      Possible values are:
 *                      SPI_FLASH_SUCCESS,
 *                      SPI_FLASH_PROTECTION_ERROR,
 *                      SPI_FLASH_WRITE_ERROR,
 *                      SPI_FLASH_INVALID_ARGUMENTS,
 *                      SPI_FLASH_INVALID_ADDRESS,
 *                      SPI_FLASH_UNSUCCESS
 *
 *                      SPI_FLASH_SUCCESS: describes the SPI Flash operation is
 *                      correct and complete
 *
 *                      SPI_FLASH_PROTECTION_ERROR: The sector is under protected and
 *                      not allowing the operation.
 *                      We need to do the unprotect and do the operation
 *
 *                      SPI_FLASH_WRITE_ERROR: describes the SPI Flash write operation is
 *                      failed
 *
 *                      SPI_FLASH_INVALID_ARGUMENTS: describes that function has received
 *                      Invalid arguments
 *
 *                      SPI_FLASH_INVALID_ADDRESS: describes that function has received
 *                      Invalid address. Address range should be between 0 to 8 MB
 *
 *                      SPI_FLASH_UNSUCCESS: describes the SPI Flash operation is
 *                      incomplete
 */

spi_flash_status_t
spi_flash_write
(
    uint32_t address,
    uint8_t * write_buffer,
    size_t size_in_bytes
);

#endif
