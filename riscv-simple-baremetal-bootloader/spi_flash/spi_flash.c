/*******************************************************************************
*
* (c) Copyright 2013 Microsemi SoC Products Group.  All rights reserved.
*
* Company: Microsemi Corporation
*
* File: spi_flash.c
*
* Description:
*
* Device driver for the on-board SPI flash for SmartFusion KITS Atmel AT25DF641
*
* SVN $Revision: 8241 $
* SVN $Date: 2016-02-15 11:21:27 +0000 (Mon, 15 Feb 2016) $
*
*******************************************************************************/

#include "hw_platform.h"
#include "spi_flash.h"
#include "core_spi.h"
#include <string.h>

#define READ_ARRAY_OPCODE         0x1B
#define DEVICE_ID_READ            0x9F

#define WRITE_ENABLE_CMD          0x06
#define WRITE_DISABLE_CMD         0x04
#define PROGRAM_PAGE_CMD          0x02
#define WRITE_STATUS1_OPCODE      0x01
#define CHIP_ERASE_OPCODE         0x60
#define ERASE_4K_BLOCK_OPCODE     0x20
#define ERASE_32K_BLOCK_OPCODE    0x52
#define ERASE_64K_BLOCK_OPCODE    0xD8
#define READ_STATUS               0x05
#define PROGRAM_RESUME_CMD        0xD0
#define READ_SECTOR_PROTECT       0x3C


#define READY_BIT_MASK            0x01
#define PROTECT_SECTOR_OPCODE     0x36
#define UNPROTECT_SECTOR_OPCODE   0x39

#define DONT_CARE                    0

#define NB_BYTES_PER_PAGE          256

#define BLOCK_ALIGN_MASK_4K      0xFFFFF000
#define BLOCK_ALIGN_MASK_32K     0xFFFF8000
#define BLOCK_ALIGN_MASK_64K     0xFFFF0000

/*
 * Maximum bytes required for command including opcode,
 * address and any dummy bytes.
 */

#define ATMEL_MAX_CMD_BYTES 6
#define ATMEL_MAX_WRITE_BYTES (ATMEL_MAX_CMD_BYTES + NB_BYTES_PER_PAGE)

spi_instance_t g_flash_core_spi;


#define SPI_INSTANCE    &g_flash_core_spi
#define SPI_SLAVE       0

#define SPI_TRANS_BLOCK SPI_transfer_block

/*
 * Our maximum write to the SPI FLASH device will be a 6 byte command
 * and a full 256 byte page of data. We construct this here because the
 * SPI block transfer only supports a single write buffer. To avoid the
 * overhead of buffer copying you could use the driver SPI_transfer_block()
 * as a template for writing a block transfer routine that allowed transfers
 * from two separate buffers without deselecting the slave.
 */
static uint8_t flash_write_buffer[ATMEL_MAX_WRITE_BYTES];

static uint8_t wait_ready( void );
static uint8_t wait_ready_erase( void );

/******************************************************************************
 *For more details please refer the spi_flash.h file
 ******************************************************************************/
spi_flash_status_t spi_flash_init( void )
{
    /*--------------------------------------------------------------------------
     * Configure MSS_SPI.
     */

	SPI_init( SPI_INSTANCE, FLASH_CORE_SPI_BASE, 32 );
	SPI_configure_master_mode( SPI_INSTANCE );
	SPI_set_slave_select( SPI_INSTANCE, SPI_SLAVE );

    return( SPI_FLASH_SUCCESS );
}

/******************************************************************************
 *For more details please refer the spi_flash.h file
 ******************************************************************************/
