/*******************************************************************************
* File Name: LedYellow_3.h  
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

#if !defined(CY_PINS_LedYellow_3_ALIASES_H) /* Pins LedYellow_3_ALIASES_H */
#define CY_PINS_LedYellow_3_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define LedYellow_3_0			(LedYellow_3__0__PC)
#define LedYellow_3_0_PS		(LedYellow_3__0__PS)
#define LedYellow_3_0_PC		(LedYellow_3__0__PC)
#define LedYellow_3_0_DR		(LedYellow_3__0__DR)
#define LedYellow_3_0_SHIFT	(LedYellow_3__0__SHIFT)
#define LedYellow_3_0_INTR	((uint16)((uint16)0x0003u << (LedYellow_3__0__SHIFT*2u)))

#define LedYellow_3_INTR_ALL	 ((uint16)(LedYellow_3_0_INTR))


#endif /* End Pins LedYellow_3_ALIASES_H */


/* [] END OF FILE */
