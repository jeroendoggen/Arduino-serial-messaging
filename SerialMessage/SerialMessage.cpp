// SerialMessage.cpp - Library for sending serial data messages
// Copyright 2012 Jeroen Doggen (jeroendoggen@gmail.com)
//  For more information: variable declaration, changelog,... see SerialMessage.h
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
/// SerialMessage.cpp - Library for sending sensor data messages over UART.
/// For more information: variable declaration, changelog,... see SerialMessage.h
/// </summary>

#include <Arduino.h>
#include "SerialMessage.h"
#include "defines.h"

/// <summary>
/// Constructor
/// </summary>
SerialMessage::SerialMessage() : _inputChar()
{
  _inComingMessageComplete = false;
  _incomingCounter=0;
  incomingMessage.messageType=0;
  incomingMessage.nodeID=0;
  incomingMessage.sensorID=0;
  incomingMessage.commandID=0;
  incomingMessage.payload=0;
  incomingMessage.parity=0;
  newMessage = false;
}

/// <summary>
/// Begin using default settings:
///  - speed: 115200 baud
///  - nodeID: 0
/// </summary>
void SerialMessage::begin()
{
  begin (DEFAULT_BAUDRATE,0);
}

/// <summary>
/// Begin using custom settings
/// </summary>
void SerialMessage::begin(long speed, uint8_t nodeID)
{
  Serial.begin(speed);
  setNodeID(nodeID);
}

/// <summary>
/// Send a single command
/// </summary>
void SerialMessage::sendCommand(uint8_t commandID, uint8_t payload)
{
  setMessageType(COMMAND);
  setCommandID(commandID);
  sendMessage(payload);
}

/// <summary>
/// Send a single command, reuses commandID from previous messages
/// </summary>
void SerialMessage::sendCommand(uint8_t payload)
{
  setMessageType(COMMAND);
  sendMessage(payload);
}

/// <summary>
/// Send a reply to a command
/// </summary>
void SerialMessage::sendCommandReply(uint8_t commandID, uint8_t payload)
{
  setMessageType(COMMAND_REPLY);
  setCommandID(commandID);
  sendMessage(payload);
}

/// <summary>
/// Request a single data value
/// </summary>
void SerialMessage::sendDataRequest(uint8_t sensorID, uint8_t payload)
{
  setMessageType(DATA_REQUEST);
  setSensorID(sensorID);
  sendMessage(payload);
}

/// <summary>
/// Request an array of data values
/// </summary>
void SerialMessage::sendDataArrayRequest(uint8_t arrayID, uint8_t payload)
{
  setMessageType(DATA_ARRAY_REQUEST);
  setSensorID(arrayID);
  sendMessage(payload);
}

/// <summary>
/// Send a single data value
/// </summary>
void SerialMessage::sendData(uint8_t sensorID, uint8_t payload)
{
  setMessageType(DATA_BYTE);
  setSensorID(sensorID);
  sendMessage(payload);
}

/// <summary>
/// Send a single data value
/// </summary>
void SerialMessage::sendData(uint8_t sensorID, int16_t payload)
{
  setMessageType(DATA_INT);
  setSensorID(sensorID);
  sendMessage(payload);
}

/// <summary>
/// Send a single 8-bit data value (Arduino 'byte' type), reuses sensorID from previous messages
/// </summary>
void SerialMessage::sendData(uint8_t payload)
{
  setMessageType(DATA_BYTE);
  sendMessage(payload);
}

/// <summary>
/// Send a single 16-bit data value (Arduino 'int' type), reuses sensorID from previous messages
/// </summary>
void SerialMessage::sendData(int16_t payload)
{
  setMessageType(DATA_INT);
  sendMessage(payload);
}

