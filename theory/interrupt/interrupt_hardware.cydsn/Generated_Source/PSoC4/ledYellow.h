/*******************************************************************************
* File Name: ledYellow.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_ledYellow_H) /* Pins ledYellow_H */
#define CY_PINS_ledYellow_H

#include "cytypes.h"
#include "cyfitter.h"
#include "ledYellow_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} ledYellow_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   ledYellow_Read(void);
void    ledYellow_Write(uint8 value);
uint8   ledYellow_ReadDataReg(void);
#if defined(ledYellow__PC) || (CY_PSOC4_4200L) 
    void    ledYellow_SetDriveMode(uint8 mode);
#endif
void    ledYellow_SetInterruptMode(uint16 position, uint16 mode);
uint8   ledYellow_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void ledYellow_Sleep(void); 
void ledYellow_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(ledYellow__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define ledYellow_DRIVE_MODE_BITS        (3)
    #define ledYellow_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - ledYellow_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the ledYellow_SetDriveMode() function.
         *  @{
         */
        #define ledYellow_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define ledYellow_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define ledYellow_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define ledYellow_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define ledYellow_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define ledYellow_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define ledYellow_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define ledYellow_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define ledYellow_MASK               ledYellow__MASK
#define ledYellow_SHIFT              ledYellow__SHIFT
#define ledYellow_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in ledYellow_SetInterruptMode() function.
     *  @{
     */
        #define ledYellow_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define ledYellow_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define ledYellow_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define ledYellow_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(ledYellow__SIO)
    #define ledYellow_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(ledYellow__PC) && (CY_PSOC4_4200L)
    #define ledYellow_USBIO_ENABLE               ((uint32)0x80000000u)
    #define ledYellow_USBIO_DISABLE              ((uint32)(~ledYellow_USBIO_ENABLE))
    #define ledYellow_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define ledYellow_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define ledYellow_USBIO_ENTER_SLEEP          ((uint32)((1u << ledYellow_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << ledYellow_USBIO_SUSPEND_DEL_SHIFT)))
    #define ledYellow_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << ledYellow_USBIO_SUSPEND_SHIFT)))
    #define ledYellow_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << ledYellow_USBIO_SUSPEND_DEL_SHIFT)))
    #define ledYellow_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(ledYellow__PC)
    /* Port Configuration */
    #define ledYellow_PC                 (* (reg32 *) ledYellow__PC)
#endif
/* Pin State */
#define ledYellow_PS                     (* (reg32 *) ledYellow__PS)
/* Data Register */
#define ledYellow_DR                     (* (reg32 *) ledYellow__DR)
/* Input Buffer Disable Override */
#define ledYellow_INP_DIS                (* (reg32 *) ledYellow__PC2)

/* Interrupt configuration Registers */
#define ledYellow_INTCFG                 (* (reg32 *) ledYellow__INTCFG)
#define ledYellow_INTSTAT                (* (reg32 *) ledYellow__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define ledYellow_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(ledYellow__SIO)
    #define ledYellow_SIO_REG            (* (reg32 *) ledYellow__SIO)
#endif /* (ledYellow__SIO_CFG) */

/* USBIO registers */
#if !defined(ledYellow__PC) && (CY_PSOC4_4200L)
    #define ledYellow_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define ledYellow_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define ledYellow_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define ledYellow_DRIVE_MODE_SHIFT       (0x00u)
#define ledYellow_DRIVE_MODE_MASK        (0x07u << ledYellow_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins ledYellow_H */


/* [] END OF FILE */
