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
* Order all parts as listed in the BOM file. You will need a 0603 kit of resistors and capacitors, and for all other parts you will find a link in the BOM.
  * [FR-8 BOM](/Release/FR8_bom.pdf)
  * [FR-Tiny BOM](/Release/FRTiny.pdf)
  * [FR-Pgm BOM](/Release/FRReceiverProg.pdf)
* Solder all parts. You can use the interactive BOM to assist you.
  * [FR-8 Interactive BOM](https://htmlpreview.github.io/?https://github.com/shaise/OpenSky328/blob/main/Hardware/FRReceiver8Pcb/bom/FR8_bom.html)
  * [FR-Tiny Interactive BOM](https://htmlpreview.github.io/?https://github.com/shaise/OpenSky328/blob/main/Hardware/FRReceiverTinyPcb/bom/FRTiny.html)
  * [FR-Pgm Interactive BOM](https://htmlpreview.github.io/?https://github.com/shaise/OpenSky328/blob/main/Hardware/FRReceiverProgPcb/bom/FRReceiverProg.html)

 


