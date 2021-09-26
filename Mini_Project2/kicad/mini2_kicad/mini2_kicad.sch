EESchema Schematic File Version 4
EELAYER 30 0
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
L MCU_Module:Arduino_UNO_R3 A1
U 1 1 613CEE9E
P 4100 3800
F 0 "A1" H 4100 4981 50  0000 C CNN
F 1 "Arduino_UNO_R3" H 4550 5000 50  0000 C CNN
F 2 "Module:Arduino_UNO_R3" H 4100 3800 50  0001 C CIN
F 3 "https://www.arduino.cc/en/Main/arduinoBoardUno" H 4100 3800 50  0001 C CNN
	1    4100 3800
	1    0    0    -1  
$EndComp
Text GLabel 4400 2750 2    50   Input ~ 0
+5V
Wire Wire Line
	4300 2800 4300 2750
Wire Wire Line
	4300 2750 4400 2750
Text GLabel 4750 3900 2    50   Input ~ 0
PAN_POT_INPUT
Wire Wire Line
	4750 3900 4600 3900
$Comp
L power:GND #PWR03
U 1 1 613F7344
P 4000 4900
F 0 "#PWR03" H 4000 4650 50  0001 C CNN
F 1 "GND" H 4005 4727 50  0000 C CNN
F 2 "" H 4000 4900 50  0001 C CNN
F 3 "" H 4000 4900 50  0001 C CNN
	1    4000 4900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 613F7775
P 4100 4900
F 0 "#PWR04" H 4100 4650 50  0001 C CNN
F 1 "GND" H 4105 4727 50  0000 C CNN
F 2 "" H 4100 4900 50  0001 C CNN
F 3 "" H 4100 4900 50  0001 C CNN
	1    4100 4900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 613F7C3D
P 4200 4900
F 0 "#PWR05" H 4200 4650 50  0001 C CNN
F 1 "GND" H 4205 4727 50  0000 C CNN
F 2 "" H 4200 4900 50  0001 C CNN
F 3 "" H 4200 4900 50  0001 C CNN
	1    4200 4900
	1    0    0    -1  
$EndComp
NoConn ~ 3600 4000
NoConn ~ 3600 3900
NoConn ~ 3600 3800
NoConn ~ 3600 3700
NoConn ~ 3600 3600
NoConn ~ 3600 3500
NoConn ~ 3600 3300
NoConn ~ 3600 3200
NoConn ~ 4600 3200
NoConn ~ 4600 3400
NoConn ~ 4600 3600
NoConn ~ 4600 4100
NoConn ~ 4600 4200
NoConn ~ 4600 4300
NoConn ~ 4200 2800
NoConn ~ 4000 2800
NoConn ~ 4600 4500
NoConn ~ 4600 4600
Text GLabel 4750 3800 2    50   Input ~ 0
IR_INPUT
Text GLabel 4750 4000 2    50   Input ~ 0
TILT_POT_INPUT
Wire Wire Line
	4750 3800 4600 3800
Wire Wire Line
	4750 4000 4600 4000
$Comp
L mini_proj_2_lib:SHARP_IR_GP2Y0A02YK0F U1
U 1 1 6150DE41
P 5350 2850
F 0 "U1" H 5275 2775 50  0000 C CNN
F 1 "SHARP_IR_GP2Y0A02YK0F" H 5275 2684 50  0000 C CNN
F 2 "" H 5350 2850 50  0001 C CNN
F 3 "" H 5350 2850 50  0001 C CNN
	1    5350 2850
	1    0    0    -1  
$EndComp
Text GLabel 5050 3150 0    50   Input ~ 0
+5V
$Comp
L power:GND #PWR06
U 1 1 615109C2
P 5050 3350
F 0 "#PWR06" H 5050 3100 50  0001 C CNN
F 1 "GND" H 5055 3177 50  0000 C CNN
F 2 "" H 5050 3350 50  0001 C CNN
F 3 "" H 5050 3350 50  0001 C CNN
	1    5050 3350
	1    0    0    -1  
