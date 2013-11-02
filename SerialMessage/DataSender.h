// DataSender.h - Library for serial messaging
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

#ifndef DataSender_h
#define DataSender_h
#include <Arduino.h>

class DataSender : public Sender
{
  public:
    DataSender();

    void sendDataRequest(uint8_t sensorID, uint8_t payload);
    void sendData(uint8_t sensorID, uint8_t payload);
    void sendData(uint8_t sensorID, int16_t payload);

    void sendData(uint8_t payload);
    void sendData(int16_t payload);

    void sendDataArrayRequest(uint8_t arrayID, uint8_t length);
    void sendDataArray(uint8_t *dataArray, uint8_t length);

  private:
    uint8_t _sensorID;
    void setSensorID(uint8_t& sensorID);

};
#endif
