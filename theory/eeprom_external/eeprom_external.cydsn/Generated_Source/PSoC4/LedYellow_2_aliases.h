/*******************************************************************************
* File Name: LedYellow_2.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_LedYellow_2_ALIASES_H) /* Pins LedYellow_2_ALIASES_H */
#define CY_PINS_LedYellow_2_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define LedYellow_2_0			(LedYellow_2__0__PC)
#define LedYellow_2_0_PS		(LedYellow_2__0__PS)
#define LedYellow_2_0_PC		(LedYellow_2__0__PC)
#define LedYellow_2_0_DR		(LedYellow_2__0__DR)
#define LedYellow_2_0_SHIFT	(LedYellow_2__0__SHIFT)
#define LedYellow_2_0_INTR	((uint16)((uint16)0x0003u << (LedYellow_2__0__SHIFT*2u)))

#define LedYellow_2_INTR_ALL	 ((uint16)(LedYellow_2_0_INTR))


#endif /* End Pins LedYellow_2_ALIASES_H */


/* [] END OF FILE */
