# Arduino 6 character security lock
Is security lock system designed using arduino nano. The program uses the EEPROM to save the password, so the password is kept on the device even when is powered off. It also uses the built in LED to display the unlocked state.

## Requirements
- Arduino Nano
- LM017L equivalent display
- 12 (3x4) key matrix keypad
- PWM servo motor
- 2 switches
  
## Features
- 6 character pin combination
- Saving and reading the pin password to EEPROM memory
- Locking and unlocking with key switches
- Password reset when is in unlocked state
- Algorithm against brute force attack

## Schematic
Connect pin D2-8 to the keypad.<br/>
Connect pin D9 to the PWM servo motor, if you change it to a different pin ensure that the pin can provide PWM output.<br/>
D3, D5, D6, D9, D10 and D11 are pins that provide PWM output. The servo rotates from 0 to 180 degrees.<br/>
Connect the two switches to 5v and ground, using a pull down resistor provide the output to pins D10 and D11. D10 switch is for the key while D11 for locking.<br/>
Pin VSS of the LM017L display gets connected to ground, while pin VDD to 5v. Connect the VEE/VO pin with potentiometer between 5v and ground, which is used to control the contrast.<br/>
Pin RS (Register Select) and E (Enable) get connected to pins A0 and A1 on arduino, while pin RW gets grounded which sets it in write mode.<br/>
4 bit transfer is being used so D4-7 pins on the display are connected to A2-A5 on arduino. If you want you can wire additional light on pin D13 to replace the builtin LED light.<br/>
![Left side of the schematic](https://github.com/martin-gjorgjiev/arduino-6-character-security-lock/blob/main/schematic_left.png)
![Right side of the schematic](https://github.com/martin-gjorgjiev/arduino-6-character-security-lock/blob/main/schematic_right.png)

## Usage
The keys 0-9 are used for normal character input, after input of 6 characters the code checks if the password matches.<br/>
The key asterisk (*) can be used to start password reset if the lock is unlocked.<br/>
The key hashtag (#) can be used to delete the password input or interrupt the password reset.<br/>
It features waiting time after wrong passcode input, with every wrong password input the waiting time increments exponentially.<br/>