spi_flash_status_t
spi_flash_control_hw
(
    spi_flash_control_hw_t operation,
    uint32_t param1,
    void *   ptrParam
)
{
	uint8_t x;
    switch(operation){
        case SPI_FLASH_READ_DEVICE_ID:
        {
            uint8_t read_device_id_cmd = DEVICE_ID_READ;
            uint8_t read_buffer[3];
            struct device_Info *ptrDevInfo = (struct device_Info *)ptrParam;
//x=1;

//while(x<0xff)
{
		//read_device_id_cmd = x;
    		SPI_TRANS_BLOCK( SPI_INSTANCE,
                                    &read_device_id_cmd,
                                    1,
                                    read_buffer,
                                    sizeof(read_buffer) );
    		x++;
}

            ptrDevInfo->manufacturer_id = read_buffer[0];
            ptrDevInfo->device_id = read_buffer[1];
            ptrDevInfo->mem_cap = read_buffer[2];

        }
        break;
        case SPI_FLASH_SECTOR_PROTECT:
        {
            uint8_t cmd_buffer[4];
            uint32_t address = param1;

            /* Send Write Enable command */
            cmd_buffer[0] = WRITE_ENABLE_CMD;
            if(wait_ready())
                   return SPI_FLASH_UNSUCCESS;
            SPI_TRANS_BLOCK( SPI_INSTANCE, cmd_buffer, 1, 0, 0 );
            /* protect sector */
            cmd_buffer[0] = PROTECT_SECTOR_OPCODE;
            cmd_buffer[1] = (address >> 16) & 0xFF;
            cmd_buffer[2] = (address >> 8 ) & 0xFF;
            cmd_buffer[3] = address & 0xFF;
            if(wait_ready())
                return SPI_FLASH_UNSUCCESS;

    		SPI_TRANS_BLOCK( SPI_INSTANCE,
                                    cmd_buffer,
                                    sizeof(cmd_buffer),
                                    0,
                                    0 );
        }
        break;
        case SPI_FLASH_SECTOR_UNPROTECT:
        {
            uint8_t cmd_buffer[4];
            uint32_t address = param1;

            /* Send Write Enable command */
            cmd_buffer[0] = WRITE_ENABLE_CMD;
            if(wait_ready())
                return SPI_FLASH_UNSUCCESS;

    		SPI_TRANS_BLOCK( SPI_INSTANCE, cmd_buffer, 1, 0, 0 );

            /* Unprotect sector */
            cmd_buffer[0] = UNPROTECT_SECTOR_OPCODE;
            cmd_buffer[1] = (address >> 16) & 0xFF;
            cmd_buffer[2] = (address >> 8 ) & 0xFF;
            cmd_buffer[3] = address & 0xFF;
            if(wait_ready())
                return SPI_FLASH_UNSUCCESS;

    		SPI_TRANS_BLOCK( SPI_INSTANCE,
                                    cmd_buffer,
                                    sizeof(cmd_buffer),
                                    0,
                                    0 );
        }
        break;

        case SPI_FLASH_GLOBAL_PROTECT:
        case SPI_FLASH_GLOBAL_UNPROTECT:
        {
            uint8_t cmd_buffer[2];
            /* Send Write Enable command */
            cmd_buffer[0] = WRITE_ENABLE_CMD;

            if(wait_ready())
               return SPI_FLASH_UNSUCCESS;

            SPI_TRANS_BLOCK( SPI_INSTANCE, cmd_buffer, 1, 0, 0 );

            /* Send Chip Erase command */
            cmd_buffer[0] = WRITE_STATUS1_OPCODE;
            cmd_buffer[1] = 0;

            if(wait_ready())
               return SPI_FLASH_UNSUCCESS;

    		SPI_TRANS_BLOCK( SPI_INSTANCE, cmd_buffer, 2, 0, 0 );
            if(wait_ready())
                return SPI_FLASH_UNSUCCESS;
        }
        break;
        case SPI_FLASH_CHIP_ERASE:
        {
            uint8_t cmd_buffer;
            /* Send Write Enable command */
            cmd_buffer = WRITE_ENABLE_CMD;

            if(wait_ready())
                return SPI_FLASH_UNSUCCESS;

            SPI_TRANS_BLOCK( SPI_INSTANCE, &cmd_buffer, 1, 0, 0 );

            /* Send Chip Erase command */
            cmd_buffer = CHIP_ERASE_OPCODE;

            if(wait_ready())
                return SPI_FLASH_UNSUCCESS;

            SPI_TRANS_BLOCK( SPI_INSTANCE, &cmd_buffer, 1, 0, 0 );
            if(wait_ready())
                return SPI_FLASH_UNSUCCESS;
        }
        break;
        case SPI_FLASH_RESET:
        {
            uint8_t cmd_buffer;
            /* Send Write Enable command */
            cmd_buffer = 0x66;
    		SPI_TRANS_BLOCK( SPI_INSTANCE, &cmd_buffer, 1, 0, 0 );
    		cmd_buffer = 0x99;
    		SPI_TRANS_BLOCK( SPI_INSTANCE, &cmd_buffer, 1, 0, 0 );
            if(wait_ready())
                return SPI_FLASH_UNSUCCESS;
        }
        break;

        case SPI_FLASH_4KBLOCK_ERASE:
        {
            uint32_t address = param1 & BLOCK_ALIGN_MASK_4K;
            uint8_t cmd_buffer[4];
            /* Send Write Enable command */
            cmd_buffer[0] = WRITE_ENABLE_CMD;

            if(wait_ready())
                return SPI_FLASH_UNSUCCESS;

            SPI_TRANS_BLOCK( SPI_INSTANCE, cmd_buffer, 1, 0, 0 );

            if(wait_ready())
                            return SPI_FLASH_UNSUCCESS;

            /* Send Chip Erase command */
            cmd_buffer[0] = ERASE_4K_BLOCK_OPCODE;
            cmd_buffer[1] = (address >> 16) & 0xFF;
            cmd_buffer[2] = (address >> 8 ) & 0xFF;
            cmd_buffer[3] = address & 0xFF;

            if(wait_ready())
                return SPI_FLASH_UNSUCCESS;

            wait_ready_erase();

            SPI_TRANS_BLOCK( SPI_INSTANCE, cmd_buffer, 4, 0, 0 );
            if(wait_ready())
                return SPI_FLASH_UNSUCCESS;

            wait_ready_erase();

        }
        break;
        case SPI_FLASH_32KBLOCK_ERASE:
        {
            uint32_t address = param1 & BLOCK_ALIGN_MASK_32K;
            uint8_t cmd_buffer[4];
            /* Send Write Enable command */
            cmd_buffer[0] = WRITE_ENABLE_CMD;

            wait_ready();
            SPI_TRANS_BLOCK( SPI_INSTANCE, cmd_buffer, 1, 0, 0 );

            /* Send Chip Erase command */
            cmd_buffer[0] = ERASE_32K_BLOCK_OPCODE;
            cmd_buffer[1] = (address >> 16) & 0xFF;
            cmd_buffer[2] = (address >> 8 ) & 0xFF;
            cmd_buffer[3] = address & 0xFF;

            if(wait_ready())
                return SPI_FLASH_UNSUCCESS;

    		SPI_TRANS_BLOCK( SPI_INSTANCE, cmd_buffer, 4, 0, 0 );
    		if(wait_ready())
                return SPI_FLASH_UNSUCCESS;
        }
        break;
        case SPI_FLASH_64KBLOCK_ERASE:
        {
            uint32_t address = param1 & BLOCK_ALIGN_MASK_64K;
            uint8_t cmd_buffer[4];
            /* Send Write Enable command */
            cmd_buffer[0] = WRITE_ENABLE_CMD;

            if(wait_ready())
                return SPI_FLASH_UNSUCCESS;

            SPI_TRANS_BLOCK( SPI_INSTANCE, cmd_buffer, 1, 0, 0 );

             /* Send Chip Erase command */
            cmd_buffer[0] = ERASE_64K_BLOCK_OPCODE;
            cmd_buffer[1] = (address >> 16) & 0xFF;
            cmd_buffer[2] = (address >> 8 ) & 0xFF;
            cmd_buffer[3] = address & 0xFF;

            if(wait_ready())
                return SPI_FLASH_UNSUCCESS;
            SPI_TRANS_BLOCK( SPI_INSTANCE,
                                    cmd_buffer,
                                    sizeof(cmd_buffer),
                                    0,
                                    0 );
            if(wait_ready())
                return SPI_FLASH_UNSUCCESS;
        }
        break;
        case SPI_FLASH_GET_STATUS:
        {
            uint8_t status[2];
            uint8_t command = READ_STATUS;

    		SPI_TRANS_BLOCK( SPI_INSTANCE,
                                    &command,
                                    sizeof(uint8_t),
                                    status,
                                    2 );

    		*((uint16_t *)ptrParam) = (status[1]) << 8 | status[0];
        }
        break;

        case SPI_FLASH_GET_PROTECT:
        {
            uint8_t cmd_buffer[4];

             /* Send Read Sector Protection Register command */
            cmd_buffer[0] = READ_SECTOR_PROTECT;
            cmd_buffer[1] = (param1 >> 16) & 0xFF;
            cmd_buffer[2] = (param1 >> 8 ) & 0xFF;
            cmd_buffer[3] = param1 & 0xFF;

            if(wait_ready())
                return SPI_FLASH_UNSUCCESS;

            SPI_TRANS_BLOCK( SPI_INSTANCE,
                                    cmd_buffer,
                                    sizeof(cmd_buffer),
                                    (uint8_t *)ptrParam,
                                    1 );

            if(wait_ready())
            {
            	*((uint8_t *)ptrParam) = 1; // Mark as bad result as real one is 0 or 255
                return SPI_FLASH_UNSUCCESS;
            }
        }
        break;

        default:
              return SPI_FLASH_INVALID_ARGUMENTS;
        break;
    }
    return SPI_FLASH_SUCCESS;
}


