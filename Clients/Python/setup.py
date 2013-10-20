"""Setup file for sp

Define the options for the "sp_controller" package
Create source Python packages (python setup.py sdist)
Create binary Python packages (python setup.py bdist)

"""
from distutils.core import setup

from sp_controller import __version__


with open('README.md') as readme_file:
    LONG_DESCRIPTION = readme_file.read()

setup(name='sp_Controller',
      version=__version__,
      description='Controller for the SerialPacket protocol',
      long_description=LONG_DESCRIPTION,
      author='Jeroen Doggen',
      author_email='jeroendoggen@gmail.com',
      url='none',
      packages=['sp_controller'],
      package_data={'sp_controller': ['*.py', '*.conf']},
      license='LGPL-v2',
      platforms=['Linux'],
      )
