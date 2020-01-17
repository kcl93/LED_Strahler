using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Windows;

namespace LED_Strahler_GUI
{
    public class LED_Strahler_Backend
    {

        private readonly MainWindow GUI = null;
        private readonly LED_Strahler_Serial Serial = new LED_Strahler_Serial();

        #region Constructor

        public LED_Strahler_Backend(MainWindow _gui)
        {
            GUI = _gui;

            GUI.ComPortList = new List<string>(SerialPort.GetPortNames());
            if(GUI.ComPortList.Count() != 0)
            {
                GUI.ComPort.SelectedIndex = 0;
            }

            //Connect serial port to all GroupControls
            this.GUI.GroupControl0.Serial = this.Serial;
            this.GUI.GroupControl1.Serial = this.Serial;
            this.GUI.GroupControl2.Serial = this.Serial;
            this.GUI.GroupControl3.Serial = this.Serial;
            this.GUI.GroupControl4.Serial = this.Serial;
            this.GUI.GroupControl5.Serial = this.Serial;

            //Link Buttons
            this.GUI.ConnectRefreshButton.Click += this.ConnectRefreshButtonClick;
            this.GUI.GetTemperatureButton.Click += this.GetTemperatureButtonClick;
        }

        #endregion

        #region methods

        public void ConnectRefreshButtonClick(object sender, RoutedEventArgs e)
        {
            //Open serial port
            if(this.GUI.ComPort.Text != null)
            {
                this.Serial.Open(this.GUI.ComPort.Text);
            }

            //Ping for devices
            this.Serial.PingRequest(out List<int> UUIDs);

            //Setup new device list
            List<LED_Strahler> DeviceList = new List<LED_Strahler>();
            foreach (int UUID in UUIDs)
            {
                DeviceList.Add(new LED_Strahler(UUID));
            }
            this.GUI.DeviceList = DeviceList;

            //Refresh datagrid
            this.GUI.DeviceGrid.Items.Refresh();
        }

        public void GetTemperatureButtonClick(object sender, RoutedEventArgs e)
        {
            foreach (LED_Strahler Strahler in this.GUI.DeviceList)
            {
                this.Serial.GetTemperature(Strahler);
            }

            //Refresh datagrid
            this.GUI.DeviceGrid.Items.Refresh();
        }

        #endregion
    }
}