/******************************************************************************
 *For more details please refer the spi_flash.h file
 ******************************************************************************/
spi_flash_status_t
spi_flash_read
(
    uint32_t address,
    uint8_t * rx_buffer,
    size_t size_in_bytes
)
{
    uint8_t cmd_buffer[6];

    cmd_buffer[0] = 0x03;//READ_ARRAY_OPCODE;
    cmd_buffer[1] = (uint8_t)((address >> 16) & 0xFF);
    cmd_buffer[2] = (uint8_t)((address >> 8) & 0xFF);;
    cmd_buffer[3] = (uint8_t)(address & 0xFF);
    cmd_buffer[4] = DONT_CARE;
    cmd_buffer[5] = DONT_CARE;

    wait_ready_erase();

    if(wait_ready())
        return SPI_FLASH_UNSUCCESS;

    wait_ready_erase();

	SPI_TRANS_BLOCK( SPI_INSTANCE, cmd_buffer, 4/*sizeof(cmd_buffer)*/, rx_buffer, size_in_bytes );
	wait_ready_erase();
    return 0;
}


/*******************************************************************************
 * This function sends the command and data to the FLASH device via SPI.
 */
static void write_cmd_data
(
    spi_instance_t * this_spi,
    const uint8_t * cmd_buffer,
    uint16_t cmd_byte_size,
    uint8_t * data_buffer,
    uint16_t data_byte_size
)
{
    /*
     * Construct our combined command and data block
     */
    if( cmd_byte_size )
    	memcpy( flash_write_buffer, cmd_buffer, cmd_byte_size );

    if( data_byte_size )
    	memcpy( &flash_write_buffer[cmd_byte_size], data_buffer, data_byte_size );

    SPI_TRANS_BLOCK( this_spi, flash_write_buffer, cmd_byte_size + data_byte_size, 0, 0 );
}

