/*OpenFFboard External Input Controller
========================================
Description: Reads Analog & Digital inputs 
and reports states over CAN to OpenFFBoard-Main
which hosts the motor driver and provides a 
single interface to the PC.

Supports OpenFFB SPI Buttons,
 Analog Axis (joystick, pedals, etc),
 and up to 4 rotary knob encoders


KNOWN ISSUE: Each Rotary encoder will consume 2 digital outputs from the SPI Buttons.
Unable to map more than 32 outputs to HID for some reason at the moment even
though the CAN digital frame supposedly supports up to 64 inputs.

AUTHOR: Sqwince
VERSION: 0.1

LICENSE
--------
The MIT License (MIT)

Copyright© 2025 Sqwince

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/