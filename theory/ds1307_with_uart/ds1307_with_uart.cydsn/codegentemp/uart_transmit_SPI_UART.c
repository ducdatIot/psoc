/***************************************************************************//**
* \file uart_transmit_SPI_UART.c
* \version 4.0
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  SPI and UART modes.
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

/***************************************
*        SPI/UART Private Vars
***************************************/

#if(uart_transmit_INTERNAL_RX_SW_BUFFER_CONST)
    /* Start index to put data into the software receive buffer.*/
    volatile uint32 uart_transmit_rxBufferHead;
    /* Start index to get data from the software receive buffer.*/
    volatile uint32 uart_transmit_rxBufferTail;
    /**
    * \addtogroup group_globals
    * \{
    */
    /** Sets when internal software receive buffer overflow
    *  was occurred.
    */
    volatile uint8  uart_transmit_rxBufferOverflow;
    /** \} globals */
#endif /* (uart_transmit_INTERNAL_RX_SW_BUFFER_CONST) */

#if(uart_transmit_INTERNAL_TX_SW_BUFFER_CONST)
    /* Start index to put data into the software transmit buffer.*/
    volatile uint32 uart_transmit_txBufferHead;
    /* Start index to get data from the software transmit buffer.*/
    volatile uint32 uart_transmit_txBufferTail;
#endif /* (uart_transmit_INTERNAL_TX_SW_BUFFER_CONST) */

#if(uart_transmit_INTERNAL_RX_SW_BUFFER)
    /* Add one element to the buffer to receive full packet. One byte in receive buffer is always empty */
    volatile uint8 uart_transmit_rxBufferInternal[uart_transmit_INTERNAL_RX_BUFFER_SIZE];
#endif /* (uart_transmit_INTERNAL_RX_SW_BUFFER) */

#if(uart_transmit_INTERNAL_TX_SW_BUFFER)
    volatile uint8 uart_transmit_txBufferInternal[uart_transmit_TX_BUFFER_SIZE];
#endif /* (uart_transmit_INTERNAL_TX_SW_BUFFER) */


