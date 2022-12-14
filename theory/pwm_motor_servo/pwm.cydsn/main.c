/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "main.h"

#include "servo.h"
#include "ledSingle.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    configSystem();
    
    int value = 50;
    
    for(;;)
    {
        value = 50;
        from0DegreeTo180Degree(&value);
        CyDelay(1000);
        
        value = 100;
        from0DegreeTo180Degree(&value);
        CyDelay(1000);
    }
}

void configSystem()
{
    pwm_Start();
}


/* [] END OF FILE */
