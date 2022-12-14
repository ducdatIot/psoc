/*******************************************************************************
* File Name: button1_isr.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_button1_isr_H)
#define CY_ISR_button1_isr_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void button1_isr_Start(void);
void button1_isr_StartEx(cyisraddress address);
void button1_isr_Stop(void);

CY_ISR_PROTO(button1_isr_Interrupt);

void button1_isr_SetVector(cyisraddress address);
cyisraddress button1_isr_GetVector(void);

void button1_isr_SetPriority(uint8 priority);
uint8 button1_isr_GetPriority(void);

void button1_isr_Enable(void);
uint8 button1_isr_GetState(void);
void button1_isr_Disable(void);

void button1_isr_SetPending(void);
void button1_isr_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the button1_isr ISR. */
#define button1_isr_INTC_VECTOR            ((reg32 *) button1_isr__INTC_VECT)

/* Address of the button1_isr ISR priority. */
#define button1_isr_INTC_PRIOR             ((reg32 *) button1_isr__INTC_PRIOR_REG)

/* Priority of the button1_isr interrupt. */
#define button1_isr_INTC_PRIOR_NUMBER      button1_isr__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable button1_isr interrupt. */
#define button1_isr_INTC_SET_EN            ((reg32 *) button1_isr__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the button1_isr interrupt. */
#define button1_isr_INTC_CLR_EN            ((reg32 *) button1_isr__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the button1_isr interrupt state to pending. */
#define button1_isr_INTC_SET_PD            ((reg32 *) button1_isr__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the button1_isr interrupt. */
#define button1_isr_INTC_CLR_PD            ((reg32 *) button1_isr__INTC_CLR_PD_REG)



#endif /* CY_ISR_button1_isr_H */


/* [] END OF FILE */
