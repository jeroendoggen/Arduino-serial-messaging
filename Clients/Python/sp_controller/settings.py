""" sp_controller: Settings class

get the cli arguments
set the logfile
set the serial port

"""

from __future__ import print_function, division  # We require Python 2.6+

import argparse
import logging
import os
import sys

import infoprinter
from sp_controller.serialcommunication import SerialCommunication
from sp_controller.__init__ import __version__


DEFAULT_PORT = "/dev/ttyUSB0"
DEFAULT_BAUDRATE = 115200
DEFAULT_LOGFILE = "sp_controller.log"
DEFAULT_SERIAL_READ_SLEEPTIME = 0.1
DEFAULT_SERIAL_WRITE_SLEEPTIME = 0.1
#keep this around 0.5 (Arduino gets confused!! (buffer overflow?)
DEFAULT_PATH = os.getcwd() + "/"


class SerialportSetting:
    """The serial port settings"""
    def __init__(self, serial_port, baudrate):
        self.serial_port = serial_port
        self.baudrate = baudrate


class Settings:
    """Configure the settings of the program"""
    path = DEFAULT_PATH
    logfile = path + DEFAULT_LOGFILE

    serial_read_sleeptime = DEFAULT_SERIAL_READ_SLEEPTIME
    serial_write_sleeptime = DEFAULT_SERIAL_WRITE_SLEEPTIME

    parser = argparse.ArgumentParser(
        prog="sp_controller",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        description="sp_controller commandline arguments:",
        epilog="Report bugs to jeroendoggen@gmail.com.")

    def __init__(self, queue_settings):
        """Initialize"""
        self.serialport_settings = SerialportSetting(DEFAULT_PORT,
                                                     DEFAULT_BAUDRATE)
        self.queue_settings = queue_settings
        self.cli_arguments()
        infoprinter.startup_info(self.serialport_settings.serial_port)
        self.set_logfile()
        self.set_serialport()

    def cli_arguments(self):
        """Configure a read all the cli arguments."""
        self.configure_cli_arguments()
        self.get_cli_arguments()

    def configure_cli_arguments(self):
        """Configure all the cli arguments."""
        self.parser.add_argument("-p", metavar="port",
          help="Set the name of the serial port")
        self.parser.add_argument("-f", metavar="file",
          help="Set the name of the logfile")
        self.parser.add_argument("-b", metavar="baudrate",
          help="Set the baudrate of the serial port")

    def get_cli_arguments(self):
        """Read all the cli arguments."""
        args = self.parser.parse_args()
        if (args.p is not None):
            self.serialport_settings.serial_port = args.p
        if (args.b is not None):
            self.serialport_settings.baudrate = args.b
        if (args.f is not None):
            self.logfile = self.path + args.f

    def set_logfile(self):
        """Set the logfile where we will write error & info messages"""
        try:
            logging.basicConfig(filename=self.logfile,
                level=logging.DEBUG,
                format="%(asctime)s %(name)s %(levelname)s %(message)s")
            self.logger = logging.getLogger(__name__)
        except IOError:
            print("Unable to open logfile")
            print("Do you have write access in the current folder?")
            sys.exit(0)

    def set_serialport(self):
        """Set the serial port"""
        #TODO: they should only get one queue (not two)
        self.sp_read = SerialCommunication("Packet_receiver    ",
                                           self.serialport_settings,
                                           self.logger,
                                           self.serial_read_sleeptime,
                                           self.queue_settings)
        self.sp_write = SerialCommunication("Command_transmitter",
                                            self.serialport_settings,
                                            self.logger,
                                            self.serial_write_sleeptime,
                                            self.queue_settings)

    def get_serial_read(self):
        """Get the serialcommunication object (used in controller)"""
        return(self.sp_read)

    def get_serial_write(self):
        """Get the serialcommunication object (used in controller)"""
        return(self.sp_write)
