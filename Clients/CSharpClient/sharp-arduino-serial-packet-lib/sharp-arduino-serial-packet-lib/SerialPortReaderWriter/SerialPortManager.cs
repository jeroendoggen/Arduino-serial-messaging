using System;
using System.IO.Ports;
using System.Reflection;

namespace sharp_arduino_serial_packet_lib.SerialPortReaderWriter
{
    /// <summary>
    /// Manager for serial port data
    /// </summary>
    //Original source: http://www.codeproject.com/Articles/75770/Basic-serial-port-listening-application
    class SerialPortManager : IDisposable
    {
        public SerialPortManager()
        {
            // Finding installed serial ports on hardware
            _currentSerialSettings.PortNameCollection = SerialPort.GetPortNames();
            _currentSerialSettings.PropertyChanged += _currentSerialSettings_PropertyChanged;

            // If serial ports is found, we select the first found
            if (_currentSerialSettings.PortNameCollection.Length > 0)
                _currentSerialSettings.PortName = _currentSerialSettings.PortNameCollection[0];
        }


        ~SerialPortManager()
        {
            Dispose(false);
        }


        #region Fields
        private SerialPort _serialPort;
        private SerialSettings _currentSerialSettings = new SerialSettings();
        public event EventHandler<SerialDataEventArgs> NewSerialDataReceived;

        #endregion

        #region Properties
        /// <summary>
        /// Gets or sets the current serial port settings
        /// </summary>
        public SerialSettings CurrentSerialSettings
        {
            get { return _currentSerialSettings; }
            set { _currentSerialSettings = value; }
        }

        #endregion

        #region Event handlers

        void _currentSerialSettings_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            // if serial port is changed, a new baud query is issued
            if (e.PropertyName.Equals("PortName"))
                UpdateBaudRateCollection();
        }


        void _serialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string nbrDataRead = _serialPort.ReadLine();
            if (nbrDataRead.Length == 0)
                return;

            // Send data to whom ever interested
            if (NewSerialDataReceived != null)
                NewSerialDataReceived(this, new SerialDataEventArgs(nbrDataRead));
        }

        #endregion

        #region Methods

        /// <summary>
        /// Connects to a serial port defined through the current settings
        /// </summary>
        public void StartUsingPort()
        {
            // Closing serial port if it is open
            if (_serialPort != null && _serialPort.IsOpen)
                _serialPort.Close();

            // Setting serial port settings
            _serialPort = new SerialPort(
                _currentSerialSettings.PortName,
                _currentSerialSettings.BaudRate,
                _currentSerialSettings.Parity,
                _currentSerialSettings.DataBits,
                _currentSerialSettings.StopBits);

            // Subscribe to event and open serial port for data
            _serialPort.DataReceived += _serialPort_DataReceived;
            _serialPort.Open();
        }

        /// <summary>
        /// Closes the serial port
        /// </summary>
        public void StopUsingPort()
        {
            _serialPort.Close();
        }

        /// <summary>
        /// Send serial byte array to serialport
        /// </summary>
        /// <param name="data"></param>
        public void SendSerialData(string data)
        {
            if (_serialPort != null && _serialPort.IsOpen)
            {
                _serialPort.WriteLine(data);
                //TODO: http://www.ibiliskov.info/2011/05/net-serialport-pitfalls/
            }
            else
            {
                throw new Exception("No serial port open. Did you call the StartUsingPort method?");
            }
        }


        /// <summary>
        /// Retrieves the current selected device's COMMPROP structure, and extracts the dwSettableBaud property
        /// </summary>
        private void UpdateBaudRateCollection()
        {
            _serialPort = new SerialPort(_currentSerialSettings.PortName);
            _serialPort.Open();
            var field = _serialPort.BaseStream.GetType().GetField("commProp", BindingFlags.Instance | BindingFlags.NonPublic);
            if (field != null)
            {
                var p = field.GetValue(_serialPort.BaseStream);

                if (p != null)
                {
                    var fieldInfo = p.GetType().GetField("dwSettableBaud", BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);
                    if (fieldInfo != null)
                    {
                        var dwSettableBaud = (Int32)fieldInfo.GetValue(p);

                        _serialPort.Close();
                        _currentSerialSettings.UpdateBaudRateCollection(dwSettableBaud);
                    }
                }

            }
        }

        // Call to release serial port
        public void Dispose()
        {
            Dispose(true);
        }

        // Part of basic design pattern for implementing Dispose
        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                _serialPort.DataReceived -= _serialPort_DataReceived;
            }
            // Releasing serial port (and other unmanaged objects)
            if (_serialPort != null)
            {
                if (_serialPort.IsOpen)
                    _serialPort.Close();

                _serialPort.Dispose();
            }
        }


        #endregion

    }

    /// <summary>
    /// EventArgs used to send bytes received on serial port
    /// </summary>
    public class SerialDataEventArgs : EventArgs
    {
        public SerialDataEventArgs(string dataInByteArray)
        {
            Data = dataInByteArray;
        }

        /// <summary>
        /// Byte array containing data from serial port
        /// </summary>
        public string Data;
    }
}
