// SerialMessage.h - Library for sending serial data messages
// Copyright 2012 Jeroen Doggen (jeroendoggen@gmail.com)
//
// Version History:
//   Version 0.1: Send single data sample in a message
//   Version 0.2: Send arrays with multiple samples in a message
//   Version 0.3: Conditional compilation: SERIAL_ASCII vs SERIAL_BINARY (includes.h)
//   Version 0.4: hexPrinting helper, changed message types, commandID/message
//   Version 0.5: added 'sendCommand', 'sendData', ... functions (replacing the generic 'sendMessage')
//                removed condition compilation from v0.3
//                shortened serial ASCII messages ('Type' -> 'T', 'NodeID' -> 'N')
//   Version 0.6: Added support for messages with 16-bit payload (renamed message types -> hex values)
//                Added "dataArrayRequest" message
//   Version 0.7: processing incoming messages
// Roadmap:
//   Version 0.7: send 16-bit data arrays
//                Separate classes for commands vs data messages?
//   Version 0.8: ??
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef SerialMessage_h
#define SerialMessage_h
#include <Arduino.h>

class SerialMessage
{
  public:
    SerialMessage();

    void begin();
    void begin(long speed, uint8_t nodeID);

    void sendCommand(uint8_t commandID, uint8_t payload);
    void sendCommand(uint8_t payload);
    void sendCommandReply(uint8_t commandID, uint8_t payload);

    void sendDataRequest(uint8_t sensorID, uint8_t payload);
    void sendData(uint8_t sensorID, uint8_t payload);
    void sendData(uint8_t sensorID, int16_t payload);

    void sendData(uint8_t payload);
    void sendData(int16_t payload);

    void sendDataArrayRequest(uint8_t arrayID, uint8_t length);
    void sendDataArray(uint8_t *dataArray, uint8_t length);

    boolean readSerialData();
    uint8_t getCommandID();
    uint8_t getPayload();

  private:
    struct message
    {
      uint8_t messageType;
      uint8_t nodeID;
      uint8_t sensorID;
      uint8_t commandID;
      uint8_t payload;
      uint8_t parity;
    } incomingMessage, outgoingMessage;             //TODO: also use this struct to send messages? (todo underscore)

    uint8_t _messageType;
    uint8_t _nodeID;
    uint8_t _sensorID;
    uint8_t _commandID;
    uint8_t _parity;
    uint8_t _checkedParity;

    boolean _inComingMessageComplete;
    char _inputChar[20];
    uint8_t _incomingCounter;

    void sendMessage(uint8_t& payload);
    void sendMessage(int16_t& payload);
    void setMessageType(uint8_t type);
    void setCommandID(uint8_t& commandID);
    void setSensorID(uint8_t& sensorID);
    void setNodeID(uint8_t& nodeID);
    void hexPrinting(uint8_t& data);
    void hexPrinting(int16_t& data);
    uint8_t hex_to_dec(uint8_t in);
    boolean parseSerialData();
    void printInfo();
    boolean checkParity();
    boolean newMessage;
    boolean validateMessageFields();
};
#endif
