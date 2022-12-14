/*******************************************************************************
* File Name: pwmToServer.h  
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

#if !defined(CY_PINS_pwmToServer_ALIASES_H) /* Pins pwmToServer_ALIASES_H */
#define CY_PINS_pwmToServer_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define pwmToServer_0			(pwmToServer__0__PC)
#define pwmToServer_0_PS		(pwmToServer__0__PS)
#define pwmToServer_0_PC		(pwmToServer__0__PC)
#define pwmToServer_0_DR		(pwmToServer__0__DR)
#define pwmToServer_0_SHIFT	(pwmToServer__0__SHIFT)
#define pwmToServer_0_INTR	((uint16)((uint16)0x0003u << (pwmToServer__0__SHIFT*2u)))

#define pwmToServer_INTR_ALL	 ((uint16)(pwmToServer_0_INTR))


#endif /* End Pins pwmToServer_ALIASES_H */


/* [] END OF FILE */
