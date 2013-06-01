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
    sensordata_queue = Queue()
    cmd_reply_queue = Queue()
    cmd_queue = Queue()
    payload_counter = 0 
    
    def __init__(self):
        """ Initialize the controller """
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
                time.sleep(2)
                self.send_command()
            except KeyboardInterrupt:
                print("Program stopped by keyboard interrupt")
                sys.exit(1)
                
    def send_command(self):
        # Just send some dummy packets (with a counter for the payload)           
        self.cmd_queue.put((1, self.payload_counter, 2))
        print("A command has been placed in the queue")
        
        self.payload_counter += 1 
        if (self.payload_counter is 255):
            self.payload_counter = 0

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
