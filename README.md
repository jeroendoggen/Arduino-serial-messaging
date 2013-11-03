Library to exchange short messages (sensordata, commands) between an Arduino and a software application running on a PC. (Linux, embedded Linux, Windows, OS X) (clients currently under development)

Both sides are able send and receive a range of "standardised" messages over the serial port. All communication is done by sending short human readable ASCII messages. We define several standard command and data packet IDs to provide interoperability between different applications.

The protocol was designed to be relatively easy to comprehend and process by both human and computer.
 * A standardised message structure (more info below)
 * All fields in a message are separated by delimiters (just a letter indicating what the next field will be)
 * All communication is in HEX ASCII values (to allow human and computer to understand the packets)
 * Invalid messages are detected by calculating the parity (XOR all serial bytes) 

## The Message Types

The library support seven basic packet types.

 * "Command": start motor, blink LED, go to sleep mode,...
 * "Command Reply": e.g. motor started, LED blinking, going to sleep mode now,...
 * "8-bit Data": (standard Arduino Byte type): e.g. temperature is 25 C, distance is 50 cm, humidity is 56%
 * "16-bit Data": (standard Arduino int type): e.g. temperature is -25 C, distance is 310 cm, time is 16200 seconds, ...
 * "Data array": send multiple sensor measurements in one burst (useful when sampling very rapidly)
 * "Data request": e.g. send me the temperature, distance, humidity, ...
 * "Data array request": e.g. send me a burst of measurements

More info in [`defines.h`](https://github.com/jeroendoggen/Arduino-serial-messaging/blob/master/SerialPacket/defines.h)

## Standard Messages

*Data messages* are used when we want to send sensor data from the Arduino to applications running on a PC. [More info](https://github.com/jeroendoggen/Arduino-serial-messaging/wiki/Sensordata-messages)

*Command messages* are used when we want to send instructions from one application to the Arduino module. [More info](https://github.com/jeroendoggen/Arduino-serial-messaging/wiki/Command-messages)

We define several standard command and data message IDs. The actual meaning of the payload of these messages is standardised to provide interoperability between different applications (00h -> 7Fh). Half of the packet IDs are undefined (80h -> FFh). These IDs can be chosen based on the needs of your own application.


## Example Messages

Command message **T01N00I12PFFQ21** : "set motor speed of 'Arduino zero' to +100%:

 * **T01**: Type      01: Command message
 * **N00**: Number    00: Node number 00 (is the destination)
 * **I12**: CommandID 12: Set motor speed
 * **PFF**: Payload   FF: full speed (range: 0 (reverse) -> 80 (stopped) -> FF (forward))
 * **Q21**: Quality   21: parity byte is 21

Data message **T12N00I10P08Q0A** : "temperature of 'Arduino zero' is 8 degrees"

 * **T12**: Type      12: Data message (1 byte payload)
 * **N00**: Number    00: Node number 00 (is the source)
 * **I10**: SensorID  10: Temperature
 * **P08**: Payload   08: 8 degrees
 * **Q0A**: Quality   0A: parity byte is 0A


## Sending Messages from Arduino Code

 * sendCommand(commandID,payload)
 * sendCommandReply(commandID,payload)

 * sendData(sensorID,payload)
 * sendDataRequest(sensorID,payload)

 * sendDataArray(sensorID,payload)
 * sendDataArrayRequest(sensorID,payload)

Example sketch: [`SendPackets.ino`](https://github.com/jeroendoggen/Arduino-serial-messaging/blob/master/SerialPacket/examples/SendPackets/SendPackets.ino)

## PC Clients

 * [Python library](https://github.com/jeroendoggen/Arduino-serial-messaging/tree/master/Clients/Python): receive & send packets (using queues & threads)
 * [C# library](https://github.com/jeroendoggen/Arduino-serial-messaging/tree/master/Clients): send & receive messages (coming soon)
 * [RRDTool sensorlogging](https://github.com/jeroendoggen/Arduino-serial-messaging/tree/master/Clients/Collectd): write Arduino sensor data into an RRDtool database using collectd
 * [Packet Inspector](https://github.com/jeroendoggen/Arduino-serial-messaging/tree/master/Clients/Processing): a GUI written in the Processing language to show a incoming data packets
