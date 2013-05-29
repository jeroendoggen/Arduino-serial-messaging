""" Print information messages

This is currently used to keep the info messages out of the other code

"""

from __future__ import print_function, division  # We require Python 2.6+


def double_line():
    """Print a double line"""
    print ("=============================================================")


def single_line():
    """Print a single line"""
    print ("-------------------------------------------------------------")


def top():
    """Print the top of the cli message"""
    print ("")
    double_line()
    print ("Starting sp_controller...")


def programflow(serial_port):
    """Print the program flow"""
    print ("")
    print ("Program flow: ")
    print (" 1. Connect to the Arduino on port: " + serial_port)
    print (" 2. Start polling for commands")
    print (" 3. Start writing incoming data to the terminal")
    print (" repeat forever... ")
    print ("")


def startup_info(serial_port):
    """Print an overview of the startup info"""
    top()
    programflow(serial_port)
