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

/// <summary>
/// Constructor
/// </summary>
SerialPacket::SerialPacket()
{
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
/// Send a single data value
/// </summary>
void SerialPacket::sendData(uint8_t sensorID, uint8_t payload)
{
  setPacketType(DATA);
  setSensorID(sensorID);
  sendPacket(payload);
}

/// <summary>
/// Send a single data value, reuses sensorID from previous packets
/// </summary>
void SerialPacket::sendData(uint8_t payload)
{
  setPacketType(DATA);
  sendPacket(payload);
}

/// <summary>
/// Send out the actual data packet (called from other 'send' functions)
/// </summary>
void SerialPacket::sendPacket(uint8_t& payload)
{
  _parity=_packetType^_nodeID^_sensorID^payload;
  Serial.print("T");
  hexPrinting(_packetType);
  Serial.print("N");
  hexPrinting(_nodeID);
  if (_packetType == COMMAND | _packetType == COMMAND_REPLY)
  {
    Serial.print("C");
    hexPrinting(_commandID);
  }
  else if (_packetType == DATA | _packetType == DATA_REQUEST)
  {
    Serial.print("S");
    hexPrinting(_sensorID);
  }
  Serial.print("D");
  hexPrinting(payload);
  Serial.print("P");
  hexPrinting(_parity);
  Serial.println("");
}

/// <summary>
/// Send multiple data samples in one packet by passing an array and its length
/// </summary>
void SerialPacket::sendDataArray(uint8_t *dataArray, uint8_t length)
{
  setSensorID(length);                            //sensorID contains the length of the data array (can be used at receiving side)
  _parity=_packetType^_nodeID^_sensorID;
  Serial.print("T");
  hexPrinting(_packetType);
  Serial.print("N");
  hexPrinting(_nodeID);
  Serial.print("S");
  hexPrinting(_sensorID);
  Serial.print("D");

  for(int i=0 ; i<length ; i++)
  {
    hexPrinting(dataArray[i]);
    _parity=_parity^dataArray[i];
  }
  Serial.print("P");                              // does this work ok with arrays? not validated by hand
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
