## Arduino sensor logging with collectd & RRDtool

Save Arduino sensor readings in a RRDtool database using 'collectd'.

Program flow:

 1. The Arduino reads its sensors.
 2. The Arduino sends a serial data packet to the 'server'.
 3. The server runs a c++ application to listen for serial packets.
 4. The server writes the payloads of the incoming packets to logfiles.
 5. Every 'x' seconds, the 'collectd' daemon runs a Ruby script to read the data from the logfiles.
 6. You can inspect, monitor, graph the info with any RRDtool compatible application. (e.g. Cacti, Nagios, KCollectd)

## KCollectd Screenshots

KCollectd is a RRDtool graphing frontend for KDE.

Screenhot shows: (sensing interval: 10 seconds)

 * Orange line: CPU usage
 * Green line: temperature sensor data (fake data: 'up/down counter')
 * Blue line: distance sensor data (fake data: 'up/down counter x 2')
 * Pink line: humidity sensor data (fake data: 'up/down counter + 100')

![KCollectd](KCollectd.png?raw=true)

## Links

 * [RRDtool](http://oss.oetiker.ch/rrdtool/): Industry standard, high performance data logging and graphing system for time series data.
 * [Nagios](http://www.nagios.org/): IT Infrastructure Monitoring
 * [KCollectd](http://www.forwiss.uni-passau.de/~berberic/Linux/kcollectd.html): a KDE-based application for displaying RRD data collected by collectd
 * [writing collectd plugins](http://support.rightscale.com/12-Guides/RightScale_101/08-Management_Tools/Monitoring_System/Writing_custom_collectd_plugins/Custom_Collectd_Plug-ins_for_Linux)