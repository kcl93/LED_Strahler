using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;

namespace LED_Strahler_GUI
{
    public class LED_Strahler_Backend
    {

        private MainWindow GUI = null;
        LED_Strahler_Serial Serial = new LED_Strahler_Serial();

        #region Constructor

        public LED_Strahler_Backend(MainWindow _gui)
        {
            GUI = _gui;

            GUI.ComPortList = new List<string>(SerialPort.GetPortNames());
            if(GUI.ComPortList.Count() != 0)
            {
                GUI.ComPort.SelectedIndex = 0;
            }
        }

        #endregion

        #region methods



        #endregion
    }
}
