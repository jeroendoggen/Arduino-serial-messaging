using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;
using sharp_arduino_serial_packet_lib;

namespace WpfTester
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            wr = new SerialReaderWriter(comport: "COM4");
        }

        private SerialReaderWriter wr;
        private bool listening = false;
        private void btnStartListening_Click(object sender, RoutedEventArgs e)
        {
            if (listening == false)
            {
                wr.SerialMessageReceived += wr_SerialMessageReceived;
                wr.StartListening();
                listening = true;
                btnStartListening.Content = "Stop listening";
            }
            else
            {
                wr.SerialMessageReceived -= wr_SerialMessageReceived;
                wr.StopListening();
                listening = false;
                btnStartListening.Content = "Start listening";
            }

        }

        private delegate void UpdateUiTextDelegate(Packet recPacket);
        void wr_SerialMessageReceived(object sender, SerialArduinoMessageEventArgs e)
        {
            Dispatcher.Invoke(
                DispatcherPriority.Send,
                new Action(
                    () =>
                    {
                        lbPackets.Items.Insert(0, e.Packet);
                    }
                        )
                            );

        }

    }

    public class CommandToText : IValueConverter
    {
        public object Convert(object value, Type targetType,
            object parameter, CultureInfo culture)
        {
            var incCom = (Commands)value;

            return incCom.ToString();
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
