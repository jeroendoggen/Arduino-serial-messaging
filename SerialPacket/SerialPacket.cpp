// SerialPacket.cpp - Library for sending serial data packets
// Copyright 2012 Jeroen Doggen (jeroendoggen@gmail.com)
//  For more information: variable declaration, changelog,... see SerialPacket.h
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

/// <summary>
/// SerialPacket.cpp - Library for sending sensor data packets over UART.
/// For more information: variable declaration, changelog,... see SerialPacket.h
/// </summary>

#include <Arduino.h>
#include "SerialPacket.h"
#include "defines.h"

// struct _packet
// {
//   uint8_t _packetType;
//   uint8_t _nodeID;
//   uint8_t _sensorID;
//   uint8_t _commandID;
//   uint8_t _parity;
// };
// 
// _packet incomingPacket = { 0 , 0, 0, 0, 0};

/// <summary>
/// Constructor
/// </summary>
SerialPacket::SerialPacket() : _inputChar() 
{
  _inComingPacketComplete = false;
  _incomingCounter=0;
//   _inputChar[0]= 0;

}

/// <summary>
/// Begin using default settings:
///  - speed: 115200 baud
///  - nodeID: 0
/// </summary>
void SerialPacket::begin()
{
  begin (115200,0);                               //TODO put baudrates in defines.h
}

/// <summary>
/// Begin using custom settings
/// </summary>
void SerialPacket::begin(long speed, uint8_t nodeID)
{
  Serial.begin(speed);
  setNodeID(nodeID);
//   incomingPacket.commandID = 2;
//   outgoingPacket.commandID = 3;
}

/// <summary>
/// Send a single command
/// </summary>
void SerialPacket::sendCommand(uint8_t commandID, uint8_t payload)
{
  setPacketType(COMMAND);
  setCommandID(commandID);
  sendPacket(payload);
}

/// <summary>
/// Send a single command, reuses commandID from previous packets
/// </summary>
void SerialPacket::sendCommand(uint8_t payload)
{
  setPacketType(COMMAND);
  sendPacket(payload);
}

/// <summary>
/// Send a reply to a command
/// </summary>
void SerialPacket::sendCommandReply(uint8_t commandID, uint8_t payload)
{
  setPacketType(COMMAND_REPLY);
  setCommandID(commandID);
  sendPacket(payload);
}

/// <summary>
/// Request a single data value
/// </summary>
void SerialPacket::sendDataRequest(uint8_t sensorID, uint8_t payload)
{
  setPacketType(DATA_REQUEST);
  setSensorID(sensorID);
  sendPacket(payload);
}

/// <summary>
/// Request an array of data values
/// </summary>
void SerialPacket::sendDataArrayRequest(uint8_t arrayID, uint8_t payload)
{
  setPacketType(DATA_ARRAY_REQUEST);
  setSensorID(arrayID);
  sendPacket(payload);
}

/// <summary>
/// Send a single data value
/// </summary>T12N00I00P00Q12
void SerialPacket::sendData(uint8_t sensorID, uint8_t payload)
{
  setPacketType(DATA_BYTE);
  setSensorID(sensorID);
  sendPacket(payload);
}

/// <summary>
/// Send a single data value
/// </summary>
void SerialPacket::sendData(uint8_t sensorID, int16_t payload)
{
  setPacketType(DATA_INT);
  setSensorID(sensorID);
  sendPacket(payload);
}

/// <summary>
/// Send a single 8-bit data value (Arduino 'byte' type), reuses sensorID from previous packets
/// </summary>
void SerialPacket::sendData(uint8_t payload)
{
  setPacketType(DATA_BYTE);
  sendPacket(payload);
}

/// <summary>
/// Send a single 16-bit data value (Arduino 'int' type), reuses sensorID from previous packets
/// </summary>
void SerialPacket::sendData(int16_t payload)
{
  setPacketType(DATA_INT);
  sendPacket(payload);
}

