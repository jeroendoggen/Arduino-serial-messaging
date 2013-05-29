""" sp_controller: core code

This file is needed to import the module properly
Copyright (C) 2013  Jeroen Doggen <jeroendoggen@gmail.com>

"""

import sys

from sp_controller.controller import Controller


def run():
    """Run the main program"""
    controller = Controller()
    controller.run()
    return(controller.exit_value())


if __name__ == "__main__":
    sys.exit(run())
