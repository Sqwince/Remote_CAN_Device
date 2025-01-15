# CAN_INPUT_BRIDGE
Input hub for OpenFFBoard (STM32) that re-transmits button and analog inputs over CAN (4-wire)

The CAN_INPUT_BRIDGE was developed to work with the OpenFFBoard-hardware. 


For Reference:
Open FFBoard
The Open FFBoard is an open source force feedback interface with the goal of creating a platform for highly compatible FFB simulation devices like steering wheels and joysticks.
This firmware is optimized for the Open FFBoard and mainly designed for use with DD steering wheels. Remember this software is experimental and is intended for advanced users. Features may contain errors and can change at any time.
More documentation about this project is on the hackaday.io page.
The hardware designs are found under OpenFFBoard-hardware.
The GUI for configuration is found at OpenFFBoard-configurator.
These git submodules can be pulled with git submodule init and git submodule update
Updates often require matching firmware and GUI versions!

Documentation
Documentation will be updated in the GitHub Wiki.
Available commands are listed on the Commands wiki page
Code summary and documentation of the latest stable version is available as a Doxygen site.
For discussion and progress updates we have a Discord server.

Extensions
The modular structure means you are free to implement your own main classes. Take a look into the FFBoardMain and ExampleMain class files in the UserExtensions folder. Helper functions for parsing CDC commands and accessing the flash are included.
The firmware is class based in a way that for example the whole main class can be changed at runtime and with it for example even the usb device and complete behavior of the firmware.
For FFB the motor drivers, button sources or encoders also have their own interfaces.
A unified command system supporting different interfaces is available and recommended for setting parameters at runtime. (see CommandHandler.h and the example mainclass)

Copyright notice:
Some parts of this software may contain third party libraries and source code licenced under different terms. The license applying to these files is found in the header of the file. For all other parts in the Firmware/FFBoard folder the LICENSE file applies.