/// <summary>
/// Send out the actual 8-bit data packet (called from other 'send' functions)
/// </summary>
void SerialPacket::sendPacket(uint8_t& payload)
{
  _parity=_packetType^_nodeID^_sensorID^payload;
  Serial.print("T");
  hexPrinting(_packetType);
  Serial.print("N");
  hexPrinting(_nodeID);
  Serial.print("I");
  if ((_packetType == COMMAND) | (_packetType == COMMAND_REPLY))
  {
    hexPrinting(_commandID);
  }
  else if ((_packetType == DATA_ARRAY_REQUEST) | (_packetType == DATA_BYTE) | (_packetType == DATA_REQUEST))
  {
    hexPrinting(_sensorID);
  }
  Serial.print("P");
  hexPrinting(payload);
  Serial.print("Q");
  hexPrinting(_parity);
  Serial.println("");
}

/// <summary>
/// Send out the actual 16-bit data packet (called from other 'send' functions)
/// </summary>
void SerialPacket::sendPacket(int16_t& payload)
{
  _parity=_packetType^_nodeID^_sensorID^payload;
  Serial.print("T");
  hexPrinting(_packetType);
  Serial.print("N");
  hexPrinting(_nodeID);
  Serial.print("I");
  if ((_packetType == COMMAND) | (_packetType == COMMAND_REPLY))
  {
    hexPrinting(_commandID);
  }
  else if ((_packetType == DATA_ARRAY_REQUEST) | (_packetType == DATA_INT) | (_packetType == DATA_REQUEST))  //_packetType == DATA_ARRAY_REQUEST (not yet implemented as a separate function)
  {
    hexPrinting(_sensorID);
  }
  Serial.print("P");
  hexPrinting(payload);
  Serial.print("Q");
  hexPrinting(_parity);
  Serial.println("");
}

/// <summary>
/// Send multiple data samples in one packet by passing an array and its length
/// </summary>T12N00I00P1CQ0E
void SerialPacket::sendDataArray(uint8_t *dataArray, uint8_t length)
{
  setSensorID(length);                            //sensorID contains the length of the data array (can be used at receiving side)
  _parity=_packetType^_nodeID^_sensorID;
  Serial.print("T");
  hexPrinting(_packetType);
  Serial.print("N");
  hexPrinting(_nodeID);
  Serial.print("I");
  hexPrinting(_sensorID);
  Serial.print("P");

  for(int i=0 ; i<length ; i++)
  {
    hexPrinting(dataArray[i]);
    _parity=_parity^dataArray[i];
  }
  Serial.print("Q");                              // does this work ok with arrays? not validated by hand
  hexPrinting(_parity);
  Serial.println("");
}

/// <summary>
/// Set commandID
/// </summary>
void SerialPacket::setCommandID(uint8_t& commandID)
{
  _commandID=commandID;
}

/// <summary>
/// Set packet type
/// </summary>
void SerialPacket::setPacketType(uint8_t type)
{
  _packetType=type;
}

/// <summary>
/// HexPrinting: helper function to print data with a constant field width (2 hex values)
/// </summary>
void SerialPacket::hexPrinting(uint8_t& data)
{
  if(data<16)
  {
    Serial.print(0, HEX);
  }
  Serial.print(data, HEX);
}

/// <summary>
/// HexPrinting: helper function to print data with a constant field width (2 hex values)
/// </summary>
void SerialPacket::hexPrinting(int16_t& data)
{
  if(data<4096 && data>0)
  {
    Serial.print(0, HEX);
  }
  if(data<256 && data>0)
  {
    Serial.print(0, HEX);
  }
  if(data<16 && data>0)
  {
    Serial.print(0, HEX);
  }
  Serial.print(uint16_t(data), HEX);  //casting to suppress FFFF for negative int values
}

/// <summary>
/// Set nodeID
/// </summary>
void SerialPacket::setNodeID(uint8_t& nodeID)
{
  _nodeID=nodeID;
}

