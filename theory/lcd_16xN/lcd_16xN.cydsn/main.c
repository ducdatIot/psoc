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
#include "main.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    configSystem();

    for(;;)
    {
        /* Place your application code here. */
        display();
    }
}

void configSystem()
{
   lcd_Start();    
    
   lcd_Position(0u,0u);
   lcd_PrintString("hello wourld");
   CyDelay(3000);
   lcd_ClearDisplay();
}

void display()
{
    lcd_Position(0u,0u);
    lcd_PrintString("lcd 16x2");
}


/* [] END OF FILE */