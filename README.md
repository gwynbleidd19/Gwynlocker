# Gwynlocker
PIN code lock simulator on Arduino Nano  with LCD (i2c) and Keypad 

Project name: Gwynlocker 
Author: Starykh Kirill
Microcontroller: Arduino Nano
Environment: VS Code (Platformio)
Please read to the end :)
==================================

===Description

A hardware-based PIN code lock simulator built on Arduino Nano. This project demonstrates the integration of a 4x4 Matrix Keypad, an I2C LCD display, and a 74HC595 shift register for LED indicator control with visual feedback.

===Key Features

- 4x4 Matrix Keypad for secure PIN entry.
- 16x2 I2C LCD for user-friendly status prompts (e.g., "Enter PIN", "UNLOCKED").
- 74HC595 Shift Register to drive 6 LED indicators (3 green for success, 3 red for failure), saving valuable GPIO pins.
- Onboard Buzzer for audible feedback on key presses and lock status.

===How It Works

1. The user enters a 4-digit PIN using the keypad and confirms with the "#" key.
2. The system compares the input against a pre-defined code.
3. Correct PIN: Green LEDs light up sequentially, the LCD displays "UNLOCKED", and a success tone plays.
4. Incorrect PIN: Red LEDs light up, the LCD displays "DENIED", and an error tone plays.
5. Security Feature: After 3 failed attempts, the system activates a 10-second lockout period (red LEDs blink) to prevent brute-force attacks.

===Hardware Components

- Arduino Nano (Microcontroller)
- 16x2 I2C LCD Display
- 4x4 Matrix Keypad
- 74HC595N Shift Register
- 6x LEDs (3 Green, 3 Red) with 220Ω resistors
- Passive Buzzer (5V)


===Pinout Summary

The full wiring schema can be found in "Full pinout.txt" and "SN74HC595N pinout.txt" inside this directory.

===Key connections

- Keypad: Digital pins D2 – D9.
- Shift Register: D10 (Latch), D11 (Data), D12 (Clock).
- LCD: I2C interface (SDA -> A4, SCL -> A5).
- Buzzer: Analog pin A0.

===Tinkercad link (more detailed scheme than .png in "Media")

https://www.tinkercad.com/things/cazRo5YytLW-pincode

===Bill of materials

1x Arduino Nano
1x 16x2 I2C LCD Display
1x 4x4 Matrix Keypad
1x 74HC595N Shift Register
1x 5V Buzzer
6x LEDs (3 Green, 3 Red) with 220Ω resistors

===Media

Check the "Media" folder for photos and a demonstration video of the device in action.

===How to use

1) Open VS Code
2) Download "Platformio" intension in VS Code (If haven't downloaded yet).
3) In platformio click "Open folder".
4) Click "Code" folder.
5) Find "platformio.ini" file and click "Build" in the bottom of the toolbar.  
6) Connect components according to "Visual scheme" in the "Media" folder, or check "Tinkercad link", that give you more flexibility in watching.
7) Make sure your Arduino Nano is connected via USB. Click the "Upload" button (➡) in the bottom toolbar. 

===Notes

- I used an Uno in the diagram because Tinkercad doesn't have a Nano model, but the physical hardware uses a Nano.

- If you want to make a useful device from this, you can add a step motor or relay to real opening something by means of this device. Device like this is created for that, general structure and logic have already built in this project, add some component to make this device useful is straightforward, but i wasn't be able to do that because I have no stuff to power relay or step motor.  

===License

This project is licensed under the MIT License - see the "LICENSE" file for details. 

