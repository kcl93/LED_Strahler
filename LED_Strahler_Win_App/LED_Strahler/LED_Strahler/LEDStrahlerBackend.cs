using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Windows;
using System.Windows.Controls;
using System.ComponentModel;

namespace LED_Strahler_GUI
{
    public class LEDStrahlerBackend
    {

        private readonly MainWindow GUI = null;

        #region Constructor

        public LEDStrahlerBackend(MainWindow _gui)
        {
            GUI = _gui;

            //Link shutdown event
            this.GUI.Closing += new CancelEventHandler(this.GUIClosing);

            //Get available serial ports
            GUI.ComPortList = new List<string>(SerialPort.GetPortNames());
            if(GUI.ComPortList.Count() != 0)
            {
                GUI.ComPort.SelectedIndex = 0;
            }

            //Connect serial port to all GroupControls
            LEDStrahlerSerial.StartDispatcher();

            //Link Buttons
            this.GUI.ConnectRefreshButton.Click += this.ConnectRefreshButtonClick;
            this.GUI.GetTemperatureButton.Click += this.GetTemperatureButtonClick;
        }

        #endregion

        #region methods

        public void GUIClosing(object sender, CancelEventArgs e)
        {
            LEDStrahlerSerial.ShutdownDispatcher();
        }

        public void ConnectRefreshButtonClick(object sender, RoutedEventArgs e)
        {
            //Open serial port
            if(this.GUI.ComPort.Text != null)
            {
                LEDStrahlerSerial.Open(this.GUI.ComPort.Text);
            }

            //Ping for devices
            LEDStrahlerSerial.PingRequest(out List<uint> UUIDs);

            //Setup new device list
            List<LEDStrahler> DeviceList = new List<LEDStrahler>();
            foreach (uint UUID in UUIDs)
            {
                DeviceList.Add(new LEDStrahler(UUID));
            }
            this.GUI.DeviceList = DeviceList;

            //Read all temperatures once
            foreach (LEDStrahler Strahler in this.GUI.DeviceList)
            {
                LEDStrahlerSerial.GetTemperature(Strahler);
            }

            this.GUI.DeviceGrid.IsEnabled = true;
        }

        public void GetTemperatureButtonClick(object sender, RoutedEventArgs e)
        {
            foreach (LEDStrahler Strahler in this.GUI.DeviceList)
            {
                LEDStrahlerSerial.GetTemperature(Strahler);
            }
        }

        #endregion
    }
}
