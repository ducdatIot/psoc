/***************************************************************************//**
* \file i2c_ds1307_BOOT.h
* \version 4.0
*
* \brief
*  This file provides constants and parameter values of the bootloader
*  communication APIs for the SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2014-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_BOOT_i2c_ds1307_H)
#define CY_SCB_BOOT_i2c_ds1307_H

#include "i2c_ds1307_PVT.h"

#if (i2c_ds1307_SCB_MODE_I2C_INC)
    #include "i2c_ds1307_I2C.h"
#endif /* (i2c_ds1307_SCB_MODE_I2C_INC) */

#if (i2c_ds1307_SCB_MODE_EZI2C_INC)
    #include "i2c_ds1307_EZI2C.h"
#endif /* (i2c_ds1307_SCB_MODE_EZI2C_INC) */

#if (i2c_ds1307_SCB_MODE_SPI_INC || i2c_ds1307_SCB_MODE_UART_INC)
    #include "i2c_ds1307_SPI_UART.h"
#endif /* (i2c_ds1307_SCB_MODE_SPI_INC || i2c_ds1307_SCB_MODE_UART_INC) */


/***************************************
*  Conditional Compilation Parameters
****************************************/

/* Bootloader communication interface enable */
#define i2c_ds1307_BTLDR_COMM_ENABLED ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_i2c_ds1307) || \
                                             (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

/* Enable I2C bootloader communication */
#if (i2c_ds1307_SCB_MODE_I2C_INC)
    #define i2c_ds1307_I2C_BTLDR_COMM_ENABLED     (i2c_ds1307_BTLDR_COMM_ENABLED && \
                                                            (i2c_ds1307_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             i2c_ds1307_I2C_SLAVE_CONST))
#else
     #define i2c_ds1307_I2C_BTLDR_COMM_ENABLED    (0u)
#endif /* (i2c_ds1307_SCB_MODE_I2C_INC) */

/* EZI2C does not support bootloader communication. Provide empty APIs */
#if (i2c_ds1307_SCB_MODE_EZI2C_INC)
    #define i2c_ds1307_EZI2C_BTLDR_COMM_ENABLED   (i2c_ds1307_BTLDR_COMM_ENABLED && \
                                                         i2c_ds1307_SCB_MODE_UNCONFIG_CONST_CFG)
#else
    #define i2c_ds1307_EZI2C_BTLDR_COMM_ENABLED   (0u)
#endif /* (i2c_ds1307_EZI2C_BTLDR_COMM_ENABLED) */

/* Enable SPI bootloader communication */
#if (i2c_ds1307_SCB_MODE_SPI_INC)
    #define i2c_ds1307_SPI_BTLDR_COMM_ENABLED     (i2c_ds1307_BTLDR_COMM_ENABLED && \
                                                            (i2c_ds1307_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             i2c_ds1307_SPI_SLAVE_CONST))
#else
        #define i2c_ds1307_SPI_BTLDR_COMM_ENABLED (0u)
#endif /* (i2c_ds1307_SPI_BTLDR_COMM_ENABLED) */

/* Enable UART bootloader communication */
#if (i2c_ds1307_SCB_MODE_UART_INC)
       #define i2c_ds1307_UART_BTLDR_COMM_ENABLED    (i2c_ds1307_BTLDR_COMM_ENABLED && \
                                                            (i2c_ds1307_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             (i2c_ds1307_UART_RX_DIRECTION && \
                                                              i2c_ds1307_UART_TX_DIRECTION)))
#else
     #define i2c_ds1307_UART_BTLDR_COMM_ENABLED   (0u)
#endif /* (i2c_ds1307_UART_BTLDR_COMM_ENABLED) */

/* Enable bootloader communication */
#define i2c_ds1307_BTLDR_COMM_MODE_ENABLED    (i2c_ds1307_I2C_BTLDR_COMM_ENABLED   || \
                                                     i2c_ds1307_SPI_BTLDR_COMM_ENABLED   || \
                                                     i2c_ds1307_EZI2C_BTLDR_COMM_ENABLED || \
                                                     i2c_ds1307_UART_BTLDR_COMM_ENABLED)


