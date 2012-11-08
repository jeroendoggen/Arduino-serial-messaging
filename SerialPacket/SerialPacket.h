// SerialPacket.h - Library for sending serial data packets
// Copyright 2012 Jeroen Doggen (jeroendoggen@gmail.com)
// 
// Version History:
//   Version 0.1: Send single data sample in a packet
//   Version 0.2: Send arrays with multiple samples in a packet
//   Version 0.3: Conditional compilation: SERIAL_ASCII vs SERIAL_BINARY (includes.h)
//   Version 0.4: hexPrinting helper, changed packet types, commandID/packet
// Roadmap:
//   Version 0.5:  ??
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

#ifndef SerialPacket_h
#define SerialPacket_h
#include <Arduino.h>

class SerialPacket
{
  public:
	SerialPacket();

	void begin();
	void begin(long speed, uint8_t type, uint8_t nodeID, uint8_t sensorID, uint8_t commandID);
	
	void setPacketType(uint8_t type);
	void setNodeID(uint8_t nodeID);
	void setSensorID(uint8_t sensorID);
  void setCommandID(uint8_t commandID);
	
	void sendPacket(uint8_t payload);
	void sendPacket(uint8_t *dataArray, uint8_t length);

  private:
   uint8_t _packetType;
   uint8_t _nodeID;
   uint8_t _sensorID;
   uint8_t _commandID;
   uint8_t _parity;
   void hexPrinting(uint8_t data);
};
#endif
