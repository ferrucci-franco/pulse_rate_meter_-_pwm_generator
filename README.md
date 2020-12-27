# Pulse-rate meter and PWM generator with Modbus interface
This projet presents a custom-designed instrument that has two main features:
* measurement of the pulse-rate of a number of digital input signals, 
* generation of pulse-width modulation (PWM) signals. 

The instrument was implemented in a STM32F4 DISCOVERY board.

Regarding the first functionality, the device computes the pulse-rate of a number of digital inputs and 
transmits the information through a communication channel to a master device. More specifically, the 
device counts the number of rising edges of twelve digital inputs simultaneously. The counting process 
takes place during a series of consecutive one-second time frames. At the end of each time frame, the 
device updates the registers with the last pulse-counts. Since the time window duration is one second, 
the values of the registers are equal to the frequency of each input (in Hz). The master device (e.g. a 
computer) accesses the device’s registers through a polling process using Modbus-RTU protocol. 

As for the second functionality, the device generates two independent PWM signals, whose parameters 
(i.e. frequency and duty cycle) are set by the master device.

More information can be found in Chapter A5 of my PhD thesis, available here:
https://www.researchgate.net/publication/341896286_PV-driven_vapor_compression_cycle_integrating_a_solid-gas_thermochemical_storage_for_cooling_applications_Cycle_de_compression_de_vapeur_pilote_par_PV_en_integrant_un_systeme_de_stockage_thermochimiqu


