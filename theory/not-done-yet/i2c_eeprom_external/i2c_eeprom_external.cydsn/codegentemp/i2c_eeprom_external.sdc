# THIS FILE IS AUTOMATICALLY GENERATED
# Project: D:\psoc\theory\i2c_eeprom_external\i2c_eeprom_external.cydsn\i2c_eeprom_external.cyprj
# Date: Mon, 22 Aug 2022 06:54:27 GMT
#set_units -time ns
create_clock -name {EZI2C_1_SCBCLK(FFB)} -period 625 -waveform {0 312.5} [list [get_pins {ClockBlock/ff_div_3}]]
create_clock -name {CyRouted1} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/dsi_in_0}]]
create_clock -name {CyILO} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyLFClk} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/lfclk}]]
create_clock -name {CyIMO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyHFClk} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/hfclk}]]
create_clock -name {CySysClk} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/sysclk}]]
create_generated_clock -name {EZI2C_1_SCBCLK} -source [get_pins {ClockBlock/hfclk}] -edges {1 15 31} [list]


# Component constraints for D:\psoc\theory\i2c_eeprom_external\i2c_eeprom_external.cydsn\TopDesign\TopDesign.cysch
# Project: D:\psoc\theory\i2c_eeprom_external\i2c_eeprom_external.cydsn\i2c_eeprom_external.cyprj
# Date: Mon, 22 Aug 2022 06:54:23 GMT
