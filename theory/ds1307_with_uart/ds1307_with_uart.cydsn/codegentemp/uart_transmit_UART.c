/***************************************************************************//**
* \file uart_transmit_UART.c
* \version 4.0
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  UART mode.
*
* Note:
*
*******************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "uart_transmit_PVT.h"
#include "uart_transmit_SPI_UART_PVT.h"
#include "cyapicallbacks.h"

#if (uart_transmit_UART_WAKE_ENABLE_CONST && uart_transmit_UART_RX_WAKEUP_IRQ)
    /**
    * \addtogroup group_globals
    * \{
    */
    /** This global variable determines whether to enable Skip Start
    * functionality when uart_transmit_Sleep() function is called:
    * 0 – disable, other values – enable. Default value is 1.
    * It is only available when Enable wakeup from Deep Sleep Mode is enabled.
    */
    uint8 uart_transmit_skipStart = 1u;
    /** \} globals */
#endif /* (uart_transmit_UART_WAKE_ENABLE_CONST && uart_transmit_UART_RX_WAKEUP_IRQ) */

#if(uart_transmit_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    const uart_transmit_UART_INIT_STRUCT uart_transmit_configUart =
    {
        uart_transmit_UART_SUB_MODE,
        uart_transmit_UART_DIRECTION,
        uart_transmit_UART_DATA_BITS_NUM,
        uart_transmit_UART_PARITY_TYPE,
        uart_transmit_UART_STOP_BITS_NUM,
        uart_transmit_UART_OVS_FACTOR,
        uart_transmit_UART_IRDA_LOW_POWER,
        uart_transmit_UART_MEDIAN_FILTER_ENABLE,
        uart_transmit_UART_RETRY_ON_NACK,
        uart_transmit_UART_IRDA_POLARITY,
        uart_transmit_UART_DROP_ON_PARITY_ERR,
        uart_transmit_UART_DROP_ON_FRAME_ERR,
        uart_transmit_UART_WAKE_ENABLE,
        0u,
        NULL,
        0u,
        NULL,
        uart_transmit_UART_MP_MODE_ENABLE,
        uart_transmit_UART_MP_ACCEPT_ADDRESS,
        uart_transmit_UART_MP_RX_ADDRESS,
        uart_transmit_UART_MP_RX_ADDRESS_MASK,
        (uint32) uart_transmit_SCB_IRQ_INTERNAL,
        uart_transmit_UART_INTR_RX_MASK,
        uart_transmit_UART_RX_TRIGGER_LEVEL,
        uart_transmit_UART_INTR_TX_MASK,
        uart_transmit_UART_TX_TRIGGER_LEVEL,
        (uint8) uart_transmit_UART_BYTE_MODE_ENABLE,
        (uint8) uart_transmit_UART_CTS_ENABLE,
        (uint8) uart_transmit_UART_CTS_POLARITY,
        (uint8) uart_transmit_UART_RTS_POLARITY,
        (uint8) uart_transmit_UART_RTS_FIFO_LEVEL,
        (uint8) uart_transmit_UART_RX_BREAK_WIDTH
    };


    /*******************************************************************************
    * Function Name: uart_transmit_UartInit
    ****************************************************************************//**
    *
    *  Configures the uart_transmit for UART operation.
    *
    *  This function is intended specifically to be used when the uart_transmit
    *  configuration is set to “Unconfigured uart_transmit” in the customizer.
    *  After initializing the uart_transmit in UART mode using this function,
    *  the component can be enabled using the uart_transmit_Start() or
    * uart_transmit_Enable() function.
    *  This function uses a pointer to a structure that provides the configuration
    *  settings. This structure contains the same information that would otherwise
    *  be provided by the customizer settings.
    *
    *  \param config: pointer to a structure that contains the following list of
    *   fields. These fields match the selections available in the customizer.
    *   Refer to the customizer for further description of the settings.
    *
    *******************************************************************************/
    void uart_transmit_UartInit(const uart_transmit_UART_INIT_STRUCT *config)
    {
        uint32 pinsConfig;

        if (NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due to bad function parameter */
        }
        else
        {
            /* Get direction to configure UART pins: TX, RX or TX+RX */
            pinsConfig  = config->direction;

        #if !(uart_transmit_CY_SCBIP_V0 || uart_transmit_CY_SCBIP_V1)
            /* Add RTS and CTS pins to configure */
            pinsConfig |= (0u != config->rtsRxFifoLevel) ? (uart_transmit_UART_RTS_PIN_ENABLE) : (0u);
            pinsConfig |= (0u != config->enableCts)      ? (uart_transmit_UART_CTS_PIN_ENABLE) : (0u);
        #endif /* !(uart_transmit_CY_SCBIP_V0 || uart_transmit_CY_SCBIP_V1) */

            /* Configure pins */
            uart_transmit_SetPins(uart_transmit_SCB_MODE_UART, config->mode, pinsConfig);

            /* Store internal configuration */
            uart_transmit_scbMode       = (uint8) uart_transmit_SCB_MODE_UART;
            uart_transmit_scbEnableWake = (uint8) config->enableWake;
            uart_transmit_scbEnableIntr = (uint8) config->enableInterrupt;

            /* Set RX direction internal variables */
            uart_transmit_rxBuffer      =         config->rxBuffer;
            uart_transmit_rxDataBits    = (uint8) config->dataBits;
            uart_transmit_rxBufferSize  =         config->rxBufferSize;

            /* Set TX direction internal variables */
            uart_transmit_txBuffer      =         config->txBuffer;
            uart_transmit_txDataBits    = (uint8) config->dataBits;
            uart_transmit_txBufferSize  =         config->txBufferSize;

            /* Configure UART interface */
            if(uart_transmit_UART_MODE_IRDA == config->mode)
            {
                /* OVS settings: IrDA */
                uart_transmit_CTRL_REG  = ((0u != config->enableIrdaLowPower) ?
                                                (uart_transmit_UART_GET_CTRL_OVS_IRDA_LP(config->oversample)) :
                                                (uart_transmit_CTRL_OVS_IRDA_OVS16));
            }
            else
            {
                /* OVS settings: UART and SmartCard */
                uart_transmit_CTRL_REG  = uart_transmit_GET_CTRL_OVS(config->oversample);
            }

            uart_transmit_CTRL_REG     |= uart_transmit_GET_CTRL_BYTE_MODE  (config->enableByteMode)      |
                                             uart_transmit_GET_CTRL_ADDR_ACCEPT(config->multiprocAcceptAddr) |
                                             uart_transmit_CTRL_UART;

            /* Configure sub-mode: UART, SmartCard or IrDA */
            uart_transmit_UART_CTRL_REG = uart_transmit_GET_UART_CTRL_MODE(config->mode);

            /* Configure RX direction */
            uart_transmit_UART_RX_CTRL_REG = uart_transmit_GET_UART_RX_CTRL_MODE(config->stopBits)              |
                                        uart_transmit_GET_UART_RX_CTRL_POLARITY(config->enableInvertedRx)          |
                                        uart_transmit_GET_UART_RX_CTRL_MP_MODE(config->enableMultiproc)            |
                                        uart_transmit_GET_UART_RX_CTRL_DROP_ON_PARITY_ERR(config->dropOnParityErr) |
                                        uart_transmit_GET_UART_RX_CTRL_DROP_ON_FRAME_ERR(config->dropOnFrameErr)   |
                                        uart_transmit_GET_UART_RX_CTRL_BREAK_WIDTH(config->breakWidth);

            if(uart_transmit_UART_PARITY_NONE != config->parity)
            {
               uart_transmit_UART_RX_CTRL_REG |= uart_transmit_GET_UART_RX_CTRL_PARITY(config->parity) |
                                                    uart_transmit_UART_RX_CTRL_PARITY_ENABLED;
            }

            uart_transmit_RX_CTRL_REG      = uart_transmit_GET_RX_CTRL_DATA_WIDTH(config->dataBits)       |
                                                uart_transmit_GET_RX_CTRL_MEDIAN(config->enableMedianFilter) |
                                                uart_transmit_GET_UART_RX_CTRL_ENABLED(config->direction);

            uart_transmit_RX_FIFO_CTRL_REG = uart_transmit_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(config->rxTriggerLevel);

            /* Configure MP address */
            uart_transmit_RX_MATCH_REG     = uart_transmit_GET_RX_MATCH_ADDR(config->multiprocAddr) |
                                                uart_transmit_GET_RX_MATCH_MASK(config->multiprocAddrMask);

            /* Configure RX direction */
            uart_transmit_UART_TX_CTRL_REG = uart_transmit_GET_UART_TX_CTRL_MODE(config->stopBits) |
                                                uart_transmit_GET_UART_TX_CTRL_RETRY_NACK(config->enableRetryNack);

            if(uart_transmit_UART_PARITY_NONE != config->parity)
            {
               uart_transmit_UART_TX_CTRL_REG |= uart_transmit_GET_UART_TX_CTRL_PARITY(config->parity) |
                                                    uart_transmit_UART_TX_CTRL_PARITY_ENABLED;
            }

            uart_transmit_TX_CTRL_REG      = uart_transmit_GET_TX_CTRL_DATA_WIDTH(config->dataBits)    |
                                                uart_transmit_GET_UART_TX_CTRL_ENABLED(config->direction);

            uart_transmit_TX_FIFO_CTRL_REG = uart_transmit_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(config->txTriggerLevel);

        #if !(uart_transmit_CY_SCBIP_V0 || uart_transmit_CY_SCBIP_V1)
            uart_transmit_UART_FLOW_CTRL_REG = uart_transmit_GET_UART_FLOW_CTRL_CTS_ENABLE(config->enableCts) | \
                                            uart_transmit_GET_UART_FLOW_CTRL_CTS_POLARITY (config->ctsPolarity)  | \
                                            uart_transmit_GET_UART_FLOW_CTRL_RTS_POLARITY (config->rtsPolarity)  | \
                                            uart_transmit_GET_UART_FLOW_CTRL_TRIGGER_LEVEL(config->rtsRxFifoLevel);
        #endif /* !(uart_transmit_CY_SCBIP_V0 || uart_transmit_CY_SCBIP_V1) */

            /* Configure interrupt with UART handler but do not enable it */
            CyIntDisable    (uart_transmit_ISR_NUMBER);
            CyIntSetPriority(uart_transmit_ISR_NUMBER, uart_transmit_ISR_PRIORITY);
            (void) CyIntSetVector(uart_transmit_ISR_NUMBER, &uart_transmit_SPI_UART_ISR);

            /* Configure WAKE interrupt */
        #if(uart_transmit_UART_RX_WAKEUP_IRQ)
            CyIntDisable    (uart_transmit_RX_WAKE_ISR_NUMBER);
            CyIntSetPriority(uart_transmit_RX_WAKE_ISR_NUMBER, uart_transmit_RX_WAKE_ISR_PRIORITY);
            (void) CyIntSetVector(uart_transmit_RX_WAKE_ISR_NUMBER, &uart_transmit_UART_WAKEUP_ISR);
        #endif /* (uart_transmit_UART_RX_WAKEUP_IRQ) */

            /* Configure interrupt sources */
            uart_transmit_INTR_I2C_EC_MASK_REG = uart_transmit_NO_INTR_SOURCES;
            uart_transmit_INTR_SPI_EC_MASK_REG = uart_transmit_NO_INTR_SOURCES;
            uart_transmit_INTR_SLAVE_MASK_REG  = uart_transmit_NO_INTR_SOURCES;
            uart_transmit_INTR_MASTER_MASK_REG = uart_transmit_NO_INTR_SOURCES;
            uart_transmit_INTR_RX_MASK_REG     = config->rxInterruptMask;
            uart_transmit_INTR_TX_MASK_REG     = config->txInterruptMask;

            /* Configure TX interrupt sources to restore. */
            uart_transmit_IntrTxMask = LO16(uart_transmit_INTR_TX_MASK_REG);

            /* Clear RX buffer indexes */
            uart_transmit_rxBufferHead     = 0u;
            uart_transmit_rxBufferTail     = 0u;
            uart_transmit_rxBufferOverflow = 0u;

            /* Clear TX buffer indexes */
            uart_transmit_txBufferHead = 0u;
            uart_transmit_txBufferTail = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: uart_transmit_UartInit
    ****************************************************************************//**
    *
    *  Configures the SCB for the UART operation.
    *
    *******************************************************************************/
    void uart_transmit_UartInit(void)
    {
        /* Configure UART interface */
        uart_transmit_CTRL_REG = uart_transmit_UART_DEFAULT_CTRL;

        /* Configure sub-mode: UART, SmartCard or IrDA */
        uart_transmit_UART_CTRL_REG = uart_transmit_UART_DEFAULT_UART_CTRL;

        /* Configure RX direction */
        uart_transmit_UART_RX_CTRL_REG = uart_transmit_UART_DEFAULT_UART_RX_CTRL;
        uart_transmit_RX_CTRL_REG      = uart_transmit_UART_DEFAULT_RX_CTRL;
        uart_transmit_RX_FIFO_CTRL_REG = uart_transmit_UART_DEFAULT_RX_FIFO_CTRL;
        uart_transmit_RX_MATCH_REG     = uart_transmit_UART_DEFAULT_RX_MATCH_REG;

        /* Configure TX direction */
        uart_transmit_UART_TX_CTRL_REG = uart_transmit_UART_DEFAULT_UART_TX_CTRL;
        uart_transmit_TX_CTRL_REG      = uart_transmit_UART_DEFAULT_TX_CTRL;
        uart_transmit_TX_FIFO_CTRL_REG = uart_transmit_UART_DEFAULT_TX_FIFO_CTRL;

    #if !(uart_transmit_CY_SCBIP_V0 || uart_transmit_CY_SCBIP_V1)
        uart_transmit_UART_FLOW_CTRL_REG = uart_transmit_UART_DEFAULT_FLOW_CTRL;
    #endif /* !(uart_transmit_CY_SCBIP_V0 || uart_transmit_CY_SCBIP_V1) */

        /* Configure interrupt with UART handler but do not enable it */
    #if(uart_transmit_SCB_IRQ_INTERNAL)
        CyIntDisable    (uart_transmit_ISR_NUMBER);
        CyIntSetPriority(uart_transmit_ISR_NUMBER, uart_transmit_ISR_PRIORITY);
        (void) CyIntSetVector(uart_transmit_ISR_NUMBER, &uart_transmit_SPI_UART_ISR);
    #endif /* (uart_transmit_SCB_IRQ_INTERNAL) */

        /* Configure WAKE interrupt */
    #if(uart_transmit_UART_RX_WAKEUP_IRQ)
        CyIntDisable    (uart_transmit_RX_WAKE_ISR_NUMBER);
        CyIntSetPriority(uart_transmit_RX_WAKE_ISR_NUMBER, uart_transmit_RX_WAKE_ISR_PRIORITY);
        (void) CyIntSetVector(uart_transmit_RX_WAKE_ISR_NUMBER, &uart_transmit_UART_WAKEUP_ISR);
    #endif /* (uart_transmit_UART_RX_WAKEUP_IRQ) */

        /* Configure interrupt sources */
        uart_transmit_INTR_I2C_EC_MASK_REG = uart_transmit_UART_DEFAULT_INTR_I2C_EC_MASK;
        uart_transmit_INTR_SPI_EC_MASK_REG = uart_transmit_UART_DEFAULT_INTR_SPI_EC_MASK;
        uart_transmit_INTR_SLAVE_MASK_REG  = uart_transmit_UART_DEFAULT_INTR_SLAVE_MASK;
        uart_transmit_INTR_MASTER_MASK_REG = uart_transmit_UART_DEFAULT_INTR_MASTER_MASK;
        uart_transmit_INTR_RX_MASK_REG     = uart_transmit_UART_DEFAULT_INTR_RX_MASK;
        uart_transmit_INTR_TX_MASK_REG     = uart_transmit_UART_DEFAULT_INTR_TX_MASK;

        /* Configure TX interrupt sources to restore. */
        uart_transmit_IntrTxMask = LO16(uart_transmit_INTR_TX_MASK_REG);

    #if(uart_transmit_INTERNAL_RX_SW_BUFFER_CONST)
        uart_transmit_rxBufferHead     = 0u;
        uart_transmit_rxBufferTail     = 0u;
        uart_transmit_rxBufferOverflow = 0u;
    #endif /* (uart_transmit_INTERNAL_RX_SW_BUFFER_CONST) */

    #if(uart_transmit_INTERNAL_TX_SW_BUFFER_CONST)
        uart_transmit_txBufferHead = 0u;
        uart_transmit_txBufferTail = 0u;
    #endif /* (uart_transmit_INTERNAL_TX_SW_BUFFER_CONST) */
    }
#endif /* (uart_transmit_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: uart_transmit_UartPostEnable
****************************************************************************//**
*
*  Restores HSIOM settings for the UART output pins (TX and/or RTS) to be
*  controlled by the SCB UART.
*
*******************************************************************************/
void uart_transmit_UartPostEnable(void)
{
#if (uart_transmit_SCB_MODE_UNCONFIG_CONST_CFG)
    #if (uart_transmit_TX_SDA_MISO_PIN)
        if (uart_transmit_CHECK_TX_SDA_MISO_PIN_USED)
        {
            /* Set SCB UART to drive the output pin */
            uart_transmit_SET_HSIOM_SEL(uart_transmit_TX_SDA_MISO_HSIOM_REG, uart_transmit_TX_SDA_MISO_HSIOM_MASK,
                                           uart_transmit_TX_SDA_MISO_HSIOM_POS, uart_transmit_TX_SDA_MISO_HSIOM_SEL_UART);
        }
    #endif /* (uart_transmit_TX_SDA_MISO_PIN_PIN) */

    #if !(uart_transmit_CY_SCBIP_V0 || uart_transmit_CY_SCBIP_V1)
        #if (uart_transmit_SS0_PIN)
            if (uart_transmit_CHECK_SS0_PIN_USED)
            {
                /* Set SCB UART to drive the output pin */
                uart_transmit_SET_HSIOM_SEL(uart_transmit_SS0_HSIOM_REG, uart_transmit_SS0_HSIOM_MASK,
                                               uart_transmit_SS0_HSIOM_POS, uart_transmit_SS0_HSIOM_SEL_UART);
            }
        #endif /* (uart_transmit_SS0_PIN) */
    #endif /* !(uart_transmit_CY_SCBIP_V0 || uart_transmit_CY_SCBIP_V1) */

#else
    #if (uart_transmit_UART_TX_PIN)
         /* Set SCB UART to drive the output pin */
        uart_transmit_SET_HSIOM_SEL(uart_transmit_TX_HSIOM_REG, uart_transmit_TX_HSIOM_MASK,
                                       uart_transmit_TX_HSIOM_POS, uart_transmit_TX_HSIOM_SEL_UART);
    #endif /* (uart_transmit_UART_TX_PIN) */

    #if (uart_transmit_UART_RTS_PIN)
        /* Set SCB UART to drive the output pin */
        uart_transmit_SET_HSIOM_SEL(uart_transmit_RTS_HSIOM_REG, uart_transmit_RTS_HSIOM_MASK,
                                       uart_transmit_RTS_HSIOM_POS, uart_transmit_RTS_HSIOM_SEL_UART);
    #endif /* (uart_transmit_UART_RTS_PIN) */
#endif /* (uart_transmit_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Restore TX interrupt sources. */
    uart_transmit_SetTxInterruptMode(uart_transmit_IntrTxMask);
}


/*******************************************************************************
* Function Name: uart_transmit_UartStop
****************************************************************************//**
*
*  Changes the HSIOM settings for the UART output pins (TX and/or RTS) to keep
*  them inactive after the block is disabled. The output pins are controlled by
*  the GPIO data register. Also, the function disables the skip start feature
*  to not cause it to trigger after the component is enabled.
*
*******************************************************************************/
void uart_transmit_UartStop(void)
{
#if(uart_transmit_SCB_MODE_UNCONFIG_CONST_CFG)
    #if (uart_transmit_TX_SDA_MISO_PIN)
        if (uart_transmit_CHECK_TX_SDA_MISO_PIN_USED)
        {
            /* Set GPIO to drive output pin */
            uart_transmit_SET_HSIOM_SEL(uart_transmit_TX_SDA_MISO_HSIOM_REG, uart_transmit_TX_SDA_MISO_HSIOM_MASK,
                                           uart_transmit_TX_SDA_MISO_HSIOM_POS, uart_transmit_TX_SDA_MISO_HSIOM_SEL_GPIO);
        }
    #endif /* (uart_transmit_TX_SDA_MISO_PIN_PIN) */

    #if !(uart_transmit_CY_SCBIP_V0 || uart_transmit_CY_SCBIP_V1)
        #if (uart_transmit_SS0_PIN)
            if (uart_transmit_CHECK_SS0_PIN_USED)
            {
                /* Set output pin state after block is disabled */
                uart_transmit_spi_ss0_Write(uart_transmit_GET_UART_RTS_INACTIVE);

                /* Set GPIO to drive output pin */
                uart_transmit_SET_HSIOM_SEL(uart_transmit_SS0_HSIOM_REG, uart_transmit_SS0_HSIOM_MASK,
                                               uart_transmit_SS0_HSIOM_POS, uart_transmit_SS0_HSIOM_SEL_GPIO);
            }
        #endif /* (uart_transmit_SS0_PIN) */
    #endif /* !(uart_transmit_CY_SCBIP_V0 || uart_transmit_CY_SCBIP_V1) */

#else
    #if (uart_transmit_UART_TX_PIN)
        /* Set GPIO to drive output pin */
        uart_transmit_SET_HSIOM_SEL(uart_transmit_TX_HSIOM_REG, uart_transmit_TX_HSIOM_MASK,
                                       uart_transmit_TX_HSIOM_POS, uart_transmit_TX_HSIOM_SEL_GPIO);
    #endif /* (uart_transmit_UART_TX_PIN) */

    #if (uart_transmit_UART_RTS_PIN)
        /* Set output pin state after block is disabled */
        uart_transmit_rts_Write(uart_transmit_GET_UART_RTS_INACTIVE);

        /* Set GPIO to drive output pin */
        uart_transmit_SET_HSIOM_SEL(uart_transmit_RTS_HSIOM_REG, uart_transmit_RTS_HSIOM_MASK,
                                       uart_transmit_RTS_HSIOM_POS, uart_transmit_RTS_HSIOM_SEL_GPIO);
    #endif /* (uart_transmit_UART_RTS_PIN) */

#endif /* (uart_transmit_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (uart_transmit_UART_WAKE_ENABLE_CONST)
    /* Disable skip start feature used for wakeup */
    uart_transmit_UART_RX_CTRL_REG &= (uint32) ~uart_transmit_UART_RX_CTRL_SKIP_START;
#endif /* (uart_transmit_UART_WAKE_ENABLE_CONST) */

    /* Store TX interrupt sources (exclude level triggered). */
    uart_transmit_IntrTxMask = LO16(uart_transmit_GetTxInterruptMode() & uart_transmit_INTR_UART_TX_RESTORE);
}


/*******************************************************************************
* Function Name: uart_transmit_UartSetRxAddress
****************************************************************************//**
*
*  Sets the hardware detectable receiver address for the UART in the
*  Multiprocessor mode.
*
*  \param address: Address for hardware address detection.
*
*******************************************************************************/
void uart_transmit_UartSetRxAddress(uint32 address)
{
     uint32 matchReg;

    matchReg = uart_transmit_RX_MATCH_REG;

    matchReg &= ((uint32) ~uart_transmit_RX_MATCH_ADDR_MASK); /* Clear address bits */
    matchReg |= ((uint32)  (address & uart_transmit_RX_MATCH_ADDR_MASK)); /* Set address  */

    uart_transmit_RX_MATCH_REG = matchReg;
}


/*******************************************************************************
* Function Name: uart_transmit_UartSetRxAddressMask
****************************************************************************//**
*
*  Sets the hardware address mask for the UART in the Multiprocessor mode.
*
*  \param addressMask: Address mask.
*   - Bit value 0 – excludes bit from address comparison.
*   - Bit value 1 – the bit needs to match with the corresponding bit
*     of the address.
*
*******************************************************************************/
void uart_transmit_UartSetRxAddressMask(uint32 addressMask)
{
    uint32 matchReg;

    matchReg = uart_transmit_RX_MATCH_REG;

    matchReg &= ((uint32) ~uart_transmit_RX_MATCH_MASK_MASK); /* Clear address mask bits */
    matchReg |= ((uint32) (addressMask << uart_transmit_RX_MATCH_MASK_POS));

    uart_transmit_RX_MATCH_REG = matchReg;
}


#if(uart_transmit_UART_RX_DIRECTION)
    /*******************************************************************************
    * Function Name: uart_transmit_UartGetChar
    ****************************************************************************//**
    *
    *  Retrieves next data element from receive buffer.
    *  This function is designed for ASCII characters and returns a char where
    *  1 to 255 are valid characters and 0 indicates an error occurred or no data
    *  is present.
    *  - RX software buffer is disabled: Returns data element retrieved from RX
    *    FIFO.
    *  - RX software buffer is enabled: Returns data element from the software
    *    receive buffer.
    *
    *  \return
    *   Next data element from the receive buffer. ASCII character values from
    *   1 to 255 are valid. A returned zero signifies an error condition or no
    *   data available.
    *
    *  \sideeffect
    *   The errors bits may not correspond with reading characters due to
    *   RX FIFO and software buffer usage.
    *   RX software buffer is enabled: The internal software buffer overflow
    *   is not treated as an error condition.
    *   Check uart_transmit_rxBufferOverflow to capture that error condition.
    *
    *******************************************************************************/
    uint32 uart_transmit_UartGetChar(void)
    {
        uint32 rxData = 0u;

        /* Reads data only if there is data to read */
        if (0u != uart_transmit_SpiUartGetRxBufferSize())
        {
            rxData = uart_transmit_SpiUartReadRxData();
        }

        if (uart_transmit_CHECK_INTR_RX(uart_transmit_INTR_RX_ERR))
        {
            rxData = 0u; /* Error occurred: returns zero */
            uart_transmit_ClearRxInterruptSource(uart_transmit_INTR_RX_ERR);
        }

        return (rxData);
    }


    /*******************************************************************************
    * Function Name: uart_transmit_UartGetByte
    ****************************************************************************//**
    *
    *  Retrieves the next data element from the receive buffer, returns the
    *  received byte and error condition.
    *   - The RX software buffer is disabled: returns the data element retrieved
    *     from the RX FIFO. Undefined data will be returned if the RX FIFO is
    *     empty.
    *   - The RX software buffer is enabled: returns data element from the
    *     software receive buffer.
    *
    *  \return
    *   Bits 7-0 contain the next data element from the receive buffer and
    *   other bits contain the error condition.
    *   - uart_transmit_UART_RX_OVERFLOW - Attempt to write to a full
    *     receiver FIFO.
    *   - uart_transmit_UART_RX_UNDERFLOW    Attempt to read from an empty
    *     receiver FIFO.
    *   - uart_transmit_UART_RX_FRAME_ERROR - UART framing error detected.
    *   - uart_transmit_UART_RX_PARITY_ERROR - UART parity error detected.
    *
    *  \sideeffect
    *   The errors bits may not correspond with reading characters due to
    *   RX FIFO and software buffer usage.
    *   RX software buffer is enabled: The internal software buffer overflow
    *   is not treated as an error condition.
    *   Check uart_transmit_rxBufferOverflow to capture that error condition.
    *
    *******************************************************************************/
    uint32 uart_transmit_UartGetByte(void)
    {
        uint32 rxData;
        uint32 tmpStatus;

        #if (uart_transmit_CHECK_RX_SW_BUFFER)
        {
            uart_transmit_DisableInt();
        }
        #endif

        if (0u != uart_transmit_SpiUartGetRxBufferSize())
        {
            /* Enables interrupt to receive more bytes: at least one byte is in
            * buffer.
            */
            #if (uart_transmit_CHECK_RX_SW_BUFFER)
            {
                uart_transmit_EnableInt();
            }
            #endif

            /* Get received byte */
            rxData = uart_transmit_SpiUartReadRxData();
        }
        else
        {
            /* Reads a byte directly from RX FIFO: underflow is raised in the
            * case of empty. Otherwise the first received byte will be read.
            */
            rxData = uart_transmit_RX_FIFO_RD_REG;


            /* Enables interrupt to receive more bytes. */
            #if (uart_transmit_CHECK_RX_SW_BUFFER)
            {

                /* The byte has been read from RX FIFO. Clear RX interrupt to
                * not involve interrupt handler when RX FIFO is empty.
                */
                uart_transmit_ClearRxInterruptSource(uart_transmit_INTR_RX_NOT_EMPTY);

                uart_transmit_EnableInt();
            }
            #endif
        }

        /* Get and clear RX error mask */
        tmpStatus = (uart_transmit_GetRxInterruptSource() & uart_transmit_INTR_RX_ERR);
        uart_transmit_ClearRxInterruptSource(uart_transmit_INTR_RX_ERR);

        /* Puts together data and error status:
        * MP mode and accept address: 9th bit is set to notify mark.
        */
        rxData |= ((uint32) (tmpStatus << 8u));

        return (rxData);
    }


    #if !(uart_transmit_CY_SCBIP_V0 || uart_transmit_CY_SCBIP_V1)
        /*******************************************************************************
        * Function Name: uart_transmit_UartSetRtsPolarity
        ****************************************************************************//**
        *
        *  Sets active polarity of RTS output signal.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *  \param polarity: Active polarity of RTS output signal.
        *   - uart_transmit_UART_RTS_ACTIVE_LOW  - RTS signal is active low.
        *   - uart_transmit_UART_RTS_ACTIVE_HIGH - RTS signal is active high.
        *
        *******************************************************************************/
        void uart_transmit_UartSetRtsPolarity(uint32 polarity)
        {
            if(0u != polarity)
            {
                uart_transmit_UART_FLOW_CTRL_REG |= (uint32)  uart_transmit_UART_FLOW_CTRL_RTS_POLARITY;
            }
            else
            {
                uart_transmit_UART_FLOW_CTRL_REG &= (uint32) ~uart_transmit_UART_FLOW_CTRL_RTS_POLARITY;
            }
        }


        /*******************************************************************************
        * Function Name: uart_transmit_UartSetRtsFifoLevel
        ****************************************************************************//**
        *
        *  Sets level in the RX FIFO for RTS signal activation.
        *  While the RX FIFO has fewer entries than the RX FIFO level the RTS signal
        *  remains active, otherwise the RTS signal becomes inactive.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *  \param level: Level in the RX FIFO for RTS signal activation.
        *   The range of valid level values is between 0 and RX FIFO depth - 1.
        *   Setting level value to 0 disables RTS signal activation.
        *
        *******************************************************************************/
        void uart_transmit_UartSetRtsFifoLevel(uint32 level)
        {
            uint32 uartFlowCtrl;

            uartFlowCtrl = uart_transmit_UART_FLOW_CTRL_REG;

            uartFlowCtrl &= ((uint32) ~uart_transmit_UART_FLOW_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
            uartFlowCtrl |= ((uint32) (uart_transmit_UART_FLOW_CTRL_TRIGGER_LEVEL_MASK & level));

            uart_transmit_UART_FLOW_CTRL_REG = uartFlowCtrl;
        }
    #endif /* !(uart_transmit_CY_SCBIP_V0 || uart_transmit_CY_SCBIP_V1) */

#endif /* (uart_transmit_UART_RX_DIRECTION) */


#if(uart_transmit_UART_TX_DIRECTION)
    /*******************************************************************************
    * Function Name: uart_transmit_UartPutString
    ****************************************************************************//**
    *
    *  Places a NULL terminated string in the transmit buffer to be sent at the
    *  next available bus time.
    *  This function is blocking and waits until there is a space available to put
    *  requested data in transmit buffer.
    *
    *  \param string: pointer to the null terminated string array to be placed in the
    *   transmit buffer.
    *
    *******************************************************************************/
    void uart_transmit_UartPutString(const char8 string[])
    {
        uint32 bufIndex;

        bufIndex = 0u;

        /* Blocks the control flow until all data has been sent */
        while(string[bufIndex] != ((char8) 0))
        {
            uart_transmit_UartPutChar((uint32) string[bufIndex]);
            bufIndex++;
        }
    }


    /*******************************************************************************
    * Function Name: uart_transmit_UartPutCRLF
    ****************************************************************************//**
    *
    *  Places byte of data followed by a carriage return (0x0D) and line feed
    *  (0x0A) in the transmit buffer.
    *  This function is blocking and waits until there is a space available to put
    *  all requested data in transmit buffer.
    *
    *  \param txDataByte: the data to be transmitted.
    *
    *******************************************************************************/
    void uart_transmit_UartPutCRLF(uint32 txDataByte)
    {
        uart_transmit_UartPutChar(txDataByte);  /* Blocks control flow until all data has been sent */
        uart_transmit_UartPutChar(0x0Du);       /* Blocks control flow until all data has been sent */
        uart_transmit_UartPutChar(0x0Au);       /* Blocks control flow until all data has been sent */
    }


    #if !(uart_transmit_CY_SCBIP_V0 || uart_transmit_CY_SCBIP_V1)
        /*******************************************************************************
        * Function Name: uart_transmitSCB_UartEnableCts
        ****************************************************************************//**
        *
        *  Enables usage of CTS input signal by the UART transmitter.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *******************************************************************************/
        void uart_transmit_UartEnableCts(void)
        {
            uart_transmit_UART_FLOW_CTRL_REG |= (uint32)  uart_transmit_UART_FLOW_CTRL_CTS_ENABLE;
        }


        /*******************************************************************************
        * Function Name: uart_transmit_UartDisableCts
        ****************************************************************************//**
        *
        *  Disables usage of CTS input signal by the UART transmitter.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *******************************************************************************/
        void uart_transmit_UartDisableCts(void)
        {
            uart_transmit_UART_FLOW_CTRL_REG &= (uint32) ~uart_transmit_UART_FLOW_CTRL_CTS_ENABLE;
        }


        /*******************************************************************************
        * Function Name: uart_transmit_UartSetCtsPolarity
        ****************************************************************************//**
        *
        *  Sets active polarity of CTS input signal.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        * \param
        * polarity: Active polarity of CTS output signal.
        *   - uart_transmit_UART_CTS_ACTIVE_LOW  - CTS signal is active low.
        *   - uart_transmit_UART_CTS_ACTIVE_HIGH - CTS signal is active high.
        *
        *******************************************************************************/
        void uart_transmit_UartSetCtsPolarity(uint32 polarity)
        {
            if (0u != polarity)
            {
                uart_transmit_UART_FLOW_CTRL_REG |= (uint32)  uart_transmit_UART_FLOW_CTRL_CTS_POLARITY;
            }
            else
            {
                uart_transmit_UART_FLOW_CTRL_REG &= (uint32) ~uart_transmit_UART_FLOW_CTRL_CTS_POLARITY;
            }
        }
    #endif /* !(uart_transmit_CY_SCBIP_V0 || uart_transmit_CY_SCBIP_V1) */


    /*******************************************************************************
    * Function Name: uart_transmit_UartSendBreakBlocking
    ****************************************************************************//**
    *
    * Sends a break condition (logic low) of specified width on UART TX line.
    * Blocks until break is completed. Only call this function when UART TX FIFO
    * and shifter are empty.
    *
    * \param breakWidth
    * Width of break condition. Valid range is 4 to 16 bits.
    *
    * \note
    * Before sending break all UART TX interrupt sources are disabled. The state
    * of UART TX interrupt sources is restored before function returns.
    *
    * \sideeffect
    * If this function is called while there is data in the TX FIFO or shifter that
    * data will be shifted out in packets the size of breakWidth.
    *
    *******************************************************************************/
    void uart_transmit_UartSendBreakBlocking(uint32 breakWidth)
    {
        uint32 txCtrlReg;
        uint32 txIntrReg;

        /* Disable all UART TX interrupt source and clear UART TX Done history */
        txIntrReg = uart_transmit_GetTxInterruptMode();
        uart_transmit_SetTxInterruptMode(0u);
        uart_transmit_ClearTxInterruptSource(uart_transmit_INTR_TX_UART_DONE);

        /* Store TX CTRL configuration */
        txCtrlReg = uart_transmit_TX_CTRL_REG;

        /* Set break width */
        uart_transmit_TX_CTRL_REG = (uart_transmit_TX_CTRL_REG & (uint32) ~uart_transmit_TX_CTRL_DATA_WIDTH_MASK) |
                                        uart_transmit_GET_TX_CTRL_DATA_WIDTH(breakWidth);

        /* Generate break */
        uart_transmit_TX_FIFO_WR_REG = 0u;

        /* Wait for break completion */
        while (0u == (uart_transmit_GetTxInterruptSource() & uart_transmit_INTR_TX_UART_DONE))
        {
        }

        /* Clear all UART TX interrupt sources to  */
        uart_transmit_ClearTxInterruptSource(uart_transmit_INTR_TX_ALL);

        /* Restore TX interrupt sources and data width */
        uart_transmit_TX_CTRL_REG = txCtrlReg;
        uart_transmit_SetTxInterruptMode(txIntrReg);
    }
#endif /* (uart_transmit_UART_TX_DIRECTION) */


#if (uart_transmit_UART_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: uart_transmit_UartSaveConfig
    ****************************************************************************//**
    *
    *  Clears and enables an interrupt on a falling edge of the Rx input. The GPIO
    *  interrupt does not track in the active mode, therefore requires to be
    *  cleared by this API.
    *
    *******************************************************************************/
    void uart_transmit_UartSaveConfig(void)
    {
    #if (uart_transmit_UART_RX_WAKEUP_IRQ)
        /* Set SKIP_START if requested (set by default). */
        if (0u != uart_transmit_skipStart)
        {
            uart_transmit_UART_RX_CTRL_REG |= (uint32)  uart_transmit_UART_RX_CTRL_SKIP_START;
        }
        else
        {
            uart_transmit_UART_RX_CTRL_REG &= (uint32) ~uart_transmit_UART_RX_CTRL_SKIP_START;
        }

        /* Clear RX GPIO interrupt status and pending interrupt in NVIC because
        * falling edge on RX line occurs while UART communication in active mode.
        * Enable interrupt: next interrupt trigger should wakeup device.
        */
        uart_transmit_CLEAR_UART_RX_WAKE_INTR;
        uart_transmit_RxWakeClearPendingInt();
        uart_transmit_RxWakeEnableInt();
    #endif /* (uart_transmit_UART_RX_WAKEUP_IRQ) */
    }


    /*******************************************************************************
    * Function Name: uart_transmit_UartRestoreConfig
    ****************************************************************************//**
    *
    *  Disables the RX GPIO interrupt. Until this function is called the interrupt
    *  remains active and triggers on every falling edge of the UART RX line.
    *
    *******************************************************************************/
    void uart_transmit_UartRestoreConfig(void)
    {
    #if (uart_transmit_UART_RX_WAKEUP_IRQ)
        /* Disable interrupt: no more triggers in active mode */
        uart_transmit_RxWakeDisableInt();
    #endif /* (uart_transmit_UART_RX_WAKEUP_IRQ) */
    }


    #if (uart_transmit_UART_RX_WAKEUP_IRQ)
        /*******************************************************************************
        * Function Name: uart_transmit_UART_WAKEUP_ISR
        ****************************************************************************//**
        *
        *  Handles the Interrupt Service Routine for the SCB UART mode GPIO wakeup
        *  event. This event is configured to trigger on a falling edge of the RX line.
        *
        *******************************************************************************/
        CY_ISR(uart_transmit_UART_WAKEUP_ISR)
        {
        #ifdef uart_transmit_UART_WAKEUP_ISR_ENTRY_CALLBACK
            uart_transmit_UART_WAKEUP_ISR_EntryCallback();
        #endif /* uart_transmit_UART_WAKEUP_ISR_ENTRY_CALLBACK */

            uart_transmit_CLEAR_UART_RX_WAKE_INTR;

        #ifdef uart_transmit_UART_WAKEUP_ISR_EXIT_CALLBACK
            uart_transmit_UART_WAKEUP_ISR_ExitCallback();
        #endif /* uart_transmit_UART_WAKEUP_ISR_EXIT_CALLBACK */
        }
    #endif /* (uart_transmit_UART_RX_WAKEUP_IRQ) */
#endif /* (uart_transmit_UART_RX_WAKEUP_IRQ) */


/* [] END OF FILE */