/******************************************************************************
 *For more details please refer the spi_flash.h file
 ******************************************************************************/
spi_flash_status_t
spi_flash_write
(
    uint32_t address,
    uint8_t * write_buffer,
    size_t size_in_bytes
)
{
    uint8_t cmd_buffer[4];

    uint32_t in_buffer_idx;
    uint32_t nb_bytes_to_write;
    uint32_t target_addr;

    /* Send Write Enable command */
    cmd_buffer[0] = WRITE_ENABLE_CMD;
    wait_ready();
    SPI_TRANS_BLOCK( SPI_INSTANCE, cmd_buffer, 1, 0, 0 );

    /* Unprotect sector */
    cmd_buffer[0] = UNPROTECT_SECTOR_OPCODE;
    cmd_buffer[1] = (address >> 16) & 0xFF;
    cmd_buffer[2] = (address >> 8 ) & 0xFF;
    cmd_buffer[3] = address & 0xFF;
    wait_ready();
    SPI_TRANS_BLOCK( SPI_INSTANCE, cmd_buffer, sizeof(cmd_buffer), 0, 0 );
    wait_ready_erase();

    /* Send Write Enable command */
    cmd_buffer[0] = WRITE_ENABLE_CMD;
    if(wait_ready())
        return SPI_FLASH_UNSUCCESS;

	SPI_TRANS_BLOCK( SPI_INSTANCE, cmd_buffer, 1, 0, 0 );

    /**/
    in_buffer_idx = 0;
    nb_bytes_to_write = size_in_bytes;
    target_addr = address;

    while ( in_buffer_idx < size_in_bytes )
    {
    	wait_ready_erase();
        uint32_t size_left;
        nb_bytes_to_write = 0x100 - (target_addr & 0xFF);
        /* adjust max possible size to page boundary. */
        size_left = size_in_bytes - in_buffer_idx;
        if ( size_left < nb_bytes_to_write )
        {
            nb_bytes_to_write = size_left;
        }

        if(wait_ready())
            return SPI_FLASH_UNSUCCESS;

        /* Send Write Enable command */
        cmd_buffer[0] = WRITE_ENABLE_CMD;
        SPI_TRANS_BLOCK( SPI_INSTANCE, cmd_buffer, 1, 0, 0 );

        /* Program page */
        if(wait_ready())
            return SPI_FLASH_UNSUCCESS;


        cmd_buffer[0] = PROGRAM_PAGE_CMD;
        cmd_buffer[1] = (target_addr >> 16) & 0xFF;
        cmd_buffer[2] = (target_addr >> 8 ) & 0xFF;
        cmd_buffer[3] = target_addr & 0xFF;

        write_cmd_data
          (
            SPI_INSTANCE,
            cmd_buffer,
            sizeof(cmd_buffer),
            &write_buffer[in_buffer_idx],
            nb_bytes_to_write
          );

        target_addr += nb_bytes_to_write;
        in_buffer_idx += nb_bytes_to_write;
        wait_ready_erase();
    }

    /* Send Write Disable command. */
    cmd_buffer[0] = WRITE_DISABLE_CMD;

    if(wait_ready())
        return SPI_FLASH_UNSUCCESS;

    SPI_TRANS_BLOCK( SPI_INSTANCE, cmd_buffer, 1, 0, 0 );
    return 0;
}


/******************************************************************************
 * This function waits for the SPI operation to complete
 ******************************************************************************/
static uint8_t wait_ready( void )
{
    uint32_t count = 0;
    uint8_t ready_bit;
    uint8_t command = READ_STATUS;
#if 1
    do {
        SPI_TRANS_BLOCK(SPI_INSTANCE, &command, 1, &ready_bit, 1);
        ready_bit = ready_bit & READY_BIT_MASK;
        count++;
    } while((ready_bit & READY_BIT_MASK) /*&& (count <= 0x7FFFFFFF)*/);
#endif
    return (ready_bit);
}

static uint8_t wait_ready_erase( void )
{
    uint32_t count = 0;
    uint8_t ready_bit;
    uint8_t command = 0x70 ; // FLAG_READ_STATUS;
#if 1
    do {
        SPI_TRANS_BLOCK(SPI_INSTANCE, &command, 1, &ready_bit, 1);
        count++;
    } while((ready_bit & 0x80) == 0);
#endif
    return (ready_bit);
}
