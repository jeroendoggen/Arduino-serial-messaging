SerialPacket Controller: Controller for the SerialPacket protocol
=================================================================
A controller application to show Python to Arduino communication using the SerialPacket protocol

Program flow:
 1. Connect to the Arduino over the serial port
 2. Start the "SerialPacket Controller"
 3. The controller reads incoming data
 4. The controller starts polling for commands 

Installation:
-------------
 * Install the Debian package: ``sudo dpkg -i python-sp-controller_0.x.x-x_all.deb`` 
 * Download the source and run ``sudo python setup.py install``

Usage:
------
 * Start the program with: ``python -m sp_controller``
 

Requirements:
-------------
 * Python 2.6+
 * python-serial (e.g. Ubuntu package)


Limitations:
------------
 * probably still some bugs

License:
--------
If not stated otherwise sp_controller is distributed in terms of the GPLv2 software license.
See COPYING in the distribution for details.

Bug reports:
------------
 * Jeroen Doggen <jeroendoggen@gmail.com>

Roadmap:
--------

0.3: ??

Changelog:
----------
 
0.0.2: First threaded version
 * Threads started from controller
 * Thread 1: Read data from serial port (SerialCommunication class)
 * Thread 2: Poll the database for commands (DatabaseCommunication class)
 * Sleeptime for all thread can be changed at runtime

0.0.1: Initial version: 
 * startup using cli arguments
 * working PyPI compatible python package
 * connect to serial
 * print info on cli + write to logfile
