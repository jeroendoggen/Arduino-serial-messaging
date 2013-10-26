using System;

namespace sharp_arduino_serial_packet_lib
{
    static class ExtensionsMethods
    {

        public static int FromHexStringToInt(this  string hexstr)
        {
            if (hexstr.Length == 2)
            {
                return Convert.ToInt32(hexstr, 16);
            }
            throw new IndexOutOfRangeException("Can only create int from 2-length hex arrays. String received: " + hexstr);
        }

        public static string FromIntToHexString(this  int hexstr)
        {
            //TODO
            throw new NotImplementedException();
        }

        public static T[] SubArray<T>(this T[] data, int index, int length)
        {
            var result = new T[length];
            Array.Copy(data, index, result, 0, length);
            return result;
        }
    }
}