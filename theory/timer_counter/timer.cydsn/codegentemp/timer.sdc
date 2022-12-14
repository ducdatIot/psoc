# THIS FILE IS AUTOMATICALLY GENERATED
# Project: D:\psoc\theory\timer\timer.cydsn\timer.cyprj
# Date: Fri, 26 Aug 2022 14:26:12 GMT
#set_units -time ns
create_clock -name {Clock_1(FFB)} -period 5000 -waveform {0 2500} [list [get_pins {ClockBlock/ff_div_8}]]
create_clock -name {Clock_2(FFB)} -period 100000 -waveform {0 50000} [list [get_pins {ClockBlock/ff_div_9}]]
create_clock -name {CyRouted1} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/dsi_in_0}]]
create_clock -name {CyILO} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyLFClk} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/lfclk}]]
create_clock -name {CyIMO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyHFClk} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/hfclk}]]
create_clock -name {CySysClk} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/sysclk}]]
create_generated_clock -name {Clock_1} -source [get_pins {ClockBlock/hfclk}] -edges {1 121 241} [list]
create_generated_clock -name {Clock_2} -source [get_pins {ClockBlock/hfclk}] -edges {1 2401 4801} [list]


# Component constraints for D:\psoc\theory\timer\timer.cydsn\TopDesign\TopDesign.cysch
# Project: D:\psoc\theory\timer\timer.cydsn\timer.cyprj
# Date: Fri, 26 Aug 2022 14:26:09 GMT
