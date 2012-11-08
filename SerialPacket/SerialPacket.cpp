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
/// Begin variables: default values:
///  - nodeID: 0
///  - sensorID: 0
///  - packetType: 0
/// </summary>
void SerialPacket::begin()
{
  begin (115200,0,0,0,0);
}

/// <summary>
/// Begin using custom settings
/// </summary>
void SerialPacket::begin(long speed, uint8_t type, uint8_t nodeID, uint8_t sensorID, uint8_t commandID)
{
  Serial.begin(speed);
  setPacketType(type);
  setNodeID(nodeID);
  setSensorID(sensorID);
  setCommandID(commandID);
}

/// <summary>
/// Set nodeID
/// </summary>
void SerialPacket::setNodeID(uint8_t nodeID)
{
  _nodeID=nodeID;
}

/// <summary>
/// Set sensorID
/// </summary>
void SerialPacket::setSensorID(uint8_t sensorID)
{
  _sensorID=sensorID;
}

/// <summary>
/// Set commandID
/// </summary>
void SerialPacket::setCommandID(uint8_t commandID)
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
void SerialPacket::hexPrinting(uint8_t data)
{
  if(data<16){
    Serial.print(0, HEX); 
  }
  Serial.print(data, HEX);
}

/// <summary>
/// Send a single sample in a packet
/// </summary>
void SerialPacket::sendPacket(uint8_t payload)
{

#ifdef SERIAL_ASCII
  switch (_packetType)
  {
    case DATAPACKET:
    case DATAREQUESTPACKET:
      _parity=_packetType^_nodeID^_sensorID^payload;
      Serial.print("Type:");
      hexPrinting(_packetType);
      Serial.print(" NodeID:");
      hexPrinting(_nodeID);
      Serial.print(" SensorID:");
      hexPrinting(_sensorID);
      Serial.print(" Data:");
      hexPrinting(payload);   //sensor data
      Serial.print(" Parity:");
      hexPrinting(_parity);
      Serial.println("");
      break;
    case COMMANDPACKET:  // currently the same code is executed for both options
    case COMMANDREPLY:
      _parity=_packetType^_nodeID^_commandID^payload;
      Serial.print("Type:");
      hexPrinting(_packetType);
      Serial.print(" NodeID:");
      hexPrinting(_nodeID);
      Serial.print(" CommandID:");
      hexPrinting(_commandID);                       //command
      Serial.print(" Data:");
      hexPrinting(payload);   //data
      Serial.print(" Parity:");
      hexPrinting(_parity);
      Serial.println("");
      break;
  }
#endif
#ifdef SERIAL_BINARY
  switch (_packetType)
  {
    case DATAPACKET:
      _parity=_packetType^_nodeID^_sensorID^payload;
      Serial.write(_packetType);
      Serial.write(_nodeID);
      Serial.write(_sensorID);
      Serial.write(payload);                       //data
      Serial.write(_parity);
      break;
    case COMMANDPACKET:  // currently the same code is executed for both options
    case COMMANDREPLY:
      _parity=_packetType^_nodeID^_commandID^payload;
      Serial.write(_packetType);
      Serial.write(_nodeID);
      Serial.write(_commandID);      
      Serial.write(payload);                       //command
      Serial.write(_parity);
      break;
  }
#endif
}

/// <summary>
/// Send multiple data samples in one packet by passing an array and its length
/// </summary>
void SerialPacket::sendPacket(uint8_t *dataArray, uint8_t length)
{
#ifdef SERIAL_ASCII
  switch (_packetType)
  {
    case AGGREGATEDDATA:
      setSensorID(length);
      _parity=_packetType^_nodeID^_sensorID;
      Serial.print("Type:");
      hexPrinting(_packetType);
      Serial.print(" NodeID:");
      hexPrinting(_nodeID);
      Serial.print(" SensorID:");
      hexPrinting(_sensorID);
      Serial.print(" Data:");
      for(int i=0 ; i<length ; i++)
      {
        hexPrinting(dataArray[i]);
        _parity=_parity^dataArray[i];
      }
      Serial.print(" Parity:");                   // does this work ok with arrays? not validated by hand
      hexPrinting(_parity);
      Serial.println("");
      break;
    default:
      Serial.print("ERROR: don't print arrays in command mode!");
  }
#endif
#ifdef SERIAL_BINARY
  switch (_packetType)
  {
    case AGGREGATEDDATA:
      setSensorID(length);
      _parity=_packetType^_nodeID^_sensorID;
      Serial.write(_packetType);
      Serial.write(_nodeID);
      Serial.write(_sensorID);
      for(int i=0 ; i<length ; i++)
      {
        Serial.write(dataArray[i]);
        _parity=_parity^dataArray[i];
      }
      Serial.write(_parity);
      break;
  }
#endif
}
