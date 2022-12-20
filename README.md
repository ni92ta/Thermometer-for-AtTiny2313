Thermometer for DS18x20 series sensor.
I was inspired to create this project by the desire to modernize the thermometer I had previously assembled. 
I ended up designing my own PCB, writing the firmware, and making minor changes to the original circuit.
Schematic diagram taken from the site "Radiokot".
https://radiokot.ru/circuit/digital/home/26/.   
The circuit diagram and printed circuit board were developed in the Altium Designer environment. The firmware was written in the Microchip Studio development environment.
I just added a pull-up to the reset pin of the MK, picked up current-limiting resistors for the LED indicator and increased the capacitance of the filter capacitor. 
You can also add transistor switches to the indicator cathodes to reduce the current load on the MK.
The resulting circuit diagram is shown in the screenshot below:

