// SerialReceiver.h - Library for sending serial data messages
// Copyright 2012 Jeroen Doggen (jeroendoggen@gmail.com)
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

#ifndef SerialCommunication_h
#define SerialCommunication_h

// to be compatible with pre 1.0 Arduino code
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include <pins_arduino.h>
#endif

// needed when using abstract classes
#define __cxa_pure_virtual()

//abstract class SerialReceiver
class SerialReceiver : public SerialCommunication
{
   public:
//     void begin();
//     void begin(long speed, uint8_t nodeID);

//     void sendCommand(uint8_t commandID, uint8_t payload);
//     void sendCommand(uint8_t payload);
//     void sendCommandReply(uint8_t commandID, uint8_t payload);
// 
//     void sendDataRequest(uint8_t sensorID, uint8_t payload);
//     void sendData(uint8_t sensorID, uint8_t payload);
//     void sendData(uint8_t sensorID, int16_t payload);
// 
//     void sendData(uint8_t payload);
//     void sendData(int16_t payload);

//     void sendDataArrayRequest(uint8_t arrayID, uint8_t length);
//     void sendDataArray(uint8_t *dataArray, uint8_t length);

    boolean readSerialData();
    uint8_t getCommandID();
    uint8_t getPayload();

  private:
//     struct message
//     {
//       uint8_t messageType;
//       uint8_t nodeID;
//       uint8_t sensorID;
//       uint8_t commandID;
//       uint8_t payload;
//       uint8_t parity;
//     } incomingMessage, outgoingMessage;             //TODO: also use this struct to send messages? (todo underscore)

//     uint8_t _messageType;
//     uint8_t _nodeID;
//     uint8_t _sensorID;
//     uint8_t _commandID;
//     uint8_t _parity;
    uint8_t _checkedParity;

    boolean _inComingMessageComplete;
    char _inputChar[20];
    uint8_t _incomingCounter;

//     void sendMessage(uint8_t& payload);
//     void sendMessage(int16_t& payload);
//     void setMessageType(uint8_t type);
//     void setCommandID(uint8_t& commandID);
//     void setSensorID(uint8_t& sensorID);
//     void setNodeID(uint8_t& nodeID);
//     void hexPrinting(uint8_t& data);
//     void hexPrinting(int16_t& data);
//     uint8_t hex_to_dec(uint8_t in);
    boolean parseSerialData();
    void printInfo();
    boolean checkParity();
    boolean newMessage;
    boolean validateMessageFields();
};
#endif
