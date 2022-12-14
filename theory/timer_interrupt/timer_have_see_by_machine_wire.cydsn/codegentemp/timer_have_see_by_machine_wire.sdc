# THIS FILE IS AUTOMATICALLY GENERATED
# Project: D:\psoc\theory\timer_interrupt\timer_have_see_by_machine_wire.cydsn\timer_have_see_by_machine_wire.cyprj
# Date: Tue, 06 Sep 2022 04:00:15 GMT
#set_units -time ns
create_clock -name {Clock(FFB)} -period 10000 -waveform {0 5000} [list [get_pins {ClockBlock/ff_div_8}]]
create_clock -name {CyRouted1} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/dsi_in_0}]]
create_clock -name {CyILO} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyLFClk} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/lfclk}]]
create_clock -name {CyIMO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyHFClk} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/hfclk}]]
create_clock -name {CySysClk} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/sysclk}]]
create_generated_clock -name {Clock} -source [get_pins {ClockBlock/hfclk}] -edges {1 241 481} [list]


# Component constraints for D:\psoc\theory\timer_interrupt\timer_have_see_by_machine_wire.cydsn\TopDesign\TopDesign.cysch
# Project: D:\psoc\theory\timer_interrupt\timer_have_see_by_machine_wire.cydsn\timer_have_see_by_machine_wire.cyprj
# Date: Tue, 06 Sep 2022 04:00:12 GMT
