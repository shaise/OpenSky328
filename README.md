# OpenSkySim
An atmel implementation of OpenSky FrSky-compatible RX  
Based on https://github.com/fishpepper/OpenSky

## Description
This open project aims to develop a family of small rc receivers compatibe with FR-Sky with telemetry.
List of devices:
* FR-8: An 8 channel PWM receiver with 2 analog telemetry inputs.
* FR-Tiny: A 5 channel micro receiver with brushed motor ESC, for micro rc planes. Weights just 2 grams.
* Programmer adapter: Lets you connect above devices to a USBASP programmer, and USB-SERIAL adapter
The hardware is bases on Atmega 329p microcontroller and TI CC2500 transceiver. 
The firmware is based on OpenSky FrSky-compatible receiver: https://github.com/fishpepper/OpenSky
It is also arduino based.

## 1. FR-8
<img src="/Media/FR8_Pinout.jpg" width="400">

## 2. FR-Tiny
<img src="/Media/FRTiny_Pinout.jpg" width="400">

## 3. FR-Pgm programmer
<img src="/Media/FRPgm_Pinout.jpg" width="400">

## Assembling instructions

* Order the PCB by sending the gerbrt files to a manufacturer of your choice. I recommend [JLCPCB](https://jlcpcb.com/).
  * [FR-8 gerber files](/Release/gerbers_fr8_V1_2Layer_FR4_1.6mm.zip) and [FR-8 pins gerber files](/Release/gerbers_frmini8_pins_V1_2Layer_FR4_1.6mm.zip)
  * [FR-Tiny](/Release/gerbers_frtiny_V5_4Layer_FR4_1.0mm.zip)
  * [FR-Pgm](/Release/gerbers_frmini_prog_V3_2Layer_FR4_1.6mm.zip)
* Order all parts as listed in the [BOM](/Release/OpenSkySimBom.pdf) file. You will need a 0603 kit of resistors and capacitors, and for all other parts you will find a link in the BOM.
* Solder all parts. You can use the [Interactive BOM](https://htmlpreview.github.io/?https://github.com/shaise/OpenSkySim/blob/main/Hardware/Bom/Interactive_bom.html) to assist you. 
* For the transmitter module, I used one without built in antenna, So if you are using the same module, you will need to add an antenna by soldering a 0.5mm transformer wire to the antenna pin, then trim the wire to a length of 31.5mm. (I tried one module with on board antenna but the reception was bad, If you find one that works, please let me know). Plese note: there are parts on both side of the PC
<img src="/Media/frsim_antenna.jpg" width="400">

Top Side (click to enlarge):  
<img src="/Media/frsim_top.jpg" width="400">
* If you have an access to a 3D printer you can print a plastic enclosure. Download and print the 2 stl files in the ![/Release](/Release) folder. Use different color for the first 3 layers to get a nice text effect. 

## Programming the firmware
* You will need an ST-Link V2 compatible programmer and programming cable (links in the BOM)
* From the JST cable and Dupont connector kit prepare the programming cable as shown in the image (Note the colors!):
<img src="/Media/prog_cable.jpg" width="600">  

* Connect the cable between the programmer and dongle:  
<img src="/Media/prog_conn.jpg" width="300">  <img src="/Media/sim_prog_conn.jpg" width="300">
* Connect the Stlink programmer to the PC. (no need to connect the dongle, it is powered from the programmer)
* Download the OpenSkySim firmware, and use a programming software such as the ![ST Link Utility](https://www.st.com/en/development-tools/stsw-link004.html) and programm the dongle.  

## Usage
This dongle works with FR-Sky telemetry compatible transmitters. (SW 1 and 2 off on DJT modules)  
You first need to bind the dongle with the transmitter (this is a one-time operation, repeat only if you change transmitter):  
* Press and hold the bind button and insert the dongle to the PC. Wait for the green light to turn on.
<img src="/Media/frsim_bind.jpg"> 

* Start binding procedure in your transmitter. If you are using an external module such as the DJT, Press and hold the module bind button and turn on the transmitter. 
* Wait for the red led on the dongle to start blinking. When it does, binding is complete! Turn off the transmitter and pull the dongle out from the PC.  

From now on all you need to do to use the simulator is just by using the slide switch, select the operation mode: either 'Std' for standard HID compatible simulator or 'Pnx' for PhoenixRC, then insert the dongle to the PC and start Simm-ing!
<img src="/Media/frsim_select.jpg"> 

Remark: When using the dongle in 'Std' mode, the PC will recognize it as "OpenSky wireless sim dongle" HID game controller.

 


