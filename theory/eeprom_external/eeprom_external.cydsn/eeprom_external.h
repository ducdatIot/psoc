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
#ifndef EEPROM_EXTERNAL
#define EEPROM_EXTERNAL
    
    #include <cytypes.h>
    
    uint8 eeprom_read_a_byte_08( uint32  address_read);
    void eeprom_read_page_08(uint8 address, uint8 *data_read, uint8 length);
    
    void eeprom_write_a_byte_08(uint32 address, uint8 data_need_write);
    void eeprom_write_page_08(uint8 address, uint32 *data_need_write, uint8 length);
    
    
    
    uint8 eeprom_read_a_byte_256( uint32  address_read);
    void eeprom_read_page_256(uint8 address, uint8 *data_read, uint8 length);
    
    void eeprom_write_a_byte_256(uint32 address, uint8 data_need_write);
    void eeprom_write_page_256(uint8 address, uint32 *data_need_write, uint8 length);
      
#endif




/* [] END OF FILE */