/// <summary>
/// Set sensorID
/// </summary>
void SerialPacket::setSensorID(uint8_t& sensorID)
{
  _sensorID=sensorID;
}

/// <summary>
/// Set readSerialData
/// </summary>
void SerialPacket::readSerialData()
{
  char inChar;
  while (Serial.available() && inChar != '\n' ) {
    _inputChar[_incomingCounter]=(char)Serial.read();
    _incomingCounter++;
  }
  
  //Flush buffer
  while (Serial.read() >= 0){} 

  parseSerialData();
  printInfo();
  _incomingCounter=0;
}


/// <summary>
/// Set parseSerialData
/// </summary>
void SerialPacket::parseSerialData()
{
  incomingPacket.packetType = hex_to_dec(_inputChar[1])*16 + hex_to_dec(_inputChar[2]);
  incomingPacket.nodeID = hex_to_dec(_inputChar[4])*16 + hex_to_dec(_inputChar[5]);
  incomingPacket.payload = hex_to_dec(_inputChar[10])*16 + hex_to_dec(_inputChar[11]);
  if ((incomingPacket.packetType == COMMAND) | (incomingPacket.packetType == COMMAND_REPLY))
  {
    incomingPacket.sensorID = hex_to_dec(_inputChar[7])*16 + hex_to_dec(_inputChar[8]);
    _checkedParity = incomingPacket.packetType^incomingPacket.nodeID^incomingPacket.sensorID^incomingPacket.payload; 
  }
  else if ((incomingPacket.packetType == DATA_INT) | (incomingPacket.packetType == DATA_BYTE) | (incomingPacket.packetType == DATA_REQUEST))
  {
    incomingPacket.commandID = hex_to_dec(_inputChar[7])*16 + hex_to_dec(_inputChar[8]);;
    _checkedParity = incomingPacket.packetType^incomingPacket.nodeID^incomingPacket.commandID^incomingPacket.payload;
  }
  incomingPacket.parity = hex_to_dec(_inputChar[13])*16 + hex_to_dec(_inputChar[14]);  
}

/// <summary>
/// Convert HEX to Decimal
/// </summary>
#define HEX_DEC_ERROR 42 
uint8_t SerialPacket::hex_to_dec(uint8_t in) 
{ 
   if(((in >= '0') && (in <= '9'))) return in-'0'; 
   in |= 0x20; 
   if(((in >= 'a') && (in <= 'f'))) return in-'a' + 10; 
   return HEX_DEC_ERROR; 
}


/// <summary>
/// printInfo: 
/// </summary>
void SerialPacket::printInfo() 
{
//     for(int i=0;i<_incomingCounter;i++)
//     {
//       Serial.print(_inputChar[i]);
//     }
    Serial.print("Type:     ");
    Serial.println(incomingPacket.packetType,HEX);
    Serial.print("NodeID:   ");
    Serial.println(incomingPacket.nodeID,HEX);
    Serial.print("SensorID: ");
    Serial.println(incomingPacket.sensorID,HEX);
    Serial.print("CommandID ");
    Serial.println(incomingPacket.commandID,HEX);
    Serial.print("Payload:  ");
    Serial.println(incomingPacket.payload,HEX);
    Serial.print("Parity:   ");
    Serial.println(incomingPacket.parity,HEX);
    Serial.println("------------");
    Serial.print("Checked parity:   ");    
    Serial.println(_checkedParity,HEX);
    Serial.println("------------");
}


/// <summary>
/// Convert HEX to DecimalT12N00I00P1CQ0E
/// </summary>
boolean SerialPacket::checkParity()
{
    if(incomingPacket.parity == _checkedParity)
    {
      return(true);
    }
    else 
    {
      return(false);
    }
}

/// <summary>
/// Get commandID
/// </summary>
uint8_t SerialPacket::getCommandID()
{
    return incomingPacket.commandID;
}

/// <summary>
/// Get commandID
/// </summary>
uint8_t SerialPacket::getPayload()
{
    return incomingPacket.payload;
}