/***************************************
*        Function Prototypes
***************************************/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (i2c_ds1307_I2C_BTLDR_COMM_ENABLED)
    /* I2C Bootloader physical layer functions */
    void i2c_ds1307_I2CCyBtldrCommStart(void);
    void i2c_ds1307_I2CCyBtldrCommStop (void);
    void i2c_ds1307_I2CCyBtldrCommReset(void);
    cystatus i2c_ds1307_I2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus i2c_ds1307_I2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map I2C specific bootloader communication APIs to SCB specific APIs */
    #if (i2c_ds1307_SCB_MODE_I2C_CONST_CFG)
        #define i2c_ds1307_CyBtldrCommStart   i2c_ds1307_I2CCyBtldrCommStart
        #define i2c_ds1307_CyBtldrCommStop    i2c_ds1307_I2CCyBtldrCommStop
        #define i2c_ds1307_CyBtldrCommReset   i2c_ds1307_I2CCyBtldrCommReset
        #define i2c_ds1307_CyBtldrCommRead    i2c_ds1307_I2CCyBtldrCommRead
        #define i2c_ds1307_CyBtldrCommWrite   i2c_ds1307_I2CCyBtldrCommWrite
    #endif /* (i2c_ds1307_SCB_MODE_I2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (i2c_ds1307_I2C_BTLDR_COMM_ENABLED) */


#if defined(CYDEV_BOOTLOADER_IO_COMP) && (i2c_ds1307_EZI2C_BTLDR_COMM_ENABLED)
    /* Bootloader physical layer functions */
    void i2c_ds1307_EzI2CCyBtldrCommStart(void);
    void i2c_ds1307_EzI2CCyBtldrCommStop (void);
    void i2c_ds1307_EzI2CCyBtldrCommReset(void);
    cystatus i2c_ds1307_EzI2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus i2c_ds1307_EzI2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map EZI2C specific bootloader communication APIs to SCB specific APIs */
    #if (i2c_ds1307_SCB_MODE_EZI2C_CONST_CFG)
        #define i2c_ds1307_CyBtldrCommStart   i2c_ds1307_EzI2CCyBtldrCommStart
        #define i2c_ds1307_CyBtldrCommStop    i2c_ds1307_EzI2CCyBtldrCommStop
        #define i2c_ds1307_CyBtldrCommReset   i2c_ds1307_EzI2CCyBtldrCommReset
        #define i2c_ds1307_CyBtldrCommRead    i2c_ds1307_EzI2CCyBtldrCommRead
        #define i2c_ds1307_CyBtldrCommWrite   i2c_ds1307_EzI2CCyBtldrCommWrite
    #endif /* (i2c_ds1307_SCB_MODE_EZI2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (i2c_ds1307_EZI2C_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (i2c_ds1307_SPI_BTLDR_COMM_ENABLED)
    /* SPI Bootloader physical layer functions */
    void i2c_ds1307_SpiCyBtldrCommStart(void);
    void i2c_ds1307_SpiCyBtldrCommStop (void);
    void i2c_ds1307_SpiCyBtldrCommReset(void);
    cystatus i2c_ds1307_SpiCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus i2c_ds1307_SpiCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map SPI specific bootloader communication APIs to SCB specific APIs */
    #if (i2c_ds1307_SCB_MODE_SPI_CONST_CFG)
        #define i2c_ds1307_CyBtldrCommStart   i2c_ds1307_SpiCyBtldrCommStart
        #define i2c_ds1307_CyBtldrCommStop    i2c_ds1307_SpiCyBtldrCommStop
        #define i2c_ds1307_CyBtldrCommReset   i2c_ds1307_SpiCyBtldrCommReset
        #define i2c_ds1307_CyBtldrCommRead    i2c_ds1307_SpiCyBtldrCommRead
        #define i2c_ds1307_CyBtldrCommWrite   i2c_ds1307_SpiCyBtldrCommWrite
    #endif /* (i2c_ds1307_SCB_MODE_SPI_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (i2c_ds1307_SPI_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (i2c_ds1307_UART_BTLDR_COMM_ENABLED)
    /* UART Bootloader physical layer functions */
    void i2c_ds1307_UartCyBtldrCommStart(void);
    void i2c_ds1307_UartCyBtldrCommStop (void);
    void i2c_ds1307_UartCyBtldrCommReset(void);
    cystatus i2c_ds1307_UartCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus i2c_ds1307_UartCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map UART specific bootloader communication APIs to SCB specific APIs */
    #if (i2c_ds1307_SCB_MODE_UART_CONST_CFG)
        #define i2c_ds1307_CyBtldrCommStart   i2c_ds1307_UartCyBtldrCommStart
        #define i2c_ds1307_CyBtldrCommStop    i2c_ds1307_UartCyBtldrCommStop
        #define i2c_ds1307_CyBtldrCommReset   i2c_ds1307_UartCyBtldrCommReset
        #define i2c_ds1307_CyBtldrCommRead    i2c_ds1307_UartCyBtldrCommRead
        #define i2c_ds1307_CyBtldrCommWrite   i2c_ds1307_UartCyBtldrCommWrite
    #endif /* (i2c_ds1307_SCB_MODE_UART_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (i2c_ds1307_UART_BTLDR_COMM_ENABLED) */

