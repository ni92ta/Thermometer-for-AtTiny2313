Thermometer for DS18x20 series sensor.
I was inspired to create this project by the desire to modernize the thermometer I had previously assembled. 
I ended up designing my own PCB, writing the firmware, and making minor changes to the original circuit.
Schematic diagram taken from the site "Radiokot".
https://radiokot.ru/circuit/digital/home/26/.   
The circuit diagram and printed circuit board were developed in the Altium Designer environment. The firmware was written in the Microchip Studio development environment.
I just added a pull-up to the reset pin of the MK, picked up current-limiting resistors for the LED indicator and increased the capacitance of the filter capacitor. 
You can also add transistor switches to the indicator cathodes to reduce the current load on the MK.
The resulting circuit diagram is shown in the screenshot below:

![image](https://user-images.githubusercontent.com/91215296/208711816-4750ac30-1270-408d-811f-f7d0f28af8c8.png)

The thermometer is built on the basis of MK AtTiny 2313, an LED indicator with a common cathode is responsible for displaying temperature readings. Measurements are carried out by sensors of the 18x20 series. Sensor 18b20 for measuring outside temperature, sensor 1820 for measuring indoor temperature.

![Thermometer](https://user-images.githubusercontent.com/91215296/208718909-fbaab430-eb3d-4233-a7be-2d1d23f1d868.gif)

The printed circuit board is shown in the screenshots below:

![Thermometer_3D_PCB_Top](https://user-images.githubusercontent.com/91215296/208720142-4f7ca8df-feb0-45ff-a7dd-ff00014941d8.png)

![Thermometer_3D_PCB_Bottom](https://user-images.githubusercontent.com/91215296/208720131-e4f7f64c-4b91-4fa7-b2e6-1865d56d94f3.png)

![Thermometer_PCB_Bottom](https://user-images.githubusercontent.com/91215296/208720101-700be5a2-36b0-4ed6-bdc7-f6b934a702ee.png)




