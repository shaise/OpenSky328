# OpenSkySim
An atmel implementation of OpenSky FrSky-compatible RX  
Based on https://github.com/fishpepper/OpenSky

## Description
This open project aims to develop a family of small rc receivers compatibe with FR-Sky with telemetry.
List of devices:
* FR-8: An 8 channel PWM receiver with 2 analog telemetry inputs.
* FR-Tiny: A 5 channel micro receiver with brushed motor ESC, for micro rc planes. Weights just 2 grams.
* FR-LVH: A 1S land vehicle version with 2 bidirectional brushed motor esc (ch:1,2) and 2 servos (ch:3,4).
* Programmer adapter: Lets you connect above devices to a USBASP programmer, and USB-SERIAL adapter
The hardware is bases on Atmega 329p microcontroller and TI CC2500 transceiver. 
The firmware is based on OpenSky FrSky-compatible receiver: https://github.com/fishpepper/OpenSky
It is also arduino based.

## 1. FR-8
<img src="/Media/FR8_Pinout.jpg" width="400">

## 2. FR-Tiny
<img src="/Media/FRTiny_Pinout.jpg" width="400">

## 3. FR-LVH
<img src="/Media/FRLVH_Pinout.jpg" width="400">

## 4. FR-Pgm programmer
<img src="/Media/FRPgm_Pinout.jpg" width="400">


## Assembling instructions

* Order the PCB by sending the gerbrt files to a manufacturer of your choice. I recommend [JLCPCB](https://jlcpcb.com/).
  * [FR-8 gerber files](/Release/gerbers_fr8_V1_2Layer_FR4_1.6mm.zip) and [FR-8 pins gerber files](/Release/gerbers_frmini8_pins_V1_2Layer_FR4_1.6mm.zip)
  * [FR-Tiny](/Release/gerbers_frtiny_V5_4Layer_FR4_1.0mm.zip)
  * [FR-LVH](/Release/gerbers_frlvh_V2_SWPWM_4Layer_FR4_1.0mm.zip)
  * [FR-Pgm](/Release/gerbers_frmini_prog_V3_2Layer_FR4_1.6mm.zip)
* Order all parts as listed in the BOM file. You will need a 0603 kit of resistors and capacitors, and for all other parts you will find a link in the BOM.
  * [FR-8 BOM](/Release/FR8_bom.pdf)
  * [FR-Tiny BOM](/Release/FRTiny.pdf)
  * [FR-LVH BOM](/Release/FRLVH_bom.pdf)
  * [FR-Pgm BOM](/Release/FRReceiverProg.pdf)
* Solder all parts. You can use the interactive BOM to assist you.
  * [FR-8 Interactive BOM](https://htmlpreview.github.io/?https://github.com/shaise/OpenSky328/blob/main/Hardware/FRReceiver8Pcb/bom/FR8_bom.html)
  * [FR-Tiny Interactive BOM](https://htmlpreview.github.io/?https://github.com/shaise/OpenSky328/blob/main/Hardware/FRReceiverTinyPcb/bom/FRTiny.html)
  * [FR-LVH Interactive BOM](https://htmlpreview.github.io/?https://github.com/shaise/OpenSky328/blob/main/Hardware/FRReceiverLVH/bom/FRLVH_bom.html)
  * [FR-Pgm Interactive BOM](https://htmlpreview.github.io/?https://github.com/shaise/OpenSky328/blob/main/Hardware/FRReceiverProgPcb/bom/FRReceiverProg.html)

 ## Programming the firmware
* Clone or download this repository.
* Edit config.h file and set the correct PCB version by uncommenting the correct line:  

Comment | Board support  
--- | ---  
#define RXBOARD_FR8_FRTINY | FR-8 or FR-Tiny boards  
#define RXBOARD_FRLVH | FR-LVH land vehicle boards  
* You will need a USBASB compatible programmer and either the programmin adapter above (recomended) or connect your own wires.
* NOTE!!! USBASP must be 3.3V version or you risk damaging the receiver.
* The software is arduino compatible (Arduino nano), so it should work with the Arduino IDE, however I did not test that. Instead I recommend using the [VS Studio Code IDE](https://code.visualstudio.com). You will need to install the C/C++ Addon, and the Platform IO addon (from the addon menu). Within Platform IO install the Atmel AVR platform.
* Connect the built receiver to the programmer using the adapter as follows:  
For FR-8:  
<img src="/Media/FR8_connect.jpg" width="400">
  
For FR-Tiny:  
<img src="/Media/FRTinyConnect.jpg" width="400">
  
For FR-LVH:  
<img src="/Media/FRLVH_Connect.jpg" width="400">
  
* Now open the 'Software' folder within VS Code. If you get a mesage that a workspace was detected, go ahead and press the "open workspace" button.
* If everything is connected correctly, just press the 'PlatformIO: Build" button at the bottom (the right arrow) and thats it!

## Usage
These receivers works with FR-Sky telemetry compatible transmitters (D8). (SW 1 and 2 off on DJT modules)
### Binding
You first need to bind the receiver with the transmitter (this is a one-time operation, repeat only if you change transmitter):  
* Press and hold the bind button and then powerup the receiver. Wait for the green light to turn on.
* Start binding procedure in your transmitter. If you are using an external module such as the DJT, Press and hold the module bind button and turn on the transmitter. 
* Wait for the red led on the receiver to start blinking. When it does, binding is complete! Turn off the transmitter and remove the receiver battery.
### Setting Fail safe
turn the tranmitter on then the receiver. Wait for the receiver green light to turn on. Press and hold the Bind button on the receiver for about 5 seconds till the red led turn on. Release the button, the red led will start blinking. You now have 10 seconds to set your fail safe state using the transmitter. When the time is up, the receiver will lock this state as the failsafe mode, and restart. when the green light is on again (without the red) the process is complete. This method of setting the fail safe mode is better then the standard, since you have both hands free to move the sticks to the desired state.
### Serial debugging
If you want to debug the receiver operation, you can use a serial terminal (built in VS code) to observe the debug data sent from the receiver.  
* Use a USB to serial Adapter. If you want to use the USB-serial to power the module, Make sure its a 3.3V version, or you risk damaging the modules.
* Connect the USB-Serial adapter to the programming adapter using the marked pins.
* Connect the programming adapter to the module as explained in the programming section.
* For the FR-Tiny you are all set. For the FR-8 / FR-LVH you will need an extra cable (jst to jst 1.0 mm 3pin) to connect the module to the serial input:  
<img src="/Media/FR8_serial.jpg" width="400">
 