/// <summary>
/// Send out the actual 8-bit data message (called from other 'send' functions)
/// </summary>
void SerialMessage::sendMessage(uint8_t& payload)
{
  Serial.print("T");
  hexPrinting(_messageType);
  Serial.print("N");
  hexPrinting(_nodeID);
  Serial.print("I");
  if ((_messageType == COMMAND) | (_messageType == COMMAND_REPLY)) {
    hexPrinting(_commandID);
    _parity=_messageType^_nodeID^_commandID^payload;
  }
  else if ((_messageType == DATA_ARRAY_REQUEST) | (_messageType == DATA_BYTE) | (_messageType == DATA_REQUEST)) {
    hexPrinting(_sensorID);
    _parity=_messageType^_nodeID^_sensorID^payload;
  }
  Serial.print("P");
  hexPrinting(payload);
  Serial.print("Q");
  hexPrinting(_parity);
  Serial.println("");
}

/// <summary>
/// Send out the actual 16-bit data message (called from other 'send' functions)
/// </summary>
void SerialMessage::sendMessage(int16_t& payload)
{
  _parity=_messageType^_nodeID^_sensorID^payload;
  Serial.print("T");
  hexPrinting(_messageType);
  Serial.print("N");
  hexPrinting(_nodeID);
  Serial.print("I");
  if ((_messageType == COMMAND) | (_messageType == COMMAND_REPLY)) {
    hexPrinting(_commandID);
  }
                                                  //_messageType == DATA_ARRAY_REQUEST (not yet implemented as a separate function)
  else if ((_messageType == DATA_ARRAY_REQUEST) | (_messageType == DATA_INT) | (_messageType == DATA_REQUEST)) {
    hexPrinting(_sensorID);
  }
  Serial.print("P");
  hexPrinting(payload);
  Serial.print("Q");
  hexPrinting(_parity);
  Serial.println("");
}

