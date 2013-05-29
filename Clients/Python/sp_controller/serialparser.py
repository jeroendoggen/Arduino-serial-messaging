""" sp_controller: serial parser

Serial parser class

"""

from __future__ import print_function  # We require Python 2.6+
import datetime


class SerialParser():
    """SerialParser: parser for SerialPacket data"""
    def __init__(self):
        pass
        #self.converter = DataConverter()
    
    def parse(self, packet):
        """Parse a packet"""
        self.now = datetime.datetime.now()
        if self.validate_packet(packet):
            if self.get_packet_type(packet) == 0x01:  # 
                return(self.parse_data(packet))
            if self.get_packet_type(packet) == 0x02:  # COMMAND_REPLY
                return(self.parse_data(packet))
        else:
            return 0

    def parse_data(self, packet):
        """Parse the data"""
#        print (self.get_sensor_id(packet),
#                " becomes ",
#                self.converter.serial_to_db(self.get_sensor_id(packet)))
        return(self.get_node_id(packet),
               self.get_sensor_id(packet),
               self.get_payload(packet),
               str(self.now))

    def validate_packet(self, packet):
        """Validate an incoming packet using parity control"""
        if len(packet) == 15:
            self.received_parity = self.get_quality_check(packet)
            self.calculated_parity = (int(packet[1:3], 16)
                      ^ int(packet[4:6], 16)
                      ^ int(packet[7:9], 16)
                      ^ int(packet[10:12], 16))
#            print(self.received_parity)
#            print(self.calculated_parity)
            if self.received_parity == self.calculated_parity:
                return True
        else:
            return False

    def get_packet_type(self, packet):
        """Get the packet type"""
        if self.validate_packet(packet):
            if packet[0] == 'T':
                return self.hex_to_dec(packet[1:3])
            else:
                return 0

    def get_node_id(self, packet):
        """Get the node id"""
        if packet[3] == 'N':
            return self.hex_to_dec(packet[4:6])
        else:
            return 0

    def get_sensor_id(self, packet):
        """Get the sensor id"""
        if packet[6] == 'I':
            return self.hex_to_dec(packet[7:9])
        else:
            return 0

    def get_payload(self, packet):
        """Get the payload"""
        if packet[9] == 'P':
            return self.hex_to_dec(packet[10:12])
        else:
            return 0

    def get_quality_check(self, packet):
        """Get the parity 'quality check'"""
        if packet[12] == 'Q':
            return self.hex_to_dec(packet[13:15])
        else:
            return 0

    def hex_to_dec(self, hexvalue):
        """Convert hex value to decimal"""
        return int(hexvalue, 16)