#if(uart_transmit_RX_DIRECTION)
    /*******************************************************************************
    * Function Name: uart_transmit_SpiUartReadRxData
    ****************************************************************************//**
    *
    *  Retrieves the next data element from the receive buffer.
    *   - RX software buffer is disabled: Returns data element retrieved from
    *     RX FIFO. Undefined data will be returned if the RX FIFO is empty.
    *   - RX software buffer is enabled: Returns data element from the software
    *     receive buffer. Zero value is returned if the software receive buffer
    *     is empty.
    *
    * \return
    *  Next data element from the receive buffer. 
    *  The amount of data bits to be received depends on RX data bits selection 
    *  (the data bit counting starts from LSB of return value).
    *
    * \globalvars
    *  uart_transmit_rxBufferHead - the start index to put data into the 
    *  software receive buffer.
    *  uart_transmit_rxBufferTail - the start index to get data from the 
    *  software receive buffer.
    *
    *******************************************************************************/
    uint32 uart_transmit_SpiUartReadRxData(void)
    {
        uint32 rxData = 0u;

    #if (uart_transmit_INTERNAL_RX_SW_BUFFER_CONST)
        uint32 locTail;
    #endif /* (uart_transmit_INTERNAL_RX_SW_BUFFER_CONST) */

        #if (uart_transmit_CHECK_RX_SW_BUFFER)
        {
            if (uart_transmit_rxBufferHead != uart_transmit_rxBufferTail)
            {
                /* There is data in RX software buffer */

                /* Calculate index to read from */
                locTail = (uart_transmit_rxBufferTail + 1u);

                if (uart_transmit_INTERNAL_RX_BUFFER_SIZE == locTail)
                {
                    locTail = 0u;
                }

                /* Get data from RX software buffer */
                rxData = uart_transmit_GetWordFromRxBuffer(locTail);

                /* Change index in the buffer */
                uart_transmit_rxBufferTail = locTail;

                #if (uart_transmit_CHECK_UART_RTS_CONTROL_FLOW)
                {
                    /* Check if RX Not Empty is disabled in the interrupt */
                    if (0u == (uart_transmit_INTR_RX_MASK_REG & uart_transmit_INTR_RX_NOT_EMPTY))
                    {
                        /* Enable RX Not Empty interrupt source to continue
                        * receiving data into software buffer.
                        */
                        uart_transmit_INTR_RX_MASK_REG |= uart_transmit_INTR_RX_NOT_EMPTY;
                    }
                }
                #endif

            }
        }
        #else
        {
            /* Read data from RX FIFO */
            rxData = uart_transmit_RX_FIFO_RD_REG;
        }
        #endif

        return (rxData);
    }


    /*******************************************************************************
    * Function Name: uart_transmit_SpiUartGetRxBufferSize
    ****************************************************************************//**
    *
    *  Returns the number of received data elements in the receive buffer.
    *   - RX software buffer disabled: returns the number of used entries in
    *     RX FIFO.
    *   - RX software buffer enabled: returns the number of elements which were
    *     placed in the receive buffer. This does not include the hardware RX FIFO.
    *
    * \return
    *  Number of received data elements.
    *
    * \globalvars
    *  uart_transmit_rxBufferHead - the start index to put data into the 
    *  software receive buffer.
    *  uart_transmit_rxBufferTail - the start index to get data from the 
    *  software receive buffer.
    *
    *******************************************************************************/
    uint32 uart_transmit_SpiUartGetRxBufferSize(void)
    {
        uint32 size;
    #if (uart_transmit_INTERNAL_RX_SW_BUFFER_CONST)
        uint32 locHead;
    #endif /* (uart_transmit_INTERNAL_RX_SW_BUFFER_CONST) */

        #if (uart_transmit_CHECK_RX_SW_BUFFER)
        {
            locHead = uart_transmit_rxBufferHead;

            if(locHead >= uart_transmit_rxBufferTail)
            {
                size = (locHead - uart_transmit_rxBufferTail);
            }
            else
            {
                size = (locHead + (uart_transmit_INTERNAL_RX_BUFFER_SIZE - uart_transmit_rxBufferTail));
            }
        }
        #else
        {
            size = uart_transmit_GET_RX_FIFO_ENTRIES;
        }
        #endif

        return (size);
    }


    /*******************************************************************************
    * Function Name: uart_transmit_SpiUartClearRxBuffer
    ****************************************************************************//**
    *
    *  Clears the receive buffer and RX FIFO.
    *
    * \globalvars
    *  uart_transmit_rxBufferHead - the start index to put data into the 
    *  software receive buffer.
    *  uart_transmit_rxBufferTail - the start index to get data from the 
    *  software receive buffer.
    *
    *******************************************************************************/
    void uart_transmit_SpiUartClearRxBuffer(void)
    {
        #if (uart_transmit_CHECK_RX_SW_BUFFER)
        {
            /* Lock from component interruption */
            uart_transmit_DisableInt();

            /* Flush RX software buffer */
            uart_transmit_rxBufferHead = uart_transmit_rxBufferTail;
            uart_transmit_rxBufferOverflow = 0u;

            uart_transmit_CLEAR_RX_FIFO;
            uart_transmit_ClearRxInterruptSource(uart_transmit_INTR_RX_ALL);

            #if (uart_transmit_CHECK_UART_RTS_CONTROL_FLOW)
            {
                /* Enable RX Not Empty interrupt source to continue receiving
                * data into software buffer.
                */
                uart_transmit_INTR_RX_MASK_REG |= uart_transmit_INTR_RX_NOT_EMPTY;
            }
            #endif
            
            /* Release lock */
            uart_transmit_EnableInt();
        }
        #else
        {
            uart_transmit_CLEAR_RX_FIFO;
        }
        #endif
    }

#endif /* (uart_transmit_RX_DIRECTION) */