/// <summary>
/// Send multiple data samples in one message by passing an array and its length
/// </summary>
void SerialMessage::sendDataArray(uint8_t *dataArray, uint8_t length)
{
  setSensorID(length);                            //sensorID contains the length of the data array (can be used at receiving side)
  _parity=_messageType^_nodeID^_sensorID;
  Serial.print("T");
  hexPrinting(_messageType);
  Serial.print("N");
  hexPrinting(_nodeID);
  Serial.print("I");
  hexPrinting(_sensorID);
  Serial.print("P");

  for(int i=0 ; i<length ; i++) {
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
void SerialMessage::setCommandID(uint8_t& commandID)
{
  _commandID=commandID;
}

/// <summary>
/// Set message type
/// </summary>
void SerialMessage::setMessageType(uint8_t type)
{
  _messageType=type;
}

/// <summary>
/// HexPrinting: helper function to print data with a constant field width (2 hex values)
/// </summary>
void SerialMessage::hexPrinting(uint8_t& data)
{
  if(data<16) {
    Serial.print(0, HEX);
  }
  Serial.print(data, HEX);
}

/// <summary>
/// HexPrinting: helper function to print data with a constant field width (2 hex values)
/// </summary>
void SerialMessage::hexPrinting(int16_t& data)
{
  if(data<4096 && data>0) {
    Serial.print(0, HEX);
  }
  if(data<256 && data>0) {
    Serial.print(0, HEX);
  }
  if(data<16 && data>0) {
    Serial.print(0, HEX);
  }
  Serial.print(uint16_t(data), HEX);              //casting to suppress FFFF for negative int values
}

/// <summary>
/// Set nodeID
/// </summary>
void SerialMessage::setNodeID(uint8_t& nodeID)
{
  _nodeID=nodeID;
}

/// <summary>
/// Set sensorID
/// </summary>
void SerialMessage::setSensorID(uint8_t& sensorID)
{
  _sensorID=sensorID;
}

/// <summary>
/// Set readSerialData
/// </summary>
boolean SerialMessage::readSerialData()
{
  while (Serial.available() && _inputChar[_incomingCounter] != '\n' ) {
    _inputChar[_incomingCounter]=(char)Serial.read();
    _incomingCounter++;
    // This checks for a minimum lenght (longer is also ok.. problem?)
    if (_incomingCounter == 16) {
      newMessage = true;
    }
  }

  //Flush buffer
  while (Serial.read() >= 0){}

  //   parseSerialData();
  // Does not work when parsing is called in the return statement
  _incomingCounter=0;
  return (parseSerialData());
}

/// <summary>
/// Set parseSerialData
/// </summary>
/// @todo: error handling: illegal payloads is now handled by char position (G2 -> 02, 2G -> 20, GG -> 00), requires hextodec reimplementation (when HEX_DEC_ERROR=0)
boolean SerialMessage::parseSerialData()
{
  incomingMessage.messageType = hex_to_dec(_inputChar[1])*16 + hex_to_dec(_inputChar[2]);
  
  incomingMessage.nodeID = hex_to_dec(_inputChar[4])*16 + hex_to_dec(_inputChar[5]);
  incomingMessage.payload = hex_to_dec(_inputChar[10])*16 + hex_to_dec(_inputChar[11]);
  // casting error hex vs decimal (in if)
  if ((incomingMessage.messageType == COMMAND) | (incomingMessage.messageType == COMMAND_REPLY)) {
    //Serial.println("parseSerialData");
    incomingMessage.commandID = hex_to_dec(_inputChar[7])*16 + hex_to_dec(_inputChar[8]);
    //Serial.println(incomingMessage.commandID);
    _checkedParity = incomingMessage.messageType^incomingMessage.nodeID^incomingMessage.commandID^incomingMessage.payload;
  }
  else if ((incomingMessage.messageType == DATA_INT) | (incomingMessage.messageType == DATA_BYTE) | (incomingMessage.messageType == DATA_REQUEST)) {
    incomingMessage.sensorID = hex_to_dec(_inputChar[7])*16 + hex_to_dec(_inputChar[8]);;
    _checkedParity = incomingMessage.messageType^incomingMessage.nodeID^incomingMessage.sensorID^incomingMessage.payload;
  }
  incomingMessage.parity = hex_to_dec(_inputChar[13])*16 + hex_to_dec(_inputChar[14]);
 
  #ifdef DEBUG_SERIAL
  printInfo();
  #endif
  
  if (newMessage) {
    newMessage=false;
    return checkParity();
  }
  else {
    return false;
  }
}

/// <summary>
/// Check if all the field in the message have acceptable value
/// @TODO: implement this
/// </summary>
boolean SerialMessage::validateMessageFields()
{
  if (true){
    return true;
  }
  else{
    return false;
  }
}


/// <summary>
/// Convert HEX to Decimal
/// </summary>
#define HEX_DEC_ERROR 0
uint8_t SerialMessage::hex_to_dec(uint8_t in)
{
  if(((in >= '0') && (in <= '9'))) return in-'0';
  in |= 0x20;
  if(((in >= 'a') && (in <= 'f'))) return in-'a' + 10;
  return HEX_DEC_ERROR;
}

/// <summary>
/// printInfo:
/// </summary>
void SerialMessage::printInfo()
{
  Serial.print("Type:     ");
  Serial.println(incomingMessage.messageType,HEX);
  Serial.print("NodeID:   ");
  Serial.println(incomingMessage.nodeID,HEX);
  Serial.print("SensorID: ");
  Serial.println(incomingMessage.sensorID,HEX);
  Serial.print("CommandID ");
  Serial.println(incomingMessage.commandID,HEX);
  Serial.print("Payload:  ");
  Serial.println(incomingMessage.payload,HEX);
  Serial.print("Parity:   ");
  Serial.println(incomingMessage.parity,HEX);
  Serial.println("------------");
  Serial.print("Checked parity:   ");
  Serial.println(_checkedParity,HEX);
  Serial.println("------------");
}

/// <summary>
/// Check parity
/// </summary>
boolean SerialMessage::checkParity()
{
  if (_checkedParity == incomingMessage.parity){
    #ifdef DEBUG_SERIAL
    Serial.println("Parity ok");
    Serial.println("----------------");
    #endif
    return true;
  }
  else{
    #ifdef DEBUG_SERIAL
    Serial.println("Parity wrong");
    #endif
    return false;
  }
}

/// <summary>
/// Get commandID
/// </summary>
uint8_t SerialMessage::getCommandID()
{
  return incomingMessage.commandID;
}

/// <summary>
/// Get getPayload
/// </summary>
uint8_t SerialMessage::getPayload()
{
  return incomingMessage.payload;
}