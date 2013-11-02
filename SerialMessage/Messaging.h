// Messaging.h - Library for serial messaging
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

#ifndef Messaging_h
#define Messaging_h

// to be compatible with pre 1.0 Arduino code
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include <pins_arduino.h>
#endif

// needed when using abstract classes
#define __cxa_pure_virtual()

//abstract class Messaging
class Messaging
{
  public:
    void begin();
    void begin(long speed, uint8_t nodeID);

  private:
    void setNodeID(uint8_t& nodeID);

    struct message
    {
      uint8_t messageType;
      uint8_t nodeID;
      uint8_t payload;
      uint8_t parity;
    } Message;
};
#endif