#if(uart_transmit_TX_DIRECTION)
    /*******************************************************************************
    * Function Name: uart_transmit_SpiUartWriteTxData
    ****************************************************************************//**
    *
    *  Places a data entry into the transmit buffer to be sent at the next available
    *  bus time.
    *  This function is blocking and waits until there is space available to put the
    *  requested data in the transmit buffer.
    *
    *  \param txDataByte: the data to be transmitted.
    *   The amount of data bits to be transmitted depends on TX data bits selection 
    *   (the data bit counting starts from LSB of txDataByte).
    *
    * \globalvars
    *  uart_transmit_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  uart_transmit_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    void uart_transmit_SpiUartWriteTxData(uint32 txData)
    {
    #if (uart_transmit_INTERNAL_TX_SW_BUFFER_CONST)
        uint32 locHead;
    #endif /* (uart_transmit_INTERNAL_TX_SW_BUFFER_CONST) */

        #if (uart_transmit_CHECK_TX_SW_BUFFER)
        {
            /* Put data directly into the TX FIFO */
            if ((uart_transmit_txBufferHead == uart_transmit_txBufferTail) &&
                (uart_transmit_SPI_UART_FIFO_SIZE != uart_transmit_GET_TX_FIFO_ENTRIES))
            {
                /* TX software buffer is empty: put data directly in TX FIFO */
                uart_transmit_TX_FIFO_WR_REG = txData;
            }
            /* Put data into TX software buffer */
            else
            {
                /* Head index to put data */
                locHead = (uart_transmit_txBufferHead + 1u);

                /* Adjust TX software buffer index */
                if (uart_transmit_TX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                /* Wait for space in TX software buffer */
                while (locHead == uart_transmit_txBufferTail)
                {
                }

                /* TX software buffer has at least one room */

                /* Clear old status of INTR_TX_NOT_FULL. It sets at the end of transfer when TX FIFO is empty. */
                uart_transmit_ClearTxInterruptSource(uart_transmit_INTR_TX_NOT_FULL);

                uart_transmit_PutWordInTxBuffer(locHead, txData);

                uart_transmit_txBufferHead = locHead;

                /* Check if TX Not Full is disabled in interrupt */
                if (0u == (uart_transmit_INTR_TX_MASK_REG & uart_transmit_INTR_TX_NOT_FULL))
                {
                    /* Enable TX Not Full interrupt source to transmit from software buffer */
                    uart_transmit_INTR_TX_MASK_REG |= (uint32) uart_transmit_INTR_TX_NOT_FULL;
                }
            }
        }
        #else
        {
            /* Wait until TX FIFO has space to put data element */
            while (uart_transmit_SPI_UART_FIFO_SIZE == uart_transmit_GET_TX_FIFO_ENTRIES)
            {
            }

            uart_transmit_TX_FIFO_WR_REG = txData;
        }
        #endif
    }


    /*******************************************************************************
    * Function Name: uart_transmit_SpiUartPutArray
    ****************************************************************************//**
    *
    *  Places an array of data into the transmit buffer to be sent.
    *  This function is blocking and waits until there is a space available to put
    *  all the requested data in the transmit buffer. The array size can be greater
    *  than transmit buffer size.
    *
    * \param wrBuf: pointer to an array of data to be placed in transmit buffer. 
    *  The width of the data to be transmitted depends on TX data width selection 
    *  (the data bit counting starts from LSB for each array element).
    * \param count: number of data elements to be placed in the transmit buffer.
    *
    * \globalvars
    *  uart_transmit_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  uart_transmit_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    void uart_transmit_SpiUartPutArray(const uint8 wrBuf[], uint32 count)
    {
        uint32 i;

        for (i=0u; i < count; i++)
        {
            uart_transmit_SpiUartWriteTxData((uint32) wrBuf[i]);
        }
    }


    /*******************************************************************************
    * Function Name: uart_transmit_SpiUartGetTxBufferSize
    ****************************************************************************//**
    *
    *  Returns the number of elements currently in the transmit buffer.
    *   - TX software buffer is disabled: returns the number of used entries in
    *     TX FIFO.
    *   - TX software buffer is enabled: returns the number of elements currently
    *     used in the transmit buffer. This number does not include used entries in
    *     the TX FIFO. The transmit buffer size is zero until the TX FIFO is
    *     not full.
    *
    * \return
    *  Number of data elements ready to transmit.
    *
    * \globalvars
    *  uart_transmit_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  uart_transmit_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    uint32 uart_transmit_SpiUartGetTxBufferSize(void)
    {
        uint32 size;
    #if (uart_transmit_INTERNAL_TX_SW_BUFFER_CONST)
        uint32 locTail;
    #endif /* (uart_transmit_INTERNAL_TX_SW_BUFFER_CONST) */

        #if (uart_transmit_CHECK_TX_SW_BUFFER)
        {
            /* Get current Tail index */
            locTail = uart_transmit_txBufferTail;

            if (uart_transmit_txBufferHead >= locTail)
            {
                size = (uart_transmit_txBufferHead - locTail);
            }
            else
            {
                size = (uart_transmit_txBufferHead + (uart_transmit_TX_BUFFER_SIZE - locTail));
            }
        }
        #else
        {
            size = uart_transmit_GET_TX_FIFO_ENTRIES;
        }
        #endif

        return (size);
    }


    /*******************************************************************************
    * Function Name: uart_transmit_SpiUartClearTxBuffer
    ****************************************************************************//**
    *
    *  Clears the transmit buffer and TX FIFO.
    *
    * \globalvars
    *  uart_transmit_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  uart_transmit_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    void uart_transmit_SpiUartClearTxBuffer(void)
    {
        #if (uart_transmit_CHECK_TX_SW_BUFFER)
        {
            /* Lock from component interruption */
            uart_transmit_DisableInt();

            /* Flush TX software buffer */
            uart_transmit_txBufferHead = uart_transmit_txBufferTail;

            uart_transmit_INTR_TX_MASK_REG &= (uint32) ~uart_transmit_INTR_TX_NOT_FULL;
            uart_transmit_CLEAR_TX_FIFO;
            uart_transmit_ClearTxInterruptSource(uart_transmit_INTR_TX_ALL);

            /* Release lock */
            uart_transmit_EnableInt();
        }
        #else
        {
            uart_transmit_CLEAR_TX_FIFO;
        }
        #endif
    }

