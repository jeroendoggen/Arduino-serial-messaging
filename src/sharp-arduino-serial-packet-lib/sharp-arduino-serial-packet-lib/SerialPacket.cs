using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sharp_arduino_serial_packet_lib
{
    public class SerialPacket
    {
        public SerialPacket()
        {
            _inComingPacketComplete = false;
            _incomingCounter = 0;

            _incomingPacket = new Packet()
                {
                    PacketType = 0,
                    NodeID = 0,
                    SensorID = 0,
                    CommandID = 0,
                    Payload = 0,
                    Parity = 0
                };

            newPacket = false;
        }

        public void Begin()
        {
            Begin(Defines.DEFAULT_BAUDRATE, 0);
        }
        public void Begin(long speed, byte nodeID)
        {
            throw new NotImplementedException();
        }

        public void SendCommand(byte commandID, byte payload)
        {
            PacketType = Defines.COMMAND;
            CommandId = commandID;
            sendPacket(payload);
        }
        public void SendCommand(byte payload)
        {
            PacketType = Defines.COMMAND;
            sendPacket(payload);
        }
        public void SendCommandReply(byte commandID, byte payload)
        {
            PacketType = Defines.COMMAND_REPLY;
            CommandId = commandID;
            sendPacket(payload);
        }
        public void SendDataRequest(byte sensorID, byte payload)
        {
            PacketType = Defines.DATA_REQUEST;
            SensorId = sensorID;
            sendPacket(payload);
        }
        public void SendData(byte sensorID, byte payload)
        {
            PacketType = Defines.DATA_BYTE;
            SensorId = sensorID;
            sendPacket(payload);
        }
        public void SendData(byte sensorID, int payload)
        {
            PacketType = Defines.DATA_INT;
            SensorId = sensorID;
            sendPacket(payload);
        }
        public void SendData(byte payload)
        {
            PacketType = Defines.DATA_BYTE;
            sendPacket(payload);
        }
        public void SendData(int payload)
        {
            PacketType = Defines.DATA_INT;
            sendPacket(payload);
        }
        public void SendDataArrayRequest(byte arrayID, byte payload)
        {
            PacketType = Defines.DATA_ARRAY_REQUEST;
            SensorId = arrayID;
            sendPacket(payload);

        }
        public void SendDataArray(byte[] dataArray, byte length)
        {
            throw new NotImplementedException();
        }

        public bool ReadSerialData()
        {
            throw new NotImplementedException();
        }
        public byte GetCommandID()
        {
            return _incomingPacket.CommandID;
        }
        public byte GetPayload()
        {
            return _incomingPacket.Payload;
        }

        private Packet _incomingPacket;
        private Packet _outgoingPacket;



        private byte _parity;
        private byte _checkedParity;

        private bool _inComingPacketComplete;
        private char[] _inputChar = new char[20];
        private byte _incomingCounter;

        //    private void sendPacket(byte& payload)
        private void sendPacket(byte payload)
        {
            throw new NotImplementedException();
        }

        //private void sendPacket(int16_t& payload)
        private void sendPacket(int payload)
        {
            throw new NotImplementedException();
        }


        public byte PacketType { get; set; }
        public byte CommandId { get; set; }
        public byte SensorId { get; set; }
        public byte NodeId { get; set; }


        //private void hexPrinting(byte& data)
        private void hexPrinting(byte data)
        {
            throw new NotImplementedException();
        }

        //private void hexPrinting(int16_t& data)
        private void hexPrinting(int data)
        {
            throw new NotImplementedException();
        }

        private byte hex_to_dec(byte inp)
        {
            throw new NotImplementedException();
        }

        private bool parseSerialData()
        {
            throw new NotImplementedException();
        }

        private void printInfo()
        {
            throw new NotImplementedException();
        }

        private bool checkParity()
        {
            throw new NotImplementedException();
        }

        private bool newPacket;
    }
}
