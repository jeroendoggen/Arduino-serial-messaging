# Arduino-serial-messaging

Library to exchange short messages (sensordata, commands) between an Arduino and a software application running on a PC. (Linux, embedded Linux, Windows, OS X) (clients currently under development)

Both sides are able send and receive a range of "standardised" messages over the serial port. All communication is done by sending short human readable ASCII messages. We define several standard command and data packet IDs to provide interoperability between different applications.

## The Message Types

The library support seven basic packet types.

 * "Command": start motor, blink LED, go to sleep mode,...
 * "Command Reply": e.g. motor started, LED blinking, going to sleep mode now,...
 * "8-bit Data": (standard Arduino Byte type): e.g. temperature is 25 C, distance is 50 cm, humidity is 56%
 * "16-bit Data": (standard Arduino int type): e.g. temperature is -25 C, distance is 310 cm, time is 16200 seconds, ...
 * "Data array": send multiple sensor measurements in one burst (useful when sampling very rapidly)
 * "Data request": e.g. send me the temperature, distance, humidity, ...
 * "Data array request": e.g. send me a burst of measurements

More info in "defines.h"

## Standard Messages

*Data messages* are used when we want to send sensor data from the Arduino to applications running on a PCs. [http://code.google.com/p/arduino-serial-packet/wiki/Sensordata More info]

*Command messages* are used when we want to send instructions from one application to the Arduino module. [http://code.google.com/p/arduino-serial-packet/wiki/Commands More info]

We define several standard command and data messages IDs. The actual meaning of the payload of these messages is standardised to provide interoperability between different applications (00h -> 7Fh). Half of the packet IDs are undefined (80h->FFh), these IDs can be chosen based on the needs of your own application.


## Sending Messages from Arduino Code

 * sendCommand(commandID,payload)
 * sendCommandReply(commandID,payload)

 * sendData(sensorID,payload)
 * sendDataRequest(sensorID,payload)

 * sendDataArray(sensorID,payload)
 * sendDataArrayRequest(sensorID,payload)

Example sketch: SendPackets.ino

## PC Clients

 * Sensorlogging with collectd
 * Packet_Inspector_processing Packet Inspector
 * PythonLibrary Python library
 * C# library