#endif /* (uart_transmit_TX_DIRECTION) */


/*******************************************************************************
* Function Name: uart_transmit_SpiUartDisableIntRx
****************************************************************************//**
*
*  Disables the RX interrupt sources.
*
*  \return
*   Returns the RX interrupt sources enabled before the function call.
*
*******************************************************************************/
uint32 uart_transmit_SpiUartDisableIntRx(void)
{
    uint32 intSource;

    intSource = uart_transmit_GetRxInterruptMode();

    uart_transmit_SetRxInterruptMode(uart_transmit_NO_INTR_SOURCES);

    return (intSource);
}


/*******************************************************************************
* Function Name: uart_transmit_SpiUartDisableIntTx
****************************************************************************//**
*
*  Disables TX interrupt sources.
*
*  \return
*   Returns TX interrupt sources enabled before function call.
*
*******************************************************************************/
uint32 uart_transmit_SpiUartDisableIntTx(void)
{
    uint32 intSourceMask;

    intSourceMask = uart_transmit_GetTxInterruptMode();

    uart_transmit_SetTxInterruptMode(uart_transmit_NO_INTR_SOURCES);

    return (intSourceMask);
}


#if(uart_transmit_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: uart_transmit_PutWordInRxBuffer
    ****************************************************************************//**
    *
    *  Stores a byte/word into the RX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \param index:      index to store data byte/word in the RX buffer.
    *  \param rxDataByte: byte/word to store.
    *
    *******************************************************************************/
    void uart_transmit_PutWordInRxBuffer(uint32 idx, uint32 rxDataByte)
    {
        /* Put data in buffer */
        if (uart_transmit_ONE_BYTE_WIDTH == uart_transmit_rxDataBits)
        {
            uart_transmit_rxBuffer[idx] = ((uint8) rxDataByte);
        }
        else
        {
            uart_transmit_rxBuffer[(uint32)(idx << 1u)]      = LO8(LO16(rxDataByte));
            uart_transmit_rxBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(rxDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: uart_transmit_GetWordFromRxBuffer
    ****************************************************************************//**
    *
    *  Reads byte/word from RX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \return
    *   Returns byte/word read from RX buffer.
    *
    *******************************************************************************/
    uint32 uart_transmit_GetWordFromRxBuffer(uint32 idx)
    {
        uint32 value;

        if (uart_transmit_ONE_BYTE_WIDTH == uart_transmit_rxDataBits)
        {
            value = uart_transmit_rxBuffer[idx];
        }
        else
        {
            value  = (uint32) uart_transmit_rxBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32)uart_transmit_rxBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return (value);
    }


    /*******************************************************************************
    * Function Name: uart_transmit_PutWordInTxBuffer
    ****************************************************************************//**
    *
    *  Stores byte/word into the TX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \param idx:        index to store data byte/word in the TX buffer.
    *  \param txDataByte: byte/word to store.
    *
    *******************************************************************************/
    void uart_transmit_PutWordInTxBuffer(uint32 idx, uint32 txDataByte)
    {
        /* Put data in buffer */
        if (uart_transmit_ONE_BYTE_WIDTH == uart_transmit_txDataBits)
        {
            uart_transmit_txBuffer[idx] = ((uint8) txDataByte);
        }
        else
        {
            uart_transmit_txBuffer[(uint32)(idx << 1u)]      = LO8(LO16(txDataByte));
            uart_transmit_txBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(txDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: uart_transmit_GetWordFromTxBuffer
    ****************************************************************************//**
    *
    *  Reads byte/word from the TX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \param idx: index to get data byte/word from the TX buffer.
    *
    *  \return
    *   Returns byte/word read from the TX buffer.
    *
    *******************************************************************************/
    uint32 uart_transmit_GetWordFromTxBuffer(uint32 idx)
    {
        uint32 value;

        if (uart_transmit_ONE_BYTE_WIDTH == uart_transmit_txDataBits)
        {
            value = (uint32) uart_transmit_txBuffer[idx];
        }
        else
        {
            value  = (uint32) uart_transmit_txBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32) uart_transmit_txBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return (value);
    }

#endif /* (uart_transmit_SCB_MODE_UNCONFIG_CONST_CFG) */


/* [] END OF FILE */