$EndComp
Text GLabel 5500 3250 2    50   Input ~ 0
IR_INPUT
NoConn ~ 3600 3400
NoConn ~ 3600 4300
NoConn ~ 3600 4400
NoConn ~ 3600 4500
$Comp
L Motor:Motor_Servo M1
U 1 1 61512FFC
P 3000 2950
F 0 "M1" H 3050 3300 50  0000 L CNN
F 1 "Motor_Servo" H 2850 3200 50  0000 L CNN
F 2 "" H 3000 2760 50  0001 C CNN
F 3 "http://forums.parallax.com/uploads/attachments/46831/74481.png" H 3000 2760 50  0001 C CNN
	1    3000 2950
	1    0    0    -1  
$EndComp
Text GLabel 2700 2950 0    50   Input ~ 0
+5V
$Comp
L power:GND #PWR01
U 1 1 61516E11
P 2600 3050
F 0 "#PWR01" H 2600 2800 50  0001 C CNN
F 1 "GND" H 2605 2877 50  0000 C CNN
F 2 "" H 2600 3050 50  0001 C CNN
F 3 "" H 2600 3050 50  0001 C CNN
	1    2600 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 3050 2700 3050
Text GLabel 2700 2850 0    50   Input ~ 0
SERVO_TILT_PWM
Text GLabel 3600 4100 0    50   Input ~ 0
SERVO_TILT_PWM
$Comp
L Motor:Motor_Servo M2
U 1 1 6151A7FB
P 3000 3550
F 0 "M2" H 3050 3900 50  0000 L CNN
F 1 "Motor_Servo" H 2850 3800 50  0000 L CNN
F 2 "" H 3000 3360 50  0001 C CNN
F 3 "http://forums.parallax.com/uploads/attachments/46831/74481.png" H 3000 3360 50  0001 C CNN
	1    3000 3550
	1    0    0    -1  
$EndComp
Text GLabel 2700 3550 0    50   Input ~ 0
+5V
$Comp
L power:GND #PWR02
U 1 1 6151A802
P 2600 3650
F 0 "#PWR02" H 2600 3400 50  0001 C CNN
F 1 "GND" H 2605 3477 50  0000 C CNN
F 2 "" H 2600 3650 50  0001 C CNN
F 3 "" H 2600 3650 50  0001 C CNN
	1    2600 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 3650 2700 3650
Text GLabel 2700 3450 0    50   Input ~ 0
SERVO_PAN_PWM
Text GLabel 3600 4200 0    50   Input ~ 0
SERVO_PAN_PWM
Wire Wire Line
	5250 4600 5250 4850
Wire Wire Line
	5050 4450 5100 4450
Text GLabel 5050 4450 0    50   Input ~ 0
+5V
$Comp
L power:GND #PWR07
U 1 1 6150C07D
P 5550 4450
F 0 "#PWR07" H 5550 4200 50  0001 C CNN
F 1 "GND" H 5555 4277 50  0000 C CNN
F 2 "" H 5550 4450 50  0001 C CNN
F 3 "" H 5550 4450 50  0001 C CNN
	1    5550 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 4450 5550 4450
$Comp
L Device:R_POT RV1
U 1 1 6150C076
P 5250 4450
F 0 "RV1" V 5043 4450 50  0000 C CNN
F 1 "1K" V 5134 4450 50  0000 C CNN
F 2 "" H 5250 4450 50  0001 C CNN
F 3 "~" H 5250 4450 50  0001 C CNN
	1    5250 4450
	0    1    1    0   
$EndComp
Wire Wire Line
	5250 5450 5250 5700
Wire Wire Line
	5050 5300 5100 5300
Text GLabel 5050 5300 0    50   Input ~ 0
+5V
$Comp
L power:GND #PWR08
U 1 1 613EF950
P 5550 5300
F 0 "#PWR08" H 5550 5050 50  0001 C CNN
F 1 "GND" H 5555 5127 50  0000 C CNN
F 2 "" H 5550 5300 50  0001 C CNN
F 3 "" H 5550 5300 50  0001 C CNN
	1    5550 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 5300 5550 5300
$Comp
L Device:R_POT RV2
U 1 1 613EDA88
P 5250 5300
F 0 "RV2" V 5043 5300 50  0000 C CNN
F 1 "1K" V 5134 5300 50  0000 C CNN
F 2 "" H 5250 5300 50  0001 C CNN
F 3 "~" H 5250 5300 50  0001 C CNN
	1    5250 5300
	0    1    1    0   
$EndComp
Text GLabel 5250 5700 2    50   Input ~ 0
TILT_POT_INPUT
Text GLabel 5250 4850 2    50   Input ~ 0
PAN_POT_INPUT
$EndSCHEMATC
