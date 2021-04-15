=================================================
Names: Raj Shreyas Penukonda, Yevhen Kaznovskyi
IDs: 1623713, 1623127
CMPUT 275, Winter 2020
Major Assignment #2 Part 2
=================================================

Included Files:
* README
* client
* server

Accessories:
* Arduino Mega Board (AMG) x1
* Jumper Wires x5
* USB - Arduino Connector Cable x1
* Breadboard x1
* Joystick x1
* 3.5" TFT LCD 480x320 Arduino Touch Screen
* 5 Female-to-male wires
* 2 Pushbuttons with internal pullup resistors

Wiring Instructions:
1. Connect port A9 and A8 on to the breadboard separately.
2. Align the pins of the TFT Touch Screen with the Arduino and plug in the screen in the Arduino.
3. Connect the positive row of breadboard into the 5V port of Arduino and negative row to GND in Arduino.
4. Connect the 5 pins of the joystick into the Female-to-male wires and the other side as such:
              VRx <--> A8
              VRy <--> A9
              SW  <--> Digital Pin 53
              GND <--> GND
              +5V <--> 5V
        8. Connect the pushbuttons with one side <--> GND and the other <--> digital pin.
               Zoom Out Button <--> Digital Pin 47
               Zoom In Button  <--> Digital Pin 45

Running  Instructions:
1. Connect the Arduino to the PC using an A-B style  USB  cable. Ensure that the  Arduino is using the proper serial port (/dev/ttyACM0  or -ACM1).
2. Compile and upload the code to arduino by being in the client directory and using command: "make upload".
3. Compile and run the server side by being in the server directory and using the command: "make".

Notes and Assumptions:
The code is unfinished as we are unable to fix a bug. We think it is because the server does not wait for a new acknowledment but sends the next way point because it reading a previous acknowledment. As a result of this, we think the longitude and latitude stored are wrong and so the the route drawn is wrong.
