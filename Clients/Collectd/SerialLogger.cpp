// SerialLogger.cpp -  Read 'serialPacket' data and write logfiles for collectd
// Copyright 2012 Jeroen Doggen (jeroendoggen@gmail.com)
// More info: 
//
// Version History:
//  Version 0.0.1: Just a first quick and dirty first implementation !!
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


// TODO list:
/// @todo:

#include <iostream>
#include <SerialStream.h>
#include <cstdlib>
#include <iostream>
#include <fstream>

#define TEMPERATURE 0x10
#define HUMIDITY    0x11
#define DISTANCE    0x30

int hex_to_dec(int in);

int main( int argc,  char** argv  )
{
    //
    // Open the serial port.
    //
    using namespace LibSerial ;
    using namespace std;
    SerialStream serial_port ;
    serial_port.Open( "/dev/ttyUSB0" ) ;
    if ( ! serial_port.good() ) 
    {
        std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                  << "Error: Could not open serial port." 
                  << std::endl ;
        exit(1) ;
    }
    //
    // Set the baud rate of the serial port.
    //
    serial_port.SetBaudRate( SerialStreamBuf::BAUD_57600 ) ;
    if ( ! serial_port.good() ) 
    {
        std::cerr << "Error: Could not set the baud rate." << std::endl ;
        exit(1) ;
    }
    //
    // Set the number of data bits.
    //
    serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
    if ( ! serial_port.good() ) 
    {
        std::cerr << "Error: Could not set the character size." << std::endl ;
        exit(1) ;
    }
    //
    // Disable parity.
    //
    serial_port.SetParity( SerialStreamBuf::PARITY_NONE ) ;
    if ( ! serial_port.good() ) 
    {
        std::cerr << "Error: Could not disable the parity." << std::endl ;
        exit(1) ;
    }
    //
    // Set the number of stop bits.
    //
    serial_port.SetNumOfStopBits( 1 ) ;
    if ( ! serial_port.good() ) 
    {
        std::cerr << "Error: Could not set the number of stop bits."
                  << std::endl ;
        exit(1) ;
    }
    //
    // Turn on hardware flow control.
    //
    serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_HARD ) ;
    if ( ! serial_port.good() ) 
    {
        std::cerr << "Error: Could not use hardware flow control."
                  << std::endl ;
        exit(1) ;
    }
    //
    // Do not skip whitespace characters while reading from the
    // serial port.
    //
    serial_port.unsetf( std::ios_base::skipws ) ;
    //
    // Keep reading data from serial port and print it to the screen.
    //
    
    ofstream distance,temperature,humidity;
    
    char next_byte1;
    char next_byte2;
    char next_byte;
        
    int packetType;
    int nodeID;
    int sensorID;
    int parity;
    int payload;
    
    //T12N00I00P01Q13
    while( true  ) 
    {
        serial_port.get(next_byte);
        if((char)next_byte == 'T')
        {
          serial_port.get(next_byte1);
          serial_port.get(next_byte2);
          packetType=hex_to_dec(next_byte1)*16 + hex_to_dec(next_byte2);
          if (packetType == 0x12)
          {
            serial_port.get(next_byte);
            if((char)next_byte == 'N')
            {
              serial_port.get(next_byte1);
              serial_port.get(next_byte2);
              nodeID=hex_to_dec(next_byte1)*16 + hex_to_dec(next_byte2);
            }
            
            serial_port.get(next_byte);
            if((char)next_byte == 'I')
            {
              serial_port.get(next_byte1);
              serial_port.get(next_byte2);
              sensorID=hex_to_dec(next_byte1)*16 + hex_to_dec(next_byte2);
            }
            serial_port.get(next_byte);
            
            serial_port.get(next_byte1);
            serial_port.get(next_byte2);
            payload=hex_to_dec(next_byte1)*16 + hex_to_dec(next_byte2);
            
            if((char)next_byte == 'P')
            {
              if(sensorID==TEMPERATURE)
              {
                distance.open ("arduino_distance.log");
                distance << payload << "\n";
                distance.close();
              }
                  
              else if(sensorID==HUMIDITY)
              {
                humidity.open ("arduino_humidity.log");
                humidity << payload << "\n";
                humidity.close();
              }
              
              else if(sensorID==DISTANCE)
              {
                temperature.open ("arduino_temperature.log");
                temperature << payload << "\n";
                temperature.close();
              }  
            }
          } 
        }  
     } 
     std::cerr << std::endl ;
     return EXIT_SUCCESS ;
}

/// Convert HEX to Decimal
#define HEX_DEC_ERROR 42 
int hex_to_dec(int in) 
{ 
   if(((in >= '0') && (in <= '9'))) return in-'0'; 
   in |= 0x20; 
   if(((in >= 'a') && (in <= 'f'))) return in-'a' + 10; 
   return HEX_DEC_ERROR; 
}