/**
* \addtogroup group_bootloader
* @{
*/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (i2c_ds1307_BTLDR_COMM_ENABLED)
    #if (i2c_ds1307_SCB_MODE_UNCONFIG_CONST_CFG)
        /* Bootloader physical layer functions */
        void i2c_ds1307_CyBtldrCommStart(void);
        void i2c_ds1307_CyBtldrCommStop (void);
        void i2c_ds1307_CyBtldrCommReset(void);
        cystatus i2c_ds1307_CyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
        cystatus i2c_ds1307_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    #endif /* (i2c_ds1307_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Map SCB specific bootloader communication APIs to common APIs */
    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_i2c_ds1307)
        #define CyBtldrCommStart    i2c_ds1307_CyBtldrCommStart
        #define CyBtldrCommStop     i2c_ds1307_CyBtldrCommStop
        #define CyBtldrCommReset    i2c_ds1307_CyBtldrCommReset
        #define CyBtldrCommWrite    i2c_ds1307_CyBtldrCommWrite
        #define CyBtldrCommRead     i2c_ds1307_CyBtldrCommRead
    #endif /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_i2c_ds1307) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (i2c_ds1307_BTLDR_COMM_ENABLED) */

/** @} group_bootloader */

/***************************************
*           API Constants
***************************************/

/* Timeout unit in milliseconds */
#define i2c_ds1307_WAIT_1_MS  (1u)

/* Return number of bytes to copy into bootloader buffer */
#define i2c_ds1307_BYTES_TO_COPY(actBufSize, bufSize) \
                            ( ((uint32)(actBufSize) < (uint32)(bufSize)) ? \
                                ((uint32) (actBufSize)) : ((uint32) (bufSize)) )

/* Size of Read/Write buffers for I2C bootloader  */
#define i2c_ds1307_I2C_BTLDR_SIZEOF_READ_BUFFER   (64u)
#define i2c_ds1307_I2C_BTLDR_SIZEOF_WRITE_BUFFER  (64u)

/* Byte to byte time interval: calculated basing on current component
* data rate configuration, can be defined in project if required.
*/
#ifndef i2c_ds1307_SPI_BYTE_TO_BYTE
    #define i2c_ds1307_SPI_BYTE_TO_BYTE   (160u)
#endif

/* Byte to byte time interval: calculated basing on current component
* baud rate configuration, can be defined in the project if required.
*/
#ifndef i2c_ds1307_UART_BYTE_TO_BYTE
    #define i2c_ds1307_UART_BYTE_TO_BYTE  (2500u)
#endif /* i2c_ds1307_UART_BYTE_TO_BYTE */

#endif /* (CY_SCB_BOOT_i2c_ds1307_H) */


/* [] END OF FILE */
