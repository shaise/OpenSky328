EESchema Schematic File Version 4
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:Conn_01x09 J1
U 1 1 5FC4F0DD
P 3100 2800
F 0 "J1" H 3180 2842 50  0000 L CNN
F 1 "Conn_01x09" H 3180 2751 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x09_P2.54mm_Vertical" H 3100 2800 50  0001 C CNN
F 3 "~" H 3100 2800 50  0001 C CNN
	1    3100 2800
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x09 J2
U 1 1 5FC4F125
P 4300 2800
F 0 "J2" H 4380 2842 50  0000 L CNN
F 1 "Conn_01x09" H 4380 2751 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x09_P2.54mm_Vertical" H 4300 2800 50  0001 C CNN
F 3 "~" H 4300 2800 50  0001 C CNN
	1    4300 2800
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J3
U 1 1 5FC4F162
P 5400 2500
F 0 "J3" H 5480 2492 50  0000 L CNN
F 1 "Conn_01x04" H 5480 2401 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 5400 2500 50  0001 C CNN
F 3 "~" H 5400 2500 50  0001 C CNN
	1    5400 2500
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J4
U 1 1 5FC4F1A0
P 5400 3100
F 0 "J4" H 5480 3092 50  0000 L CNN
F 1 "Conn_01x04" H 5480 3001 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 5400 3100 50  0001 C CNN
F 3 "~" H 5400 3100 50  0001 C CNN
	1    5400 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 2400 5200 2400
Wire Wire Line
	5200 3300 5100 3300
Wire Wire Line
	5100 3300 5100 3200
Wire Wire Line
	5200 2500 5100 2500
Connection ~ 5100 2500
Wire Wire Line
	5100 2500 5100 2400
Wire Wire Line
	5200 2600 5100 2600
Connection ~ 5100 2600
Wire Wire Line
	5100 2600 5100 2500
Wire Wire Line
	5200 2700 5100 2700
Connection ~ 5100 2700
Wire Wire Line
	5100 2700 5100 2600
Wire Wire Line
	5200 3000 5100 3000
Connection ~ 5100 3000
Wire Wire Line
	5100 3000 5100 2700
Wire Wire Line
	5200 3100 5100 3100
Connection ~ 5100 3100
Wire Wire Line
	5100 3100 5100 3000
Wire Wire Line
	5200 3200 5100 3200
Connection ~ 5100 3200
Wire Wire Line
	5100 3200 5100 3100
Wire Wire Line
	4100 2800 4000 2800
Wire Wire Line
	4000 2800 4000 2300
Wire Wire Line
	4000 2300 5100 2300
Wire Wire Line
	5100 2300 5100 2400
Connection ~ 5100 2400
Text Label 4550 2300 0    50   ~ 0
GND
Wire Wire Line
	4100 2400 3750 2400
Wire Wire Line
	3750 2400 3750 2500
Wire Wire Line
	3750 3200 4100 3200
Wire Wire Line
	4100 2500 3750 2500
Connection ~ 3750 2500
Wire Wire Line
	4100 2600 3750 2600
Wire Wire Line
	3750 2500 3750 2600
Connection ~ 3750 2600
Wire Wire Line
	3750 2600 3750 2700
Wire Wire Line
	4100 2700 3750 2700
Connection ~ 3750 2700
Wire Wire Line
	3750 2700 3750 2900
Wire Wire Line
	4100 2900 3750 2900
Connection ~ 3750 2900
Wire Wire Line
	3750 2900 3750 3000
Wire Wire Line
	4100 3000 3750 3000
Connection ~ 3750 3000
Wire Wire Line
	3750 3000 3750 3100
Wire Wire Line
	4100 3100 3750 3100
Connection ~ 3750 3100
Wire Wire Line
	3750 3100 3750 3200
Text Label 3800 2400 0    50   ~ 0
+5V
NoConn ~ 2900 2400
NoConn ~ 2900 2500
NoConn ~ 2900 2600
NoConn ~ 2900 2700
NoConn ~ 2900 3100
NoConn ~ 2900 3200
NoConn ~ 2900 2800
NoConn ~ 2900 2900
NoConn ~ 2900 3000
$EndSCHEMATC
