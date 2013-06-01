""" sp_controller: serial communication

Serial communication class
  - Connect to the serial port
  - Allow a thread to be started to read data from the serial port

"""

from __future__ import print_function  # We require Python 2.6+

import sys
import threading
import time

try:
    import serial
except ImportError as exc:
    print("Error: failed to import pyserial module")
    print("Solution: you probably need to install the pyserial module")
    sys.exit(0)

from sp_controller.serialparser import SerialParser


class SerialCommunication(threading.Thread):
    """SerialCommunication: handling all serial communication in a thread"""
    def __init__(self,
                 identification,
                 serialport_settings,
                 logger,
                 sleeptime,
                 queue_settings):
        """Initialize the serial port object."""
        self.logger = logger
        self.sleeptime = sleeptime
        self.sensordata_queue = queue_settings.sensordata_queue
        self.cmd_reply_queue = queue_settings.cmd_reply_queue
        self.cmd_queue = queue_settings.cmd_queue
        self.identification = identification
        self.serial_connect(serialport_settings.serial_port,
                            serialport_settings.baudrate)
        self.parser = SerialParser()
        threading.Thread.__init__(self)

    def run(self):
        """This function will be called when the thread starts"""
        while True:
            time.sleep(self.sleeptime)
#            print(self.identification)
            if self.identification == "Packet_receiver":
                self.get_packet()
            if self.identification == "Command_transmitter":
                self.send_commands()

    def check_baudrate(self, baudrate):
        """TODO: implement this (now the program hangs on invalid baudrates)"""
        pass
#        if baudrate != 115200:
#            print("Baudrate error")
#            sys.exit()

    def serial_connect(self, serial_port, baudrate):
        """Connect to serial port"""
        try:
            self.check_baudrate(baudrate)
            self.ser = serial.Serial(serial_port, baudrate)
            self.ser.flush()
            self.logger.info(self.identification
                             + " : Connected to serial port: "
                             + serial_port
                             + " with speed:" + str(baudrate))
            print(self.identification
                  + " : connected to serial port: ", end="")
            print(serial_port, end="")
            print(" with speed: ", end="")
            print(baudrate)
        except:
            self.logger.critical("Unable to connect to serial port: "
                                 + serial_port
                                 + " with speed:"
                                 + str(baudrate))
            print("Unable to connect to serial port: ", end="")
            print(serial_port, end="")
            print(" with speed: ", end="")
            print(baudrate)
            sys.exit(1)

    def read_line(self):
        """Read one line of text over the serial port"""
        try:
            line = self.ser.readline()[:-1]
#            line = self.ser.readline().decode('utf-8')[:-1]
            return(line)
        except IOError:
            self.logger.critical("Lost serial connection")
            print ("Lost serial connection")
            #self.failed_test_list.append(current_test)
            sys.exit(1)

    def set_sleeptime(self, sleeptime):
        """Set the sleeptime for the thread"""
        self.sleeptime = sleeptime

    def get_packet(self):
        """Get one serial packet (one line of data)"""
        packet = self.read_line()[:-1]
        print("Incoming packet: " + packet)  # Print packet (HEX format)
        packet = self.parser.parse(packet)
        print(packet)  # Print out all info about the packet (human readable)
        print("")      # Print empty line
        if packet != 0:
            self.sensordata_queue.put(packet)

    def send_commands(self):
        """Send commands from the queue to the serial ports"""
        if not self.cmd_queue.empty():
            command = self.cmd_queue.get()
            #print(command)
            #self.cmd_queue.join
            command_id = command[0]
            payload = command[1]
            node_id = command[2]
            self.send_command(command_id, payload, node_id)
            
#            print(command)

    def send_old_command(self, value):
        """Send a command packet: compatible with command queue (deprecated: june 2013)"""
#        print(value)
        value = map(int, value)  # convert command elements in list to int
#        print(value)
        value = value[0]  # get first int from list
        print("Command in db: ", value)
        if (value < 256):
            #value = self.converter.db_to_serial(value)
            value = hex(value)  # convert to hex string
#            print(value)
            value = value[2:4]  # trow away 0x in front of hex string
#            print(value)
            parity = 0x01 ^ 0x00 ^ 0x12 ^ int(value, 16)  # calculate parity
            parity = hex(parity)  # convert to hex string
            parity = parity[2:4]  # throw away 0x in front of sting
##        parity = "EC"
            self.ser.write("T01N00I12P" + str(value) + "Q"
                           + str(parity) + "\r")
            print("Command for Arduino:   " + str(value), "HEX")

    def send_command(self, command_id, payload, node_id):
        """Send a command packet"""
        # Debug info:
        print("Command: ", self.hex_converter(command_id))
        print("Payload: ", self.hex_converter(payload))
        print("Node_id: ", self.hex_converter(node_id))
        
        # Calculate parity field:
        parity = 0x01 ^ node_id ^ command_id ^ payload
        parity = self.hex_converter(parity)
 
        # Convert to hex
        command_id = self.hex_converter(command_id)
        payload = self.hex_converter(payload)
        node_id = self.hex_converter(node_id)

        # Create packet
        packet = "T01" + "N" + str(node_id) + "I" + str(command_id) + "P" + str(payload) + "Q" + str(parity) + "\r" 
        print(packet)
        
        # Send packet
        self.ser.write(packet)
        print("")
        
    def hex_converter(self, value):
        # print("Input: ", end="")
        # print(value)
        value = hex(value)
        value = value[2:4]
        if (int(value,16) < 16):
            value = "0" + value
        # print("Output: ", end="")
        # print(value)
        return(value)
        
         
