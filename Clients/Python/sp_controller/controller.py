""" sp_controller: core code

TODO write some comments
TODO try to restart the threads when they exit
  - when disconnecting the usb the serial thread stops
  - when reconnecting, the thread should try to restart (after a timeout)

"""
from __future__ import print_function, division  # We require Python 2.6+
import time
import sys
from Queue import Queue


from sp_controller.settings import Settings


class QueueSetting:
    """The different queues for input/output"""
    def __init__(self, sensordata_queue, cmd_reply_queue, cmd_queue):
        self.sensordata_queue = sensordata_queue
        self.cmd_reply_queue = cmd_reply_queue
        self.cmd_queue = cmd_queue


class Controller:
    """Controller class: does the core of the work"""
    def __init__(self):
        """ Initialize the controller """
        self.sensordata_queue = Queue()
        self.cmd_reply_queue = Queue()
        self.cmd_queue = Queue()
        self.queue_settings = QueueSetting(self.sensordata_queue,
                                           self.cmd_reply_queue,
                                           self.cmd_queue)

        self.config = Settings(self.queue_settings)
        self.serial_read_thread = self.config.get_serial_read()
        self.serial_write_thread = self.config.get_serial_write()
        self.errors = 0

    def run(self):
        """The main thread for the controller"""
        self.start_serial_communication_threads()
        while True:
            try:
                time.sleep(100)
            except KeyboardInterrupt:
                print("Program stopped by keyboard interrupt")
                sys.exit(1)

    def start_serial_communication_threads(self):
        """Start serial communication thread"""
        self.serial_write_thread.daemon = True
        self.serial_write_thread.start()
        self.serial_read_thread.daemon = True
        self.serial_read_thread.start()

    def exit_value(self):
        """TODO: Generate the exit value for the application."""
        if (self.errors == 0):
            return 0
        else:
            return 42
